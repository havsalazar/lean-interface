/**
 * @file imu_task.c
 *
 * MPU-6050 -> lean angle + longitudinal/lateral g -> UI subjects.
 *
 * Mounting: any orientation. "Up" is learned during calibration (the device
 * must be still and upright then). Only the forward direction has to be told,
 * with IMU_FWD below: by default the "X" arrow printed on the GY-521 points
 * toward the front of the vehicle.
 *
 * Lean: the roll gyro is integrated, and slowly pulled toward
 *   - the gravity direction when riding straight, and
 *   - atan(-yaw_lateral / yaw_vertical) while turning. In a steady turn the
 *     accelerometer only sees "down" through the vehicle, so on a motorcycle
 *     it reads ~0 lean even at 45 deg; the split of the turn rate between the
 *     two gyro axes is what reveals the real lean, with no GPS speed needed.
 */
#include "imu_task.h"

#include <math.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "lvgl.h"
#include "inclin_ui.h"   /* exported UI: subject_lean_deg, subject_accel_cg, ... */

/*********************
 *  CONFIGURATION
 *********************/
#define IMU_SDA_GPIO        38
#define IMU_SCL_GPIO        39
#define IMU_I2C_ADDR        0x68    /* 0x69 if AD0 is tied high */
#define IMU_I2C_HZ          400000

/* Sensor axis that points toward the front of the vehicle, in the sensor's own
 * frame. {1,0,0} = +X arrow forward, {-1,0,0} = X arrow backward, {0,1,0} = +Y ... */
#define IMU_FWD             { 1.0f, 0.0f, 0.0f }

/* 1: motorcycle/bicycle (leans into turns): lateral g = tan(lean).
 * 0: car/boat (stays level): lateral g is measured directly. */
#define IMU_LEANING_VEHICLE 1

/* Flip a direction if your mounting makes it read backwards */
#define IMU_LEAN_SIGN       1       /* -1 swaps left and right lean */
#define IMU_ACCEL_SIGN      1       /* -1 swaps accelerating and braking */

#define IMU_RATE_HZ         100
#define IMU_CAL_SAMPLES     200     /* 2 s at 100 Hz */
#define IMU_FUSION_K        0.01f   /* pull toward the reference per sample, ~1 s time constant */
#define IMU_TURN_DPS        8.0f    /* above this turn rate the gyro split is the lean reference */
#define IMU_G_FILTER        0.15f   /* low-pass on the g values shown */
#define IMU_G_DEADBAND_CG   3       /* below 0.03 g the accel/decel bars stay at 0 */

/* MPU-6050 registers */
#define REG_SMPLRT_DIV      0x19
#define REG_CONFIG          0x1A
#define REG_GYRO_CONFIG     0x1B
#define REG_ACCEL_CONFIG    0x1C
#define REG_ACCEL_XOUT_H    0x3B
#define REG_PWR_MGMT_1      0x6B
#define REG_WHO_AM_I        0x75

#define ACCEL_LSB_PER_G     8192.0f /* +-4 g */
#define GYRO_LSB_PER_DPS    65.5f   /* +-500 dps */

#define RAD2DEG             57.29578f

static const char * TAG = "imu";

/*********************
 *  STATE
 *********************/
static i2c_master_dev_handle_t s_dev;

/* Written by the IMU task, read by the LVGL timer */
static portMUX_TYPE s_mux = portMUX_INITIALIZER_UNLOCKED;
static int32_t s_lean_deg;
static int32_t s_accel_cg;
static int32_t s_lat_cg;
static volatile bool s_cal_request = true;
static volatile bool s_calibrating = true;

typedef struct { float x, y, z; } vec3_t;

static float v_dot(vec3_t a, vec3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static vec3_t v_scale(vec3_t a, float s) { return (vec3_t){ a.x * s, a.y * s, a.z * s }; }
static vec3_t v_sub(vec3_t a, vec3_t b) { return (vec3_t){ a.x - b.x, a.y - b.y, a.z - b.z }; }
static vec3_t v_cross(vec3_t a, vec3_t b)
{
    return (vec3_t){ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}
static vec3_t v_norm(vec3_t a)
{
    float l = sqrtf(v_dot(a, a));
    return l > 1e-6f ? v_scale(a, 1.0f / l) : a;
}

/*********************
 *  MPU-6050
 *********************/
static esp_err_t reg_write(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };
    return i2c_master_transmit(s_dev, buf, sizeof(buf), 50);
}

static esp_err_t reg_read(uint8_t reg, uint8_t * buf, size_t len)
{
    return i2c_master_transmit_receive(s_dev, &reg, 1, buf, len, 50);
}

static esp_err_t mpu_init(void)
{
    uint8_t who = 0;
    esp_err_t err = reg_read(REG_WHO_AM_I, &who, 1);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "No answer at 0x%02X on SDA=%d SCL=%d: check the wiring (%s)",
                 IMU_I2C_ADDR, IMU_SDA_GPIO, IMU_SCL_GPIO, esp_err_to_name(err));
        return err;
    }
    /* Genuine parts answer 0x68; common clones answer 0x70, 0x72 or 0x98 and work the same */
    ESP_LOGI(TAG, "WHO_AM_I = 0x%02X", who);

    err  = reg_write(REG_PWR_MGMT_1, 0x80);              /* reset */
    vTaskDelay(pdMS_TO_TICKS(100));
    err |= reg_write(REG_PWR_MGMT_1, 0x01);              /* wake, PLL on gyro X */
    vTaskDelay(pdMS_TO_TICKS(10));
    err |= reg_write(REG_CONFIG, 0x03);                  /* DLPF 44 Hz, 1 kHz internal rate */
    err |= reg_write(REG_SMPLRT_DIV, 1000 / IMU_RATE_HZ - 1);
    err |= reg_write(REG_GYRO_CONFIG, 0x08);             /* +-500 dps */
    err |= reg_write(REG_ACCEL_CONFIG, 0x08);            /* +-4 g */
    return err == ESP_OK ? ESP_OK : ESP_FAIL;
}

/** Accel in g and gyro in deg/s, in the sensor frame */
static esp_err_t mpu_read(vec3_t * acc, vec3_t * gyr)
{
    uint8_t b[14];
    esp_err_t err = reg_read(REG_ACCEL_XOUT_H, b, sizeof(b));
    if(err != ESP_OK) return err;

#define RAW(i) ((int16_t)((b[i] << 8) | b[(i) + 1]))
    acc->x = RAW(0) / ACCEL_LSB_PER_G;
    acc->y = RAW(2) / ACCEL_LSB_PER_G;
    acc->z = RAW(4) / ACCEL_LSB_PER_G;
    /* b[6..7] is the temperature */
    gyr->x = RAW(8) / GYRO_LSB_PER_DPS;
    gyr->y = RAW(10) / GYRO_LSB_PER_DPS;
    gyr->z = RAW(12) / GYRO_LSB_PER_DPS;
#undef RAW
    return ESP_OK;
}

/*********************
 *  TASK
 *********************/
static void imu_task(void * arg)
{
    LV_UNUSED(arg);

    vec3_t gyro_bias = { 0 };
    vec3_t ax_fwd = { 0 }, ax_right = { 0 }, ax_up = { 0 }; /* vehicle frame in sensor coordinates */
    float roll = 0.0f, accel_g = 0.0f, lat_g = 0.0f;
    int64_t last_us = esp_timer_get_time();
    TickType_t wake = xTaskGetTickCount();

    while(1) {
        vTaskDelayUntil(&wake, pdMS_TO_TICKS(1000 / IMU_RATE_HZ));

        if(s_cal_request) {
            s_calibrating = true;
            ESP_LOGI(TAG, "Calibrating: keep still and upright for 2 s");
            vec3_t sum_a = { 0 }, sum_g = { 0 };
            int n = 0;
            while(n < IMU_CAL_SAMPLES) {
                vec3_t a, g;
                vTaskDelayUntil(&wake, pdMS_TO_TICKS(1000 / IMU_RATE_HZ));
                if(mpu_read(&a, &g) != ESP_OK) continue;
                sum_a = (vec3_t){ sum_a.x + a.x, sum_a.y + a.y, sum_a.z + a.z };
                sum_g = (vec3_t){ sum_g.x + g.x, sum_g.y + g.y, sum_g.z + g.z };
                n++;
            }
            gyro_bias = v_scale(sum_g, 1.0f / n);
            ax_up = v_norm(sum_a);                       /* at rest the accelerometer reads +1 g "up" */

            vec3_t fwd = IMU_FWD;
            ax_fwd = v_norm(v_sub(fwd, v_scale(ax_up, v_dot(fwd, ax_up)))); /* level it */
            if(fabsf(v_dot(v_norm(fwd), ax_up)) > 0.9f) {
                ESP_LOGW(TAG, "IMU_FWD points almost straight up: set it to the axis that points forward");
            }
            ax_right = v_cross(ax_fwd, ax_up);

            roll = accel_g = lat_g = 0.0f;
            last_us = esp_timer_get_time();
            s_cal_request = false;
            s_calibrating = false;
            ESP_LOGI(TAG, "Calibrated: up=(%.2f %.2f %.2f) gyro bias=(%.2f %.2f %.2f) dps",
                     ax_up.x, ax_up.y, ax_up.z, gyro_bias.x, gyro_bias.y, gyro_bias.z);
            continue;
        }

        vec3_t a, g;
        if(mpu_read(&a, &g) != ESP_OK) {
            ESP_LOGW(TAG, "read failed");
            continue;
        }
        g = v_sub(g, gyro_bias);

        int64_t now_us = esp_timer_get_time();
        float dt = (now_us - last_us) / 1e6f;
        last_us = now_us;
        if(dt <= 0.0f || dt > 0.1f) dt = 1.0f / IMU_RATE_HZ;

        /* Into the vehicle frame */
        float a_fwd = v_dot(a, ax_fwd), a_right = v_dot(a, ax_right), a_up = v_dot(a, ax_up);
        float w_fwd = v_dot(g, ax_fwd), w_right = v_dot(g, ax_right), w_up = v_dot(g, ax_up);

        /* Lean: integrate the roll rate (positive about "forward" = top tilts right) ... */
        roll += w_fwd * dt;

        /* ... and correct the drift toward a reference */
        float turn_rate = sqrtf(w_up * w_up + w_right * w_right);
        float ref;
        if(turn_rate > IMU_TURN_DPS && fabsf(w_up) > 1.0f) {
            ref = atanf(-w_right / w_up) * RAD2DEG;      /* turning: split of the yaw rate */
        }
        else {
            ref = atan2f(-a_right, a_up) * RAD2DEG;      /* straight: gravity direction */
        }
        roll += IMU_FUSION_K * (ref - roll);

        /* Longitudinal g, and lateral g */
        accel_g += IMU_G_FILTER * (a_fwd - accel_g);
#if IMU_LEANING_VEHICLE
        float lat_now = tanf(fmaxf(fminf(roll, 70.0f), -70.0f) / RAD2DEG);
#else
        float lat_now = a_right;
#endif
        lat_g += IMU_G_FILTER * (lat_now - lat_g);

        taskENTER_CRITICAL(&s_mux);
        s_lean_deg = IMU_LEAN_SIGN * (int32_t)lroundf(roll);
        s_accel_cg = IMU_ACCEL_SIGN * (int32_t)lroundf(accel_g * 100.0f);
        s_lat_cg   = IMU_LEAN_SIGN * (int32_t)lroundf(lat_g * 100.0f);
        taskEXIT_CRITICAL(&s_mux);
    }
}

esp_err_t imu_task_start(i2c_master_bus_handle_t bus)
{
    if(bus == NULL) {
        i2c_master_bus_config_t bus_cfg = {
            .i2c_port = -1,                              /* any free port */
            .sda_io_num = IMU_SDA_GPIO,
            .scl_io_num = IMU_SCL_GPIO,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags.enable_internal_pullup = true,        /* the GY-521 has its own pull-ups too */
        };
        esp_err_t err = i2c_new_master_bus(&bus_cfg, &bus);
        if(err != ESP_OK) {
            ESP_LOGE(TAG, "i2c_new_master_bus failed: %s (is IO38/IO39 already used by another bus?)",
                     esp_err_to_name(err));
            return err;
        }
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = IMU_I2C_ADDR,
        .scl_speed_hz = IMU_I2C_HZ,
    };
    esp_err_t err = i2c_master_bus_add_device(bus, &dev_cfg, &s_dev);
    if(err != ESP_OK) return err;

    err = mpu_init();
    if(err != ESP_OK) return err;

    return xTaskCreate(imu_task, "imu", 4096, NULL, 5, NULL) == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}

void imu_request_calibration(void)
{
    s_cal_request = true;
}

/*********************
 *  UI
 *********************/
static void ui_update_cb(lv_timer_t * t)
{
    LV_UNUSED(t);
    int32_t lean, accel;
    taskENTER_CRITICAL(&s_mux);
    lean = s_lean_deg;
    accel = s_accel_cg;
    taskEXIT_CRITICAL(&s_mux);

    bool calibrating = s_calibrating;
    lv_subject_set_int(subject_imu_calibrating, calibrating ? 1 : 0);
    if(calibrating) {
        /* Show level and zero g while calibrating instead of stale values */
        lean = accel = 0;
    }

    lv_subject_set_int(subject_lean_deg, lean);
    lv_subject_set_int(subject_lean_abs_deg, lean < 0 ? -lean : lean);
    lv_subject_set_int(subject_accel_cg, accel);

    /* The two bars each show a magnitude: braking on the left, accelerating on the right.
     * A small deadband keeps both at 0.0 when standing still. */
    int32_t decel = accel < -IMU_G_DEADBAND_CG ? -accel : 0;
    int32_t acc   = accel >  IMU_G_DEADBAND_CG ?  accel : 0;
    lv_subject_set_int(subject_g_decel_cg, decel);
    lv_subject_set_int(subject_g_accel_cg, acc);

    /* The UI resets these to 0 on a long press, so compare against the subject itself */
    if(-lean > lv_subject_get_int(subject_lean_max_left))  lv_subject_set_int(subject_lean_max_left, -lean);
    if(lean  > lv_subject_get_int(subject_lean_max_right)) lv_subject_set_int(subject_lean_max_right, lean);
}

/* The "RESET GYRO" button sets subject_imu_cal_request to 1 */
static void cal_request_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(observer);
    if(lv_subject_get_int(subject) == 1) {
        ESP_LOGI(TAG, "Gyro reset requested from the screen");
        s_calibrating = true;           /* show "HOLD STILL" right away */
        imu_request_calibration();
        lv_subject_set_int(subject, 0);
    }
}

void imu_ui_bind(void)
{
    lv_subject_add_observer(subject_imu_cal_request, cal_request_cb, NULL);
    lv_timer_create(ui_update_cb, 33, NULL);
}

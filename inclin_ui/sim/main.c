/**
 * @file main.c
 *
 * PC simulator entry point for a UI exported from LVGL Pro: opens a window,
 * initializes the exported UI library and starts the LVGL loop.
 */

/* Built only by sim/CMakeLists.txt; lets a glob-based embedded build skip it. */
#ifdef LVGL_PRO_SIMULATOR_BUILD

#include "lvgl.h"
#include "hal.h"
#include UI_HEADER   /* exported UI library: header, UI_INIT and size come from
                        project.xml via sim/CMakeLists.txt */

/* Simulator only: sweep lean +-55 deg and accel/brake +-0.9 g. On the device
 * firmware/imu_task.c writes the same subjects from the MPU-6050. */
static void demo_motion_cb(lv_timer_t * t)
{
    LV_UNUSED(t);
    static uint32_t tick;
    tick++;

    int32_t lean  = 55 * lv_trigo_sin((int16_t)((tick * 2) % 360)) / LV_TRIGO_SIN_MAX;
    int32_t accel = 90 * lv_trigo_sin((int16_t)((tick * 5) % 360)) / LV_TRIGO_SIN_MAX;

    lv_subject_set_int(subject_lean_deg, lean);
    lv_subject_set_int(subject_accel_cg, accel);
    lv_subject_set_int(subject_g_decel_cg, accel < 0 ? -accel : 0);
    lv_subject_set_int(subject_g_accel_cg, accel > 0 ? accel : 0);

    /* Fake the gyro reset: "HOLD STILL" for 2 s after the button is pressed */
    static uint32_t cal_until;
    if(lv_subject_get_int(subject_imu_cal_request) == 1) {
        lv_subject_set_int(subject_imu_cal_request, 0);
        cal_until = tick + 60;
    }
    lv_subject_set_int(subject_imu_calibrating, tick < cal_until ? 1 : 0);

    if(-lean > lv_subject_get_int(subject_lean_max_left))  lv_subject_set_int(subject_lean_max_left, -lean);
    if(lean  > lv_subject_get_int(subject_lean_max_right)) lv_subject_set_int(subject_lean_max_right, lean);
}

int main(void)
{
    lv_init();

    /* Before lv_lock(): the Windows driver's window thread needs the mutex. */
    hal_init(UI_WIDTH, UI_HEIGHT);

    /* Hold the LVGL mutex while building the UI: a no-op without an OS, but
     * required to stay off the Windows driver's render thread. */
    lv_lock();

    UI_INIT("A:");      /* "A:" = file-system drive for file-based assets */

    lv_screen_load(lean_screen_create());

    /* No IMU on the PC: fake a rider leaning and braking so the UI moves. */
    lv_timer_create(demo_motion_cb, 33, NULL);

    lv_unlock();

    while(1) {
        uint32_t idle_ms = lv_timer_handler();
        if(idle_ms == LV_NO_TIMER_READY) idle_ms = LV_DEF_REFR_PERIOD;
        lv_sleep_ms(idle_ms);
    }

    return 0;
}

#endif /*LVGL_PRO_SIMULATOR_BUILD*/

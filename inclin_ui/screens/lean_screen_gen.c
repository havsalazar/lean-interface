/**
 * @file lean_screen_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "lean_screen_gen.h"
#include "../inclin_ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

static lv_style_t * lean_screen_style_screen_warn_ref; /* set in create(), read by a binding callback */
static lv_style_t * lean_screen_style_screen_danger_ref; /* set in create(), read by a binding callback */
static lv_style_t * lean_screen_style_gauge_fill_danger_ref; /* set in create(), read by a binding callback */
static lv_style_t * lean_screen_style_accent_line_danger_ref; /* set in create(), read by a binding callback */
static lv_style_t * lean_screen_style_title_danger_ref; /* set in create(), read by a binding callback */
static void lean_screen_lv_obj_0_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_obj_0_style_enabled_cb_2(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_set_subject_cb(lv_event_t * e);
static void lean_screen_set_subject_cb_2(lv_event_t * e);
static void lean_screen_lv_label_0_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_arc_0_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_2_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_2_style_text_color_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_3_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_4_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_5_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_obj_6_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_6_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_6_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_button_0_disabled_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_set_subject_cb_3(lv_event_t * e);
static void lean_screen_lv_label_7_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);
static void lean_screen_lv_label_8_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lean_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_screen;
    static lv_style_t style_screen_warn;
    static lv_style_t style_screen_danger;
    static lv_style_t style_column;
    static lv_style_t style_gauge_track;
    static lv_style_t style_gauge_fill;
    static lv_style_t style_gauge_knob;
    static lv_style_t style_lean_value;
    static lv_style_t style_g_track;
    static lv_style_t style_g_fill;
    static lv_style_t style_g_scale;
    static lv_style_t style_g_scale_major;
    static lv_style_t style_accent_line;
    static lv_style_t style_title;
    static lv_style_t style_gauge_fill_danger;
    static lv_style_t style_accent_line_danger;
    static lv_style_t style_title_danger;
    static lv_style_t style_button;
    static lv_style_t style_button_pressed;
    static lv_style_t style_button_disabled;

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_screen);
        lv_style_init(&style_screen_warn);
        lv_style_init(&style_screen_danger);
        lv_style_init(&style_column);
        lv_style_init(&style_gauge_track);
        lv_style_init(&style_gauge_fill);
        lv_style_init(&style_gauge_knob);
        lv_style_init(&style_lean_value);
        lv_style_init(&style_g_track);
        lv_style_init(&style_g_fill);
        lv_style_init(&style_g_scale);
        lv_style_init(&style_g_scale_major);
        lv_style_init(&style_accent_line);
        lv_style_init(&style_title);
        lv_style_init(&style_gauge_fill_danger);
        lv_style_init(&style_accent_line_danger);
        lv_style_init(&style_title_danger);
        lv_style_init(&style_button);
        lv_style_init(&style_button_pressed);
        lv_style_init(&style_button_disabled);

        lv_style_set_bg_color(&style_screen, COLOR_BG_TOP);
        lv_style_set_bg_grad_color(&style_screen, COLOR_BG_BOTTOM);
        lv_style_set_bg_grad_dir(&style_screen, LV_GRAD_DIR_VER);
        lv_style_set_bg_main_stop(&style_screen, 120);
        lv_style_set_bg_opa(&style_screen, (255 * 100 / 100));
        lv_style_set_text_color(&style_screen, COLOR_TEXT);
        lv_style_set_text_font(&style_screen, font_small);
        lv_style_set_pad_all(&style_screen, SPACE_SM);
        lv_style_set_pad_column(&style_screen, SPACE_SM);
        lv_style_set_bg_color(&style_screen_warn, COLOR_BG_WARN_TOP);
        lv_style_set_bg_grad_color(&style_screen_warn, COLOR_BG_WARN_BOTTOM);
        lv_style_set_bg_color(&style_screen_danger, COLOR_BG_DANGER_TOP);
        lv_style_set_bg_grad_color(&style_screen_danger, COLOR_BG_DANGER_BOTTOM);
        lv_style_set_bg_opa(&style_column, 0);
        lv_style_set_border_width(&style_column, 0);
        lv_style_set_pad_all(&style_column, 0);
        lv_style_set_radius(&style_column, 0);
        lv_style_set_pad_row(&style_column, SPACE_XS);
        lv_style_set_arc_color(&style_gauge_track, COLOR_GAUGE);
        lv_style_set_arc_width(&style_gauge_track, LEAN_GAUGE_WIDTH);
        lv_style_set_arc_rounded(&style_gauge_track, false);
        lv_style_set_bg_opa(&style_gauge_track, 0);
        lv_style_set_border_width(&style_gauge_track, 0);
        lv_style_set_pad_all(&style_gauge_track, 0);
        lv_style_set_arc_color(&style_gauge_fill, COLOR_ACCENT);
        lv_style_set_arc_width(&style_gauge_fill, LEAN_GAUGE_WIDTH);
        lv_style_set_arc_rounded(&style_gauge_fill, false);
        lv_style_set_bg_opa(&style_gauge_knob, 0);
        lv_style_set_border_width(&style_gauge_knob, 0);
        lv_style_set_shadow_width(&style_gauge_knob, 0);
        lv_style_set_outline_width(&style_gauge_knob, 0);
        lv_style_set_text_font(&style_lean_value, font_lean);
        lv_style_set_bg_color(&style_g_track, COLOR_TRACK);
        lv_style_set_bg_opa(&style_g_track, (255 * 100 / 100));
        lv_style_set_radius(&style_g_track, 3);
        lv_style_set_pad_all(&style_g_track, 0);
        lv_style_set_bg_color(&style_g_fill, COLOR_GAUGE);
        lv_style_set_bg_opa(&style_g_fill, (255 * 100 / 100));
        lv_style_set_radius(&style_g_fill, 3);
        lv_style_set_line_color(&style_g_scale, COLOR_TEXT_MUTED);
        lv_style_set_line_width(&style_g_scale, 1);
        lv_style_set_text_color(&style_g_scale, COLOR_TEXT_MUTED);
        lv_style_set_text_font(&style_g_scale, font_small);
        lv_style_set_length(&style_g_scale, 6);
        lv_style_set_line_color(&style_g_scale_major, COLOR_TEXT);
        lv_style_set_line_width(&style_g_scale_major, 2);
        lv_style_set_length(&style_g_scale_major, 10);
        lv_style_set_text_color(&style_g_scale_major, COLOR_TEXT);
        lv_style_set_bg_color(&style_accent_line, COLOR_ACCENT);
        lv_style_set_bg_opa(&style_accent_line, (255 * 100 / 100));
        lv_style_set_border_width(&style_accent_line, 0);
        lv_style_set_radius(&style_accent_line, 0);
        lv_style_set_pad_all(&style_accent_line, 0);
        lv_style_set_text_color(&style_title, COLOR_ACCENT_TEXT);
        lv_style_set_text_font(&style_title, font_title);
        lv_style_set_arc_color(&style_gauge_fill_danger, COLOR_DANGER);
        lv_style_set_bg_color(&style_accent_line_danger, COLOR_DANGER);
        lv_style_set_text_color(&style_title_danger, COLOR_TEXT);
        lv_style_set_bg_color(&style_button, COLOR_BG_TOP);
        lv_style_set_bg_opa(&style_button, (255 * 60 / 100));
        lv_style_set_border_color(&style_button, COLOR_ACCENT);
        lv_style_set_border_width(&style_button, 2);
        lv_style_set_radius(&style_button, 16);
        lv_style_set_shadow_width(&style_button, 0);
        lv_style_set_pad_hor(&style_button, SPACE_MD);
        lv_style_set_pad_ver(&style_button, SPACE_XS);
        lv_style_set_text_color(&style_button, COLOR_TEXT);
        lv_style_set_text_font(&style_button, font_small);
        lv_style_set_bg_color(&style_button_pressed, COLOR_ACCENT);
        lv_style_set_bg_opa(&style_button_pressed, (255 * 100 / 100));
        lv_style_set_border_color(&style_button_disabled, COLOR_WARN);
        lv_style_set_text_color(&style_button_disabled, COLOR_WARN);

        style_inited = true;
    }


    lv_obj_t * the_root = NULL;

    #if INCLIN_UI_CHECK_COMPILE_TARGET(INCLIN_UI_TARGET_ALL)
    if (inclin_ui_check_target(INCLIN_UI_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "lean_screen_#");
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_ROW);
        lv_obj_set_scrollable(lv_obj_0, false);
        lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);

        lv_obj_add_style(lv_obj_0, &style_screen, 0);
        lv_obj_add_style(lv_obj_0, &style_screen_warn, 0);
        lean_screen_style_screen_warn_ref = &style_screen_warn;
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_obj_0_style_enabled_cb, lv_obj_0, NULL);
        lv_obj_add_style(lv_obj_0, &style_screen_danger, 0);
        lean_screen_style_screen_danger_ref = &style_screen_danger;
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_obj_0_style_enabled_cb_2, lv_obj_0, NULL);
        lv_obj_add_event_cb(lv_obj_0, lean_screen_set_subject_cb, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_add_event_cb(lv_obj_0, lean_screen_set_subject_cb_2, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_1, 100);
        lv_obj_set_height(lv_obj_1, lv_pct(100));
        lv_obj_set_flex_flow(lv_obj_1, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scrollable(lv_obj_1, false);
        lv_obj_set_clickable(lv_obj_1, false);
        lv_obj_set_style_flex_main_place(lv_obj_1, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_1, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_add_style(lv_obj_1, &style_column, 0);
        lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_2, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_obj_2, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_2, LV_FLEX_FLOW_ROW);
        lv_obj_set_scrollable(lv_obj_2, false);
        lv_obj_set_clickable(lv_obj_2, false);
        lv_obj_set_style_pad_column(lv_obj_2, SPACE_XS, 0);
        lv_obj_set_style_pad_ver(lv_obj_2, SPACE_MD, 0);
        lv_obj_add_style(lv_obj_2, &style_column, 0);
        lv_obj_t * lv_scale_0 = lv_scale_create(lv_obj_2);
        lv_obj_set_width(lv_scale_0, 44);
        lv_obj_set_height(lv_scale_0, G_BAR_HEIGHT);
        lv_scale_set_mode(lv_scale_0, LV_SCALE_MODE_VERTICAL_LEFT);
        lv_scale_set_min_value(lv_scale_0, 0);
        lv_scale_set_max_value(lv_scale_0, G_FULL_SCALE);
        lv_scale_set_total_tick_count(lv_scale_0, 16);
        lv_scale_set_major_tick_every(lv_scale_0, 5);
        lv_scale_set_label_show(lv_scale_0, true);
        lv_obj_set_ext_draw_size(lv_scale_0, SPACE_MD);
        static const char *lv_scale_0_text_src_0[] = {"1.5", "1.0", "0.5", "0.0", NULL};
        lv_scale_set_text_src(lv_scale_0, lv_scale_0_text_src_0);
        lv_obj_add_style(lv_scale_0, &style_g_scale, LV_PART_ITEMS);
        lv_obj_add_style(lv_scale_0, &style_g_scale, 0);
        lv_obj_add_style(lv_scale_0, &style_g_scale_major, LV_PART_INDICATOR);

        lv_obj_t * lv_bar_0 = lv_bar_create(lv_obj_2);
        lv_obj_set_width(lv_bar_0, G_BAR_WIDTH);
        lv_obj_set_height(lv_bar_0, G_BAR_HEIGHT);
        lv_bar_set_orientation(lv_bar_0, LV_BAR_ORIENTATION_VERTICAL);
        lv_bar_set_min_value(lv_bar_0, G_FULL_SCALE);
        lv_bar_set_max_value(lv_bar_0, 0);
        lv_bar_bind_value(lv_bar_0, subject_g_decel_cg);
        lv_obj_add_style(lv_bar_0, &style_g_track, 0);
        lv_obj_add_style(lv_bar_0, &style_g_fill, LV_PART_INDICATOR);

        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_1);
        lv_subject_add_observer_obj(subject_g_decel_cg, lean_screen_lv_label_0_text_binding_cb, lv_label_0, NULL);
        lv_obj_add_style(lv_label_0, &style_value, 0);

        lv_obj_t * lv_label_1 = lv_label_create(lv_obj_1);
        lv_label_set_text(lv_label_1, "Deceleration");
        lv_obj_add_style(lv_label_1, &style_text_muted, 0);

        lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_3, LEAN_GAUGE_SIZE + 8);
        lv_obj_set_height(lv_obj_3, lv_pct(100));
        lv_obj_set_flex_flow(lv_obj_3, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scrollable(lv_obj_3, false);
        lv_obj_set_clickable(lv_obj_3, false);
        lv_obj_set_style_flex_main_place(lv_obj_3, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_3, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_add_style(lv_obj_3, &style_column, 0);
        lv_obj_t * lv_obj_4 = lv_obj_create(lv_obj_3);
        lv_obj_set_width(lv_obj_4, lv_pct(100));
        lv_obj_set_height(lv_obj_4, LEAN_GAUGE_SIZE / 2 + 6);
        lv_obj_set_scrollable(lv_obj_4, false);
        lv_obj_set_clickable(lv_obj_4, false);
        lv_obj_add_style(lv_obj_4, &style_column, 0);
        lv_obj_t * lv_arc_0 = lv_arc_create(lv_obj_4);
        lv_obj_set_width(lv_arc_0, LEAN_GAUGE_SIZE);
        lv_obj_set_height(lv_arc_0, LEAN_GAUGE_SIZE);
        lv_obj_set_align(lv_arc_0, LV_ALIGN_TOP_MID);
        lv_arc_set_rotation(lv_arc_0, 180);
        lv_arc_set_bg_start_angle(lv_arc_0, 0);
        lv_arc_set_bg_end_angle(lv_arc_0, 180);
        lv_arc_set_min_value(lv_arc_0, -LEAN_LIMIT);
        lv_arc_set_max_value(lv_arc_0, LEAN_LIMIT);
        lv_arc_set_mode(lv_arc_0, LV_ARC_MODE_SYMMETRICAL);
        lv_arc_bind_value(lv_arc_0, subject_lean_deg);
        lv_obj_set_clickable(lv_arc_0, false);
        lv_obj_add_style(lv_arc_0, &style_gauge_track, 0);
        lv_obj_add_style(lv_arc_0, &style_gauge_fill, LV_PART_INDICATOR);
        lv_obj_add_style(lv_arc_0, &style_gauge_fill_danger, LV_PART_INDICATOR);
        lean_screen_style_gauge_fill_danger_ref = &style_gauge_fill_danger;
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_arc_0_style_enabled_cb, lv_arc_0, NULL);
        lv_obj_add_style(lv_arc_0, &style_gauge_knob, LV_PART_KNOB);

        lv_obj_t * lv_label_2 = lv_label_create(lv_obj_4);
        lv_obj_set_align(lv_label_2, LV_ALIGN_BOTTOM_MID);
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_label_2_text_binding_cb, lv_label_2, NULL);
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_label_2_style_text_color_binding_cb, lv_label_2, NULL);
        lv_obj_add_style(lv_label_2, &style_lean_value, 0);

        lv_obj_t * lv_obj_5 = lv_obj_create(lv_obj_3);
        lv_obj_set_width(lv_obj_5, lv_pct(100));
        lv_obj_set_height(lv_obj_5, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_5, LV_FLEX_FLOW_ROW);
        lv_obj_set_scrollable(lv_obj_5, false);
        lv_obj_set_clickable(lv_obj_5, false);
        lv_obj_set_style_flex_main_place(lv_obj_5, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_5, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_hor(lv_obj_5, SPACE_XS, 0);
        lv_obj_add_style(lv_obj_5, &style_column, 0);
        lv_obj_t * lv_label_3 = lv_label_create(lv_obj_5);
        lv_subject_add_observer_obj(subject_lean_max_left, lean_screen_lv_label_3_text_binding_cb, lv_label_3, NULL);
        lv_obj_add_style(lv_label_3, &style_value, 0);

        lv_obj_t * lv_label_4 = lv_label_create(lv_obj_5);
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_label_4_text_binding_cb, lv_label_4, NULL);
        lv_obj_add_style(lv_label_4, &style_value, 0);

        lv_obj_t * lv_label_5 = lv_label_create(lv_obj_5);
        lv_subject_add_observer_obj(subject_lean_max_right, lean_screen_lv_label_5_text_binding_cb, lv_label_5, NULL);
        lv_obj_add_style(lv_label_5, &style_value, 0);

        lv_obj_t * lv_obj_6 = lv_obj_create(lv_obj_3);
        lv_obj_set_width(lv_obj_6, lv_pct(100));
        lv_obj_set_height(lv_obj_6, 3);
        lv_obj_set_clickable(lv_obj_6, false);
        lv_obj_add_style(lv_obj_6, &style_accent_line, 0);
        lv_obj_add_style(lv_obj_6, &style_accent_line_danger, 0);
        lean_screen_style_accent_line_danger_ref = &style_accent_line_danger;
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_obj_6_style_enabled_cb, lv_obj_6, NULL);

        lv_obj_t * lv_label_6 = lv_label_create(lv_obj_3);
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_label_6_text_binding_cb, lv_label_6, NULL);
        lv_obj_add_style(lv_label_6, &style_title, 0);
        lv_obj_add_style(lv_label_6, &style_title_danger, 0);
        lean_screen_style_title_danger_ref = &style_title_danger;
        lv_subject_add_observer_obj(subject_lean_abs_deg, lean_screen_lv_label_6_style_enabled_cb, lv_label_6, NULL);

        lv_obj_t * lv_button_0 = lv_button_create(lv_obj_3);
        lv_obj_set_height(lv_button_0, 32);
        lv_obj_set_style_margin_top(lv_button_0, SPACE_SM, 0);
        lv_subject_add_observer_obj(subject_imu_calibrating, lean_screen_lv_button_0_disabled_binding_cb, lv_button_0, NULL);
        lv_obj_add_style(lv_button_0, &style_button, 0);
        lv_obj_add_style(lv_button_0, &style_button_pressed, LV_STATE_PRESSED);
        lv_obj_add_style(lv_button_0, &style_button_disabled, LV_STATE_DISABLED);
        lv_obj_add_event_cb(lv_button_0, lean_screen_set_subject_cb_3, LV_EVENT_CLICKED, NULL);
        lv_obj_t * lv_label_7 = lv_label_create(lv_button_0);
        lv_obj_set_align(lv_label_7, LV_ALIGN_CENTER);
        lv_subject_add_observer_obj(subject_imu_calibrating, lean_screen_lv_label_7_text_binding_cb, lv_label_7, NULL);

        lv_obj_t * lv_obj_7 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_7, 100);
        lv_obj_set_height(lv_obj_7, lv_pct(100));
        lv_obj_set_flex_flow(lv_obj_7, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scrollable(lv_obj_7, false);
        lv_obj_set_clickable(lv_obj_7, false);
        lv_obj_set_style_flex_main_place(lv_obj_7, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_7, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_add_style(lv_obj_7, &style_column, 0);
        lv_obj_t * lv_obj_8 = lv_obj_create(lv_obj_7);
        lv_obj_set_width(lv_obj_8, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_obj_8, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_8, LV_FLEX_FLOW_ROW);
        lv_obj_set_scrollable(lv_obj_8, false);
        lv_obj_set_clickable(lv_obj_8, false);
        lv_obj_set_style_pad_column(lv_obj_8, SPACE_XS, 0);
        lv_obj_set_style_pad_ver(lv_obj_8, SPACE_MD, 0);
        lv_obj_add_style(lv_obj_8, &style_column, 0);
        lv_obj_t * lv_bar_1 = lv_bar_create(lv_obj_8);
        lv_obj_set_width(lv_bar_1, G_BAR_WIDTH);
        lv_obj_set_height(lv_bar_1, G_BAR_HEIGHT);
        lv_bar_set_orientation(lv_bar_1, LV_BAR_ORIENTATION_VERTICAL);
        lv_bar_set_min_value(lv_bar_1, 0);
        lv_bar_set_max_value(lv_bar_1, G_FULL_SCALE);
        lv_bar_bind_value(lv_bar_1, subject_g_accel_cg);
        lv_obj_add_style(lv_bar_1, &style_g_track, 0);
        lv_obj_add_style(lv_bar_1, &style_g_fill, LV_PART_INDICATOR);

        lv_obj_t * lv_scale_1 = lv_scale_create(lv_obj_8);
        lv_obj_set_width(lv_scale_1, 44);
        lv_obj_set_height(lv_scale_1, G_BAR_HEIGHT);
        lv_scale_set_mode(lv_scale_1, LV_SCALE_MODE_VERTICAL_RIGHT);
        lv_scale_set_min_value(lv_scale_1, 0);
        lv_scale_set_max_value(lv_scale_1, G_FULL_SCALE);
        lv_scale_set_total_tick_count(lv_scale_1, 16);
        lv_scale_set_major_tick_every(lv_scale_1, 5);
        lv_scale_set_label_show(lv_scale_1, true);
        lv_obj_set_ext_draw_size(lv_scale_1, SPACE_MD);
        static const char *lv_scale_1_text_src_0[] = {"0.0", "0.5", "1.0", "1.5", NULL};
        lv_scale_set_text_src(lv_scale_1, lv_scale_1_text_src_0);
        lv_obj_add_style(lv_scale_1, &style_g_scale, LV_PART_ITEMS);
        lv_obj_add_style(lv_scale_1, &style_g_scale, 0);
        lv_obj_add_style(lv_scale_1, &style_g_scale_major, LV_PART_INDICATOR);

        lv_obj_t * lv_label_8 = lv_label_create(lv_obj_7);
        lv_subject_add_observer_obj(subject_g_accel_cg, lean_screen_lv_label_8_text_binding_cb, lv_label_8, NULL);
        lv_obj_add_style(lv_label_8, &style_value, 0);

        lv_obj_t * lv_label_9 = lv_label_create(lv_obj_7);
        lv_label_set_text(lv_label_9, "Acceleration");
        lv_obj_add_style(lv_label_9, &style_text_muted, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lean_screen_lv_obj_0_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_style_enabled(obj, lean_screen_style_screen_warn_ref, 0, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_WARN && lv_subject_get_int(subject_lean_abs_deg) < LEAN_DANGER);
}

static void lean_screen_lv_obj_0_style_enabled_cb_2(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_style_enabled(obj, lean_screen_style_screen_danger_ref, 0, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_DANGER);
}

static void lean_screen_set_subject_cb(lv_event_t * e)
{
    LV_UNUSED(e);
    lv_subject_set_int(subject_lean_max_left, 0);
}

static void lean_screen_set_subject_cb_2(lv_event_t * e)
{
    LV_UNUSED(e);
    lv_subject_set_int(subject_lean_max_right, 0);
}

static void lean_screen_lv_label_0_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    char eval_buf_1[LV_XML_EVAL_STRING_BUF_SIZE];
    lv_snprintf(eval_buf_1, sizeof(eval_buf_1), "%d%s%d%s", (int)(lv_subject_get_int(subject_g_decel_cg) / 100), ".", (int)(lv_subject_get_int(subject_g_decel_cg) % 100 / 10), " G");
    lv_label_set_text(obj, eval_buf_1);
}

static void lean_screen_lv_arc_0_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_style_enabled(obj, lean_screen_style_gauge_fill_danger_ref, LV_PART_INDICATOR, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_DANGER);
}

static void lean_screen_lv_label_2_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    char eval_buf_1[LV_XML_EVAL_STRING_BUF_SIZE];
    lv_snprintf(eval_buf_1, sizeof(eval_buf_1), "%d%s", (int)(lv_subject_get_int(subject_lean_abs_deg)), "°");
    lv_label_set_text(obj, eval_buf_1);
}

static void lean_screen_lv_label_2_style_text_color_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_style_text_color(obj, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_DANGER ? COLOR_DANGER : lv_subject_get_int(subject_lean_abs_deg) >= LEAN_WARN ? COLOR_WARN : COLOR_TEXT, 0);
}

static void lean_screen_lv_label_3_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    char eval_buf_1[LV_XML_EVAL_STRING_BUF_SIZE];
    lv_snprintf(eval_buf_1, sizeof(eval_buf_1), "%d%s", (int)(lv_subject_get_int(subject_lean_max_left)), "°");
    lv_label_set_text(obj, lv_subject_get_int(subject_lean_max_left) == 0 ? "--°" : eval_buf_1);
}

static void lean_screen_lv_label_4_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    char eval_buf_1[LV_XML_EVAL_STRING_BUF_SIZE];
    lv_snprintf(eval_buf_1, sizeof(eval_buf_1), "%d%s", (int)(lv_subject_get_int(subject_lean_abs_deg)), "°");
    lv_label_set_text(obj, eval_buf_1);
}

static void lean_screen_lv_label_5_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    char eval_buf_1[LV_XML_EVAL_STRING_BUF_SIZE];
    lv_snprintf(eval_buf_1, sizeof(eval_buf_1), "%d%s", (int)(lv_subject_get_int(subject_lean_max_right)), "°");
    lv_label_set_text(obj, lv_subject_get_int(subject_lean_max_right) == 0 ? "--°" : eval_buf_1);
}

static void lean_screen_lv_obj_6_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_style_enabled(obj, lean_screen_style_accent_line_danger_ref, 0, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_DANGER);
}

static void lean_screen_lv_label_6_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_label_set_text(obj, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_DANGER ? "AGGRESSIVE LEAN!" : "Lean angle");
}

static void lean_screen_lv_label_6_style_enabled_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_style_enabled(obj, lean_screen_style_title_danger_ref, 0, lv_subject_get_int(subject_lean_abs_deg) >= LEAN_DANGER);
}

static void lean_screen_lv_button_0_disabled_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_obj_set_disabled(obj, lv_subject_get_int(subject_imu_calibrating) == 1);
}

static void lean_screen_set_subject_cb_3(lv_event_t * e)
{
    LV_UNUSED(e);
    lv_subject_set_int(subject_imu_cal_request, 1);
}

static void lean_screen_lv_label_7_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    lv_label_set_text(obj, lv_subject_get_int(subject_imu_calibrating) == 1 ? "HOLD STILL" : "RESET GYRO");
}

static void lean_screen_lv_label_8_text_binding_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_obj_t * obj = lv_observer_get_target_obj(observer);
    char eval_buf_1[LV_XML_EVAL_STRING_BUF_SIZE];
    lv_snprintf(eval_buf_1, sizeof(eval_buf_1), "%d%s%d%s", (int)(lv_subject_get_int(subject_g_accel_cg) / 100), ".", (int)(lv_subject_get_int(subject_g_accel_cg) % 100 / 10), " G");
    lv_label_set_text(obj, eval_buf_1);
}


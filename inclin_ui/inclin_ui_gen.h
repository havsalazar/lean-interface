/**
 * @file inclin_ui_gen.h
 */

#ifndef LVGL_PRO_INCLIN_UI_GEN_H
#define LVGL_PRO_INCLIN_UI_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/* Prototypes for target functions, needed by responsive const definitions */

void inclin_ui_set_target(uint32_t target);
uint32_t inclin_ui_get_target(void);
bool inclin_ui_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define INCLIN_UI_TARGET_UNDEFINED  (0 << 1)
#define INCLIN_UI_TARGET_TARGET1    (1 << 1)
#define INCLIN_UI_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef INCLIN_UI_COMPILE_TARGET
#define INCLIN_UI_COMPILE_TARGET INCLIN_UI_TARGET_ALL
#endif

#define INCLIN_UI_CHECK_COMPILE_TARGET(target) (INCLIN_UI_COMPILE_TARGET & (target) ? 1 : 0)

#define COLOR_BG_TOP (lv_color_hex(0x1c2335))
#define COLOR_BG_BOTTOM (lv_color_hex(0x0a1c9c))
#define COLOR_BG_WARN_TOP (lv_color_hex(0x2a2210))
#define COLOR_BG_WARN_BOTTOM (lv_color_hex(0x7a4a00))
#define COLOR_BG_DANGER_TOP (lv_color_hex(0x2e1012))
#define COLOR_BG_DANGER_BOTTOM (lv_color_hex(0x8c1010))
#define COLOR_TRACK (lv_color_hex(0x2c3656))
#define COLOR_GAUGE (lv_color_hex(0xb4c2ff))
#define COLOR_ACCENT (lv_color_hex(0x2f6bff))
#define COLOR_ACCENT_TEXT (lv_color_hex(0x5c8cff))
#define COLOR_TEXT (lv_color_hex(0xf2f4f8))
#define COLOR_TEXT_MUTED (lv_color_hex(0xc0c8e0))
#define COLOR_WARN (lv_color_hex(0xffb020))
#define COLOR_DANGER (lv_color_hex(0xff5a50))
#define SPACE_XS (4)
#define SPACE_SM (8)
#define SPACE_MD (12)
#define LEAN_LIMIT (60)
#define LEAN_GAUGE_SIZE (236)
#define LEAN_GAUGE_WIDTH (20)
#define LEAN_WARN (30)
#define LEAN_DANGER (45)
#define G_FULL_SCALE (150)
#define G_BAR_WIDTH (30)
#define G_BAR_HEIGHT (190)


#ifndef LV_XML_EVAL_STRING_BUF_SIZE
    #define LV_XML_EVAL_STRING_BUF_SIZE 256
#endif

/**
 * Animation path of a style transition with a `bezier(…)` easing. The control
 * points come from the transition descriptor's `user_data`, as a transition
 * descriptor cannot store animation parameters itself.
 */
static inline int32_t inclin_ui_style_transition_bezier_path_helper(const lv_anim_t * a)
{
    const lv_anim_bezier3_para_t * para = (const lv_anim_bezier3_para_t *)lv_anim_get_user_data(a);
    if(para == NULL) return lv_anim_path_linear(a);

    lv_anim_t a_tmp = *a;
    lv_anim_set_bezier3_param(&a_tmp, para->x1, para->y1, para->x2, para->y2);
    return lv_anim_path_custom_bezier3(&a_tmp);
}

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t style_transparent;
extern lv_style_t style_text_muted;
extern lv_style_t style_value;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_lean;
extern lv_font_t * font_title;
extern lv_font_t * font_small;


/*----------------
 * Images
 *----------------*/



/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t * subject_lean_deg;
extern lv_subject_t * subject_lean_abs_deg;
extern lv_subject_t * subject_accel_cg;
extern lv_subject_t * subject_g_decel_cg;
extern lv_subject_t * subject_g_accel_cg;
extern lv_subject_t * subject_lean_max_left;
extern lv_subject_t * subject_lean_max_right;
extern lv_subject_t * subject_imu_cal_request;
extern lv_subject_t * subject_imu_calibrating;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/*----------------
 * Easing Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void inclin_ui_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "screens/lean_screen_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_INCLIN_UI_GEN_H*/
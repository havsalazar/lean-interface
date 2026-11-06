/**
 * @file inclin_ui_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "inclin_ui.h"

#if defined(LV_USE_XML) && LV_USE_XML
#endif /* LV_USE_XML */

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void check_font(lv_font_t ** font, const char * name);

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t inclin_ui_target = INCLIN_UI_COMPILE_TARGET;

/*----------------
 * Translations
 *----------------*/

#ifndef LV_EDITOR_PREVIEW
    static const char * translation_languages[] = {"en", "de", NULL};
    static const char * translation_tags[] = {"dog", "cat", "house", NULL};
    static const char * translation_texts[] = {
        "This is a dog", "Das ist ein Hund", /* dog */
        "A curious little cat", "Eine neugierige kleine Katze", /* cat */
        "The house is cozy and warm", "Das Haus ist gemütlich und warm", /* house */
    };
#endif

/**********************
 *  GLOBAL VARIABLES
 **********************/

/*--------------------
 *  Permanent screens
 *-------------------*/

/*----------------
 * Fonts
 *----------------*/

lv_font_t * font_lean;
extern lv_font_t font_lean_data;
lv_font_t * font_title;
extern lv_font_t font_title_data;
lv_font_t * font_small;
extern lv_font_t font_small_data;

/*----------------
 * Images
 *----------------*/



/*----------------
 * Global styles
 *----------------*/

lv_style_t style_transparent;
lv_style_t style_text_muted;
lv_style_t style_value;

/*----------------
 * Subjects
 *----------------*/

lv_subject_t * subject_lean_deg;
lv_subject_t * subject_lean_abs_deg;
lv_subject_t * subject_accel_cg;
lv_subject_t * subject_g_decel_cg;
lv_subject_t * subject_g_accel_cg;
lv_subject_t * subject_lean_max_left;
lv_subject_t * subject_lean_max_right;
lv_subject_t * subject_imu_cal_request;
lv_subject_t * subject_imu_calibrating;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void inclin_ui_init_gen(const char * asset_path)
{
    /* When running from the editor the theme set from the XML should overwrite this */
#if !defined(LV_EDITOR_PREVIEW)
#if LV_USE_THEME_SIMPLE
    lv_display_t * disp = lv_display_get_default();
    lv_theme_t * th = lv_theme_simple_init(disp);
    lv_display_set_theme(disp, th);
#else
    LV_LOG_WARN("Simple theme is selected in project.xml but LV_USE_THEME_SIMPLE is disabled");
#endif
#endif /*LV_EDITOR_PREVIEW*/


    /*----------------
     * Fonts
     *----------------*/

    /* Targets: any */

    #if INCLIN_UI_CHECK_COMPILE_TARGET(INCLIN_UI_TARGET_ALL)
    if (inclin_ui_check_target(INCLIN_UI_TARGET_ALL)) {
        if (!font_lean) {
            /* font_lean */
            /* get font 'font_lean' from a C array */
            font_lean = &font_lean_data;

        }
        if (!font_title) {
            /* font_title */
            /* get font 'font_title' from a C array */
            font_title = &font_title_data;

        }
        if (!font_small) {
            /* font_small */
            /* get font 'font_small' from a C array */
            font_small = &font_small_data;

        }
    }
    #endif

    /*----------------
     * Images
     *----------------*/



    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&style_transparent);
        lv_style_init(&style_text_muted);
        lv_style_init(&style_value);

        lv_style_set_bg_opa(&style_transparent, 0);
        lv_style_set_border_width(&style_transparent, 0);
        lv_style_set_pad_all(&style_transparent, 0);
        lv_style_set_radius(&style_transparent, 0);
        lv_style_set_text_color(&style_text_muted, COLOR_TEXT_MUTED);
        lv_style_set_text_font(&style_text_muted, font_small);
        lv_style_set_text_color(&style_value, COLOR_TEXT);
        lv_style_set_text_font(&style_value, font_title);

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    subject_lean_deg = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_lean_deg, -90);
    lv_subject_set_max_value_int(subject_lean_deg, 90);
    lv_subject_set_int(subject_lean_deg, 0);
    subject_lean_abs_deg = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_lean_abs_deg, 0);
    lv_subject_set_max_value_int(subject_lean_abs_deg, 90);
    lv_subject_set_int(subject_lean_abs_deg, 0);
    subject_accel_cg = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_accel_cg, -300);
    lv_subject_set_max_value_int(subject_accel_cg, 300);
    lv_subject_set_int(subject_accel_cg, 0);
    subject_g_decel_cg = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_g_decel_cg, 0);
    lv_subject_set_max_value_int(subject_g_decel_cg, 300);
    lv_subject_set_int(subject_g_decel_cg, 0);
    subject_g_accel_cg = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_g_accel_cg, 0);
    lv_subject_set_max_value_int(subject_g_accel_cg, 300);
    lv_subject_set_int(subject_g_accel_cg, 0);
    subject_lean_max_left = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_lean_max_left, 0);
    lv_subject_set_max_value_int(subject_lean_max_left, 90);
    lv_subject_set_int(subject_lean_max_left, 0);
    subject_lean_max_right = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_lean_max_right, 0);
    lv_subject_set_max_value_int(subject_lean_max_right, 90);
    lv_subject_set_int(subject_lean_max_right, 0);
    subject_imu_cal_request = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_imu_cal_request, 0);
    lv_subject_set_max_value_int(subject_imu_cal_request, 1);
    lv_subject_set_int(subject_imu_cal_request, 0);
    subject_imu_calibrating = lv_subject_create(LV_SUBJECT_TYPE_INT);
    lv_subject_set_min_value_int(subject_imu_calibrating, 0);
    lv_subject_set_max_value_int(subject_imu_calibrating, 1);
    lv_subject_set_int(subject_imu_calibrating, 0);

    /*----------------
     * Translations
     *----------------*/

    #ifndef LV_EDITOR_PREVIEW
        lv_translation_add_static(translation_languages, translation_tags, translation_texts);
        lv_translation_set_language(translation_languages[0]);
    #endif

#if defined(LV_USE_XML) && LV_USE_XML
    /* Register widgets */

    /* Check all fonts / default if needed. This prevents fonts that are used in one target but
       defined in another from causing assertion failures during rendering of the Preview. */
    check_font(&font_lean, "font_lean");
    check_font(&font_title, "font_title");
    check_font(&font_small, "font_small");

    /* Register fonts */
    lv_xml_register_font(NULL, "font_lean", font_lean);
    lv_xml_register_font(NULL, "font_title", font_title);
    lv_xml_register_font(NULL, "font_small", font_small);

    /* Register subjects */
    lv_xml_register_subject(NULL, "subject_lean_deg", subject_lean_deg);
    lv_xml_register_subject(NULL, "subject_lean_abs_deg", subject_lean_abs_deg);
    lv_xml_register_subject(NULL, "subject_accel_cg", subject_accel_cg);
    lv_xml_register_subject(NULL, "subject_g_decel_cg", subject_g_decel_cg);
    lv_xml_register_subject(NULL, "subject_g_accel_cg", subject_g_accel_cg);
    lv_xml_register_subject(NULL, "subject_lean_max_left", subject_lean_max_left);
    lv_xml_register_subject(NULL, "subject_lean_max_right", subject_lean_max_right);
    lv_xml_register_subject(NULL, "subject_imu_cal_request", subject_imu_cal_request);
    lv_xml_register_subject(NULL, "subject_imu_calibrating", subject_imu_calibrating);

    /* Register callbacks */

    /* Register easing callbacks */
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if defined(LV_USE_XML) && LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
#endif

#if !defined(LV_USE_XML) || LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manually from XML using lv_xml_create() */
#endif
}

void inclin_ui_set_target(uint32_t target)
{
    inclin_ui_target = target;
}

uint32_t inclin_ui_get_target(void)
{
    return inclin_ui_target;
}

bool inclin_ui_check_target(uint32_t target)
{
    return (inclin_ui_target & target) ? true : false;
}

/* Callbacks */

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void check_font(lv_font_t ** font, const char * name)
{
    if (!(*font)) {
        *font = (lv_font_t *)LV_FONT_DEFAULT;
        LV_LOG_WARN("font `%s` was not set. Using `LV_FONT_DEFAULT` instead", name);
    }
}
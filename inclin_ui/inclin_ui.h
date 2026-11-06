/**
 * @file inclin_ui.h
 */

#ifndef LVGL_PRO_INCLIN_UI_H
#define LVGL_PRO_INCLIN_UI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      DEFINES
 *********************/

/* Target(s) select at startup: one or more INCLIN_UI_TARGET_* values OR-ed together. */
#ifndef INCLIN_UI_COMPILE_TARGET
#define INCLIN_UI_COMPILE_TARGET INCLIN_UI_TARGET_ALL
#endif

/*********************
 *      INCLUDES
 *********************/

#include "inclin_ui_gen.h"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the component library
 */
void inclin_ui_init(const char * asset_path);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_INCLIN_UI_H*/
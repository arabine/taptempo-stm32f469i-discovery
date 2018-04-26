/**
 * This file has a different license to the rest of the uGFX system.
 * You can copy, modify and distribute this file as you see fit.
 * You do not need to publish your source modifications to this file.
 * The only thing you are not permitted to do is to relicense it
 * under a different license.
 */

/**
 * Copy this file into your project directory and rename it as gfxconf.h
 * Edit your copy to turn on the uGFX features you want to use.
 * The values below are the defaults.
 *
 * Only remove the comments from lines where you want to change the
 * default value. This allows definitions to be included from
 * driver makefiles when required and provides the best future
 * compatibility for your project.
 *
 * Please use spaces instead of tabs in this file.
 */

#ifndef _GFXCONF_H
#define _GFXCONF_H


///////////////////////////////////////////////////////////////////////////
// GOS - One of these must be defined, preferably in your Makefile       //
///////////////////////////////////////////////////////////////////////////
//#define GFX_USE_OS_CHIBIOS                           FALSE
//#define GFX_USE_OS_FREERTOS                          FALSE
//    #define GFX_FREERTOS_USE_TRACE                   FALSE
//#define GFX_USE_OS_WIN32                             FALSE
//#define GFX_USE_OS_LINUX                             FALSE
//#define GFX_USE_OS_OSX                               FALSE
//#define GFX_USE_OS_ECOS                              FALSE
//#define GFX_USE_OS_RAWRTOS                           FALSE
//#define GFX_USE_OS_ARDUINO                           FALSE
//#define GFX_USE_OS_KEIL                              FALSE
//#define GFX_USE_OS_RTX5                              FALSE
//#define GFX_USE_OS_CMSIS                             FALSE
//#define GFX_USE_OS_CMSIS2                            FALSE
#define GFX_USE_OS_RAW32                             TRUE
//#define GFX_USE_OS_ZEPHYR                            FALSE
//#define GFX_USE_OS_NIOS                              FALSE
//#define GFX_USE_OS_QT                                FALSE
//    #define INTERRUPTS_OFF()                         optional_code
//    #define INTERRUPTS_ON()                          optional_code

// Options that (should where relevant) apply to all operating systems
//    #define GFX_NO_INLINE                            FALSE
//    #define GFX_COMPILER                             GFX_COMPILER_UNKNOWN
//    #define GFX_SHOW_COMPILER                        FALSE
//    #define GFX_CPU                                  GFX_CPU_UNKNOWN
//    #define GFX_CPU_NO_ALIGNMENT_FAULTS              FALSE
//    #define GFX_CPU_ENDIAN                           GFX_CPU_ENDIAN_UNKNOWN
    #define GFX_OS_HEAP_SIZE                         40960
//    #define GFX_OS_NO_INIT                           FALSE
//    #define GFX_OS_INIT_NO_WARNING                   FALSE
    #define GFX_OS_PRE_INIT_FUNCTION                 Raw32OSInit
//    #define GFX_OS_EXTRA_INIT_FUNCTION               myOSInitRoutine
//    #define GFX_OS_EXTRA_DEINIT_FUNCTION             myOSDeInitRoutine
//    #define GFX_OS_CALL_UGFXMAIN                     FALSE
//    #define GFX_OS_UGFXMAIN_STACKSIZE                0
//    #define GFX_EMULATE_MALLOC                       FALSE


/* GFX sub-systems to turn on */
#define GFX_USE_GDISP			TRUE
#define GFX_USE_GWIN			TRUE
#define GFX_USE_GINPUT			TRUE
#define GFX_USE_GEVENT			TRUE
#define GFX_USE_GTIMER			TRUE

/* Features for the GDISP sub-system. */
#define GDISP_NEED_VALIDATION	TRUE
#define GDISP_NEED_CLIP			TRUE
#define GDISP_NEED_CIRCLE		TRUE
#define GDISP_NEED_TEXT			TRUE
#define GDISP_NEED_CONTROL		TRUE
#define GDISP_NEED_IMAGE		                TRUE
#define GDISP_NEED_IMAGE_BMP		            TRUE

/* GDISP fonts to include */
#define GDISP_INCLUDE_FONT_UI2	TRUE
#define GDISP_INCLUDE_FONT_LARGENUMBERS          FALSE

/* Features for the GWIN subsystem. */
#define GWIN_NEED_WINDOWMANAGER	TRUE
#define GWIN_NEED_WIDGET		TRUE
#define GWIN_NEED_BUTTON		TRUE
#define GWIN_NEED_LABEL         TRUE

/* Features for the GINPUT subsystem. */
#define GINPUT_NEED_MOUSE		TRUE
#define GINPUT_TOUCH_NOCALIBRATE FALSE
#define GINPUT_TOUCH_NOCALIBRATE_GUI FALSE

#define GDISP_NEED_MULTITHREAD	TRUE
#define GFX_USE_GQUEUE	TRUE
#define GQUEUE_NEED_ASYNC	TRUE

#define GFX_USE_GFILE                           TRUE
#define GFILE_NEED_ROMFS                        TRUE
#define GFILE_MAX_GFILES                         24

#endif /* _GFXCONF_H */

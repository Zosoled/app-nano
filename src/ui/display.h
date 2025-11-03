#pragma once

#include <stdbool.h>  // bool

#define ICON_CONCAT(a, b, c, d) a##b##c##d
#define ICON_APP(a, b) ICON_CONCAT(C_app_, a, _, b)

#if defined(TARGET_NANOX) || defined(TARGET_NANOS2)
#define ICON_APP_BOILERPLATE ICON_APP(COIN, 14px)
#define ICON_APP_HOME        C_home_boilerplate_14px
#define ICON_APP_WARNING     C_icon_warning
#elif defined(TARGET_STAX) || defined(TARGET_FLEX)
#define ICON_APP_BOILERPLATE ICON_APP(COIN, 64px)
#define ICON_APP_HOME        ICON_APP_BOILERPLATE
#define ICON_APP_WARNING     C_Warning_64px
#elif defined(TARGET_APEX_P)
#define ICON_APP_BOILERPLATE ICON_APP(COIN, 48px)
#define ICON_APP_HOME        ICON_APP_BOILERPLATE
#define ICON_APP_WARNING     LARGE_WARNING_ICON
#endif

/**
 * Callback to reuse action with approve/reject in step FLOW.
 */
typedef void (*action_validate_cb)(bool);

/**
 * Display address on the device and ask confirmation to export.
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_address(void);

/**
 * Display transaction information on the device and ask confirmation to sign.
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_transaction(void);

/**
 * Display blind-sign transaction information on the device and ask confirmation to sign.
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_blind_signed_transaction(void);

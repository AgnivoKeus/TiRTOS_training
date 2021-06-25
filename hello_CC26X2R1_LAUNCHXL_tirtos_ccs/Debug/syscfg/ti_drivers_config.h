/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the CC26X2R1_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC26X2R1_LAUNCHXL
#ifndef DeviceFamily_CC26X2
#define DeviceFamily_CC26X2
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== CCFG ========
 */


/*
 *  ======== GPIO ========
 */

/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t              CONFIG_GPIO_0_CONST;
#define CONFIG_GPIO_0                   0
#define CONFIG_TI_DRIVERS_GPIO_COUNT    1

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== PIN ========
 */
#include <ti/drivers/PIN.h>

extern const PIN_Config BoardGpioInitTable[];

/* XDS110 UART, Parent Signal: CONFIG_UART_0 TX, (DIO3) */
#define CONFIG_PIN_1                   0x00000003
/* XDS110 UART, Parent Signal: CONFIG_UART_0 RX, (DIO2) */
#define CONFIG_PIN_2                   0x00000002
/* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_0 GPIO Pin, (DIO13) */
#define CONFIG_PIN_3                   0x0000000d
/* LaunchPad LED Red, Parent Signal: CONFIG_GPTIMER_0 PWM Pin, (DIO6) */
#define CONFIG_PIN_0                   0x00000006
#define CONFIG_TI_DRIVERS_PIN_COUNT    4


/*
 *  ======== PWM ========
 */

/* DIO6, LaunchPad LED Red */
extern const uint_least8_t              LED0_PWM_CONST;
#define LED0_PWM                        0
#define CONFIG_TI_DRIVERS_PWM_COUNT     1


/*
 *  ======== Timer ========
 */

extern const uint_least8_t                  timer0_CONST;
#define timer0                              0
#define CONFIG_TI_DRIVERS_TIMER_COUNT       1


/*
 *  ======== UART ========
 */

/*
 *  TX: DIO3
 *  RX: DIO2
 *  XDS110 UART
 */
extern const uint_least8_t              CONFIG_UART_0_CONST;
#define CONFIG_UART_0                   0
#define CONFIG_TI_DRIVERS_UART_COUNT    1


/*
 *  ======== Button ========
 */

/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t                  BUTTON0_CONST;
#define BUTTON0                             0
#define CONFIG_TI_DRIVERS_BUTTON_COUNT      1


/*
 *  ======== LED ========
 */

/* DIO6, LaunchPad LED Red */
extern const uint_least8_t              LED0_CONST;
#define LED0                            0
#define CONFIG_TI_DRIVERS_LED_COUNT     1


/*
 *  ======== GPTimer ========
 */

extern const uint_least8_t                  CONFIG_GPTIMER_1_CONST;
#define CONFIG_GPTIMER_1                    0
extern const uint_least8_t                  CONFIG_GPTIMER_0_CONST;
#define CONFIG_GPTIMER_0                    1
#define CONFIG_TI_DRIVERS_GPTIMER_COUNT     2


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */

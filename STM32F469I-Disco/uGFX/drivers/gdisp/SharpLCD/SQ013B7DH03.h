#ifndef SQ013B7DH03
#define SQ013B7DH03

/* SiLabs display configuration file (for the happy gecko). */

/* Command to clear the screen. */
#define CLEAR_SCREEN_CMD    0x04
/* Command to write a line to the LCD. */
#define WRITE_LINE_CMD      0x01

/* LCD and SPI GPIO pin connections on the STK. */
#define LCD_PORT_SCLK             (4)  /* = gpioPortE */
#define LCD_PIN_SCLK             (12)
#define LCD_PORT_SI               (4)  /* = gpioPortE */
#define LCD_PIN_SI               (10)
#define LCD_PORT_SCS              (0)  /* = gpioPortA */
#define LCD_PIN_SCS              (10)
#define LCD_PORT_EXTCOMIN         (5)  /* = gpioPortF */
#define LCD_PIN_EXTCOMIN          (3)
#define LCD_PORT_DISP_SEL         (0)  /* = gpioPortA */
#define LCD_PIN_DISP_SEL          (8)

/* PRS settings for polarity inversion extcomin auto toggle.  */
#define LCD_AUTO_TOGGLE_PRS_CH    (0)  /* PRS channel 0.      */
#define LCD_AUTO_TOGGLE_PRS_ROUTE_LOC   PRS_ROUTE_LOCATION_LOC1
#define LCD_AUTO_TOGGLE_PRS_ROUTE_PEN   PRS_ROUTE_CH0PEN

/*
 * Select how LCD polarity inversion should be handled:
 *
 * If POLARITY_INVERSION_EXTCOMIN is defined,
 * the polarity inversion is armed for every rising edge of the EXTCOMIN
 * pin. The actual polarity inversion is triggered at the next transision of
 * SCS. This mode is recommended because it causes less CPU and SPI load than
 * the alternative mode, see below.
 * If POLARITY_INVERSION_EXTCOMIN is undefined,
 * the polarity inversion is toggled by sending an SPI command. This mode
 * causes more CPU and SPI load than using the EXTCOMIN pin mode.
 */
#define POLARITY_INVERSION_EXTCOMIN

/* Define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE if you want the PAL
 * (Platform Abstraction Layer interface) to automatically toggle the EXTCOMIN
 *  pin.
 * If the PAL_TIMER_REPEAT function is defined the EXTCOMIN toggling is handled
 * by a timer repeat system, therefore we must undefine
 * POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE;
 */
#ifndef PAL_TIMER_REPEAT_FUNCTION
  #define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE
#endif

/* Frequency of LCD polarity inversion. */
#define LS013B7DH03_POLARITY_INVERSION_FREQUENCY (64)

/*
 * Select which oscillator should source the RTC clock.
 */
#undef  PAL_RTC_CLOCK_LFXO
#undef  PAL_RTC_CLOCK_LFRCO
#define PAL_RTC_CLOCK_ULFRCO

/*
 * PAL SPI / USART configuration for the EFM32ZG_STK3200.
 * Select which USART and location is connected to the device via SPI.
 */
#define PAL_SPI_USART_UNIT        (USART0)
#define PAL_SPI_USART_CLOCK       (cmuClock_USART0)
#define PAL_SPI_USART_LOCATION    (USART_ROUTE_LOCATION_LOC0)
#define PAL_SPI_USART_DMAREQ_TXBL (DMAREQ_USART0_TXBL)

/*
 * Specify the SPI baud rate:
 */
#define PAL_SPI_BAUDRATE       (3500000) /* Max baudrate ????. */

#define POLARITY_INVERSION_FREQ     60
#define DISPLAY_TOGGLE_FREQ         2
#define DISPLAY_COMMAND_CLEAR       0x4
#define DMA_CHANNEL                 0
#define DMA_MAX_UNIT_TRANSFERS      1024
#define TRANSFERS_PER_FRAME         3

#endif

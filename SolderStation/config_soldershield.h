#ifndef _CONFIG_SOLDERSHIELD
#define _CONFIG_SOLDERSHIELD

// Temperature step number
#define TEMP_STEP 1

// Iron pins
#define PIN_PWM_OUT 3
#define PIN_PROBE_IN A1

// Controls
#define BUTTON_UP_PIN 12
#define BUTTON_UP_PULL 1
#define BUTTON_UP_INVERTED 1

#define BUTTON_DOWN_PIN 13
#define BUTTON_DOWN_PULL 1
#define BUTTON_DOWN_INVERTED 1

#define BUTTON_STANDBY
#define BUTTON_STANDBY_PIN A2
#define BUTTON_STANDBY_PULL 1
#define BUTTON_STANDBY_INVERTED 1

// Version Display
#define SCREEN_SEG7
//#define SCREEN_LCD

#if defined(SCREEN_SEG7) && defined(SCREEN_LCD)
#error CAN'T USE SEG7 & LCD IN SAME TIME
#endif

// Display SEG7
#ifdef SCREEN_SEG7
// SEG7 Module
#define SEG7_MODULE
#define SEG7_A_PIN 8
#define SEG7_B_PIN 7
#define SEG7_C_PIN 5
#define SEG7_D_PIN A4
#define SEG7_E_PIN A5
#define SEG7_F_PIN 6
#define SEG7_G_PIN 4
#define SEG7_DP_PIN 2
#define SEG7_SEL0_PIN 11
#define SEG7_SEL1_PIN 10
#define SEG7_SEL2_PIN 9
#endif

// Display LCD
#ifdef SCREEN_LCD
// Menu module
#define MENU_MODULE

// LCD Module
#define LCD_MODULE
#define LCD_SHOW_HEAT
#define LCD_ST7735_MODULE
#define LCD_ST7735_CS 10
#define LCD_ST7735_DC 9
#define LCD_ST7735_RESET 8
#define LCD_ST7735_BL 2
#endif

// LED Module
#define LED_MODULE
#define LED_HEAT A3
#define LED_HEAT_INVERTED 0

// Serial Module
#define SERIAL_MODULE
#define SERIAL_BAUD 115200

// Override UI loop -> LED
#undef DELAY_UI_LOOP
#define DELAY_UI_LOOP 5

#define HEAT_PROTECTION
#define MEMORY_SETTINGS

#endif //_CONFIG_SOLDERSHIELD

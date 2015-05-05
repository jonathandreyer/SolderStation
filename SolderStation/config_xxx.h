#ifndef _CONFIG_XXX_H
#define _CONFIG_XXX_H

#ifdef CONFIG_NAME
#error Configuration already defined
#endif
#define CONFIG_NAME XXX

// Iron pins
#define PIN_PWM_OUT 3
#define PIN_PROBE_IN A0

// Controls
#define BUTTON_EXTENDED
#define BUTTON_UP_PIN 7
#define BUTTON_UP_PULL 1
#define BUTTON_UP_INVERTED 1

#define BUTTON_DOWN_PIN 6
#define BUTTON_DOWN_PULL 1
#define BUTTON_DOWN_INVERTED 1

#define BUTTON_SELECT_PIN 5
#define BUTTON_SELECT_PULL 1
#define BUTTON_SELECT_INVERTED 1

#define BUTTON_BACK_PIN 4
#define BUTTON_BACK_PULL 1
#define BUTTON_BACK_INVERTED 1

#define BUTTON_STANDBY
#define BUTTON_STANDBY_PIN 16
#define BUTTON_STANDBY_PULL 1
#define BUTTON_STANDBY_INVERTED 1

// Menu module
#define MENU_MODULE

// LCD Module
#define LCD_MODULE
#define LCD_SHOW_HEAT
#define LCD_PCD8544_MODULE
#define LCD_PCD8544_CS 10
#define LCD_PCD8544_DC 9
#define LCD_PCD8544_RESET 8
#define LCD_PCD8544_BL 2

// LED Module
#define LED_MODULE
#define LED_HEAT A3
#define LED_HEAT_INVERTED 0

// Serial Module
#define SERIAL_MODULE
#define SERIAL_BAUD 115200

#define HEAT_PROTECTION

#endif //_CONFIG_XXX_H

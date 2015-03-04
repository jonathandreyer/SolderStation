#include "config.h"
#include "controls.h"
#include "solder_station.h"
#include "utils.h"

#ifndef BUTTON_UP_PIN
#error Missing configuration of BUTTON_UP_PIN
#endif //BUTTON_UP_PIN
#ifndef BUTTON_UP_PULL
#error Missing configuration of BUTTON_UP_PULL
#endif //BUTTON_UP_PULL
#ifndef BUTTON_UP_INVERTED
#error Missing configuration of BUTTON_UP_INVERTED
#endif //BUTTON_UP_INVERTED

#ifndef BUTTON_DOWN_PIN
#error Missing configuration of BUTTON_DOWN_PIN
#endif //BUTTON_DOWN_PIN
#ifndef BUTTON_DOWN_PULL
#error Missing configuration of BUTTON_DOWN_PULL
#endif //BUTTON_DOWN_PULL
#ifndef BUTTON_DOWN_INVERTED
#error Missing configuration of BUTTON_DOWN_INVERTED
#endif //BUTTON_DOWN_INVERTED

#ifdef BUTTON_EXTENDED
#ifndef BUTTON_SELECT_PIN
#error Missing configuration of BUTTON_SELECT_PIN
#endif //BUTTON_SELECT_PIN
#ifndef BUTTON_SELECT_PULL
#error Missing configuration of BUTTON_SELECT_PULL
#endif //BUTTON_SELECT_PULL
#ifndef BUTTON_SELECT_INVERTED
#error Missing configuration of BUTTON_SELECT_INVERTED
#endif //BUTTON_SELECT_INVERTED

#ifndef BUTTON_BACK_PIN
#error Missing configuration of BUTTON_BACK_PIN
#endif //BUTTON_BACK_PIN
#ifndef BUTTON_BACK_PULL
#error Missing configuration of BUTTON_BACK_PULL
#endif //BUTTON_BACK_PULL
#ifndef BUTTON_BACK_INVERTED
#error Missing configuration of BUTTON_BACK_INVERTED
#endif //BUTTON_BACK_INVERTED
#endif //BUTTON_EXTENDED

#undef PIN
#undef PULL
#undef INVERTED

#define PIN_BIT 0
#define PIN_MASK 0xf

#define PULL_BIT 4
#define PULL_MASK 0x1

#define INVERTED_BIT 5
#define INVERTED_MASK 0x1

#define GET_OFFSET(state) state##_BIT
#define GET_MASK(state) state##_MASK

#define SET_STATE(value, state) ((value & GET_MASK(state)) << GET_OFFSET(state))
#define GET_STATE(value, state) ((value >> GET_OFFSET(state)) & GET_MASK(state))

Button buttons[BUTTON_MAX];
byte buttons_state[BUTTON_MAX] = {
  SET_STATE(BUTTON_UP_PIN, PIN) | SET_STATE(BUTTON_UP_PULL, PULL) | SET_STATE(BUTTON_UP_INVERTED, INVERTED), 
  SET_STATE(BUTTON_DOWN_PIN, PIN) | SET_STATE(BUTTON_DOWN_PULL, PULL) | SET_STATE(BUTTON_DOWN_INVERTED, INVERTED), 
#ifdef BUTTON_EXTENDED
  SET_STATE(BUTTON_SELECT_PIN, PIN) | SET_STATE(BUTTON_SELECT_PULL, PULL) | SET_STATE(BUTTON_SELECT_INVERTED, INVERTED), 
  SET_STATE(BUTTON_BACK_PIN, PIN) | SET_STATE(BUTTON_BACK_PULL, PULL) | SET_STATE(BUTTON_BACK_INVERTED, INVERTED),
#endif //BUTTON_EXTENDED
};

/*
 * Init controls stuff
 */
void controls_init() {
  DEBUG_LOG_LN("Init Controls");
  for(int i = 0; i < BUTTON_MAX; ++i) {
    buttons[i].setMode(Button::OneShotTimer);
    buttons[i].setTimer(750);
    buttons[i].setRefresh(150);
    buttons[i].assign(GET_STATE(buttons_state[i], PIN));
    if(GET_STATE(buttons_state[i], PULL)) {
      buttons[i].turnOnPullUp();
    } else {
      buttons[i].turnOffPullUp();
    }
    buttons[i].setInverted(GET_STATE(buttons_state[i], INVERTED));
  }
}

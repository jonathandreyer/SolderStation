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

#ifdef BUTTON_STANDBY
#ifndef BUTTON_STANDBY_PIN
#error Missing configuration of BUTTON_STANDBY_PIN
#endif //BUTTON_STANDBY_PIN
#ifndef BUTTON_STANDBY_PULL
#error Missing configuration of BUTTON_STANDBY_PULL
#endif //BUTTON_STANDBY_PULL
#ifndef BUTTON_STANDBY_INVERTED
#error Missing configuration of BUTTON_STANDBY_INVERTED
#endif //BUTTON_STANDBY_INVERTED
#endif //BUTTON_STANDBY

#undef PIN
#undef PULL
#undef INVERTED

#define PIN_BIT 0
#define PIN_MASK 0x1f

#define PULL_BIT 5
#define PULL_MASK 0x1

#define INVERTED_BIT 6
#define INVERTED_MASK 0x1

#define GET_OFFSET(state) state##_BIT
#define GET_MASK(state) state##_MASK

#define SET_STATE(value, state) ((value & GET_MASK(state)) << GET_OFFSET(state))
#define GET_STATE(value, state) ((value >> GET_OFFSET(state)) & GET_MASK(state))

word buttons_state[CONTROL_MAX] = {
  SET_STATE(BUTTON_UP_PIN, PIN) | SET_STATE(BUTTON_UP_PULL, PULL) | SET_STATE(BUTTON_UP_INVERTED, INVERTED), 
  SET_STATE(BUTTON_DOWN_PIN, PIN) | SET_STATE(BUTTON_DOWN_PULL, PULL) | SET_STATE(BUTTON_DOWN_INVERTED, INVERTED),
#ifdef BUTTON_EXTENDED
  SET_STATE(BUTTON_SELECT_PIN, PIN) | SET_STATE(BUTTON_SELECT_PULL, PULL) | SET_STATE(BUTTON_SELECT_INVERTED, INVERTED), 
  SET_STATE(BUTTON_BACK_PIN, PIN) | SET_STATE(BUTTON_BACK_PULL, PULL) | SET_STATE(BUTTON_BACK_INVERTED, INVERTED),
#endif //BUTTON_EXTENDED
#ifdef BUTTON_STANDBY
  SET_STATE(BUTTON_STANDBY_PIN, PIN) | SET_STATE(BUTTON_STANDBY_PULL, PULL) | SET_STATE(BUTTON_STANDBY_INVERTED, INVERTED),
#endif //BUTTON_STANDBY
};

Control::~Control() {
}

class ButtonControl: public Control {
  byte i;
  Button button;
public:
  ButtonControl(byte i) {
    this->i = i;
#ifdef BUTTON_STANDBY
    if(i == CONTROL_STANDBY) {
      button.setMode(Button::Memory); 
    } else {
#endif //BUTTON_STANDBY
    button.setMode(Button::OneShotTimer);
    button.setTimer(750);
    button.setRefresh(150);
#ifdef BUTTON_STANDBY
    }
#endif //BUTTON_STANDBY
    button.assign(GET_STATE(buttons_state[i], PIN));
    if(GET_STATE(buttons_state[i], PULL)) {
      button.turnOnPullUp();
    } else {
      button.turnOffPullUp();
    }
    button.setInverted(GET_STATE(buttons_state[i], INVERTED));
  }
  
  virtual void acknowledge() {
#ifdef BUTTON_STANDBY
    if(i != CONTROL_STANDBY) {
      button.acknowledge();
    }
#else //BUTTON_STANDBY
    button.acknowledge();
#endif //BUTTON_STANDBY
  }
  
  virtual int getValue()  {
    return (button.check() & 0x3)? 1:0;
  }
  
  ~ButtonControl() {
  }
};

#ifdef ROTATING_UP_DOWN
class RotatingDevice {
  byte plus;
  byte minus;
  
public:
  RotatingDevice(byte plus, byte minus) {
    this->plus = plus;
    this->minus = minus;
    if(GET_STATE(buttons_state[plus], PULL)) {
      pinMode(GET_STATE(buttons_state[plus], PIN), INPUT_PULLUP);
    } else {
      pinMode(GET_STATE(buttons_state[plus], PIN), INPUT);
    }
    if(GET_STATE(buttons_state[minus], PULL)) {
      pinMode(GET_STATE(buttons_state[minus], PIN), INPUT_PULLUP);
    } else {
      pinMode(GET_STATE(buttons_state[minus], PIN), INPUT);
    }
    PCintPort::attachInterrupt(tPIN1, quicfunc0, FALLING);
  }
  
  byte getPlusControl() {
    return plus;
  }
  
  byte getMinusControl() {
    return minus;
  }
  
  int getValue() {
  }
  
  void init() {
  }
};
RotatingDevice upDownRotationDevice(CONTROL_UP, CONTROL_DOWN);

class RotatingControl : public Control {
  byte i;
  RotatingDevice *rotating_device;
public:
  RotatingControl(byte i, RotatingDevice *rotating_device) {
    this->i = i;
    this->rotating_device = rotating_device;
  }
  
  virtual int getValue()  {
    bool plus = rotating_device->getPlusControl() == i;
    int value = rotating_device->getValue();
    if(plus && value > 0) {
      return value;
    }
    if(!plus && value < 0) {
      return -value;
    }
    return 0;
  }
  
  virtual void acknowledge() {
  }
  
  ~RotatingControl() {
  }
};
#endif //ROTATING_UP_DOWN

static union {
  byte buttonControlData[sizeof(ButtonControl)];
#ifdef ROTATING_UP_DOWN
  byte rotatingControlData[sizeof(RotatingControl)];
#endif //ROTATING_UP_DOWN
} __controls [CONTROL_MAX];
Control* controls[CONTROL_MAX];

// Placement new
inline void *operator new(size_t, void *buf) { return buf; }

/*
 * Init controls stuff
 */
void controls_init() {
  DEBUG_LOG_LN("Init Controls");
#ifdef ROTATION_UP_DOWN
  upDownRotationDevice.init();
#endif //ROTATION_UP_DOWN
  for(int i = 0; i < CONTROL_MAX; ++i) {
#ifdef ROTATING_UP_DOWN
    if(i == CONTROL_UP || i == CONTROL_DOWN) {
      controls[i] = new(&__controls[i]) RotatingControl(i, &upDownRotationDevice);
    } else {
#endif //ROTATING_UP_DOWN
    controls[i] = new(&__controls[i]) ButtonControl(i);
#ifdef ROTATING_UP_DOWN
    }
#endif //ROTATING_UP_DOWN
  }
}

#include <Arduino.h>
#include "jk_bms.h"
#include "display.h"

// const u8 S1_tx = 4;
// const u8 S1_rx = 5;
// arduino::UART Serial1(S1_tx, S1_rx, NC, NC);

const u32 BUAD_RATE = 115200;
const u32 JK_BAUD = 115200;

JK_BMS bms;
CIRCULAR_DISPLAY display;

void setup()
{
  Serial.begin(BUAD_RATE);
  Serial1.begin(JK_BAUD);

  display.setup();
}

void loop()
{  
  static uint32_t last = 0;
  static u8 state = 1;
  if (millis() - last > 5000)
  {
    last = millis();
    state++;
  }

  switch (state)
  {
  case 1:
    display.draw_565();
    break;
  case 2:
    display.set_init_metric();
    state++;
    break;
  case 3:
  case 4:
  case 5:
    display.test_metric();
    break;
  default:
    state = 1;
    break;
  }

  // static unsigned long dt = 0;

  // bms.req_status();
  // bms.print_full_status();

  // Serial.print(millis() - dt);
  // dt = millis();

  // delay(1000);
}
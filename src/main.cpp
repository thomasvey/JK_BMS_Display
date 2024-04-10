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
  static u8 state = 0;
  if(millis() - last > 2000){
    last = millis();
    state ++;
  }

  switch (state)
  {
  case 0:
    display.draw_png();
    break;
  case 1:
    display.set_init_ring_meter();
    break;
  case 2:
    display.test_ring_meter();
    break;
  default:
    state = 0;
    break;
  }
  
  // static unsigned long dt = 0;

  // bms.req_status();
  // bms.print_full_status();
  
  // Serial.print(millis() - dt);
  // dt = millis();

  //delay(1000);
}
#pragma once

#include "Arduino.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

// class FRAME
// {
// public:
//   struct SEND
//   {
//     u8 header[2] = {0x55, 0xAA};
//     u8 addr = 0x01;
//     u8 cmd = 0xFF;
//     u16 data = 0x0000;
//     u8 check = 0xFF;
//   };

//   // low byte -> right
//   // high byte -> left
//   struct SEND2
//   {
//     u8 header[2] = {0x4E, 0x57};
//     u8 len[2] = {0, 23};
//     u8 ter_num[4] = {0x00};
//     u8 cmd = 0;
//     u8 src = 0x03;
//     u8 type = 0x00;
//     u8 data[2] = {0x00};
//     u8 rec_nr[4] = {0x00};
//     u8 end = 0x68;
//     u8 check[4] = {0x00};
//   };

//   struct RECV
//   {
//     u8 header[2] = {0xEB, 0x90};
//     u8 add = 0;
//     u8 cmd = 0;
//     u8 data[69] = {0};
//     u8 check = 0;
//   };
// };

class JK_BMS
{
  const u8 req_len = 21;
  const u8 req_buffer[21] = {0x4E, 0x57, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x01, 0x29};

  size_t rsp_len = 1024;
  u8 rsp_buffer[1024] = {0};

  enum INDEX
  {
    volt = 58,
    current = 61,
    soc = 64,
    current_callibration = 176,
    temprature_MOS = 49,
    temprature_1 = 52,
    temprature_2 = 55,
    sensors = 66,
    cycels = 68,
  };



  float get_voltage();
  float get_current();
  u8 get_SOC();
  
  u16 get_current_calibration();
  u8 get_temprature_MOS();
  u8 get_temprature_1();
  u8 get_temprature_2();
  u8 get_sensors();
  u8 get_cycels();

public:
  void req_status();
  void print_status();
  void print_full_status();

  JK_BMS();
};
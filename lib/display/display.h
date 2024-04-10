
class CIRCULAR_DISPLAY
{
  uint8_t radius = 0;
  int16_t xpos = 0;
  int16_t ypos = 0;

  // RGB565 5bit 6bit 5bit
  // https://rgbcolorpicker.com/565
  uint16_t back_color = 0;
  uint16_t font_color = 0;
  uint16_t lable_color = 0;
  uint16_t bar_color = 0;
  uint16_t bar_back_color = 0;

public:

  void setup();
  void clear(uint16_t color);
  void test();
  void ringMeter(int8_t val, const char *units);
};
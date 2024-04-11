
class CIRCULAR_DISPLAY
{
  uint8_t radius = 0;
  int16_t xpos = 0;
  int16_t ypos = 0;

  // RGB565 5bit 6bit 5bit
  // https://rgbcolorpicker.com/565
  uint16_t current_color = 0;
	uint16_t batter_color = 0;
	uint16_t volt_color = 0;
  uint16_t back_color = 0;
  uint16_t bar_back_color = 0;

  bool init_metric = true;

public:
  void setup();
  void clear(uint16_t color);
  
  void draw_565();

  void set_init_metric();
  void test_metric();
  void update_metric(float current, float battery, float volt);
  void print_val(float val, const char *format, const char sign, uint8_t font_size, uint16_t color, uint8_t pos_x, uint8_t pos_y);
  

};
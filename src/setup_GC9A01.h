// See SetupX_Template.h for all options available
#define USER_SETUP_ID 999

#define GC9A01_DRIVER

////////////////////////////////////////////////////////////////////////////////////////////
// RP2040 config and pins
////////////////////////////////////////////////////////////////////////////////////////////

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  5000000
//#define SPI_FREQUENCY  80000000
#define SPI_FREQUENCY  27000000

#define RP2040_PIO_CLK_DIV 1
#define TFT_SPI_PORT 0

#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_CS   17
#define TFT_DC   20
#define TFT_RST  22
#define TFT_BL   21


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

////////////////////////////////////////////////////////////////////////////////////////////
// Display params
////////////////////////////////////////////////////////////////////////////////////////////
#define TFT_WIDTH  240
#define TFT_HEIGHT 240
#define TFT_INVERSION_ON

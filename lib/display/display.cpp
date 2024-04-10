// This is a test sketch being developed for a new arc based meter widget
// The meter graphic is fully anti-aliased to avoid jaggy pixelated edges

// For this demo randomly sized meters are drawn, cycled and redrawn a random size.
// The meter is ramped up and down 0-100 and 100-0, then pauses before a new
// random sized meter is drawn

// If the radius is > 25 then the value is drawn in the middle

// The outer ring of the meter uses the drawSmoothCircle function (which draws
// a narrow full circle smooth arc)

// If DRAW_DIGITS is defined the OpenFontRender library must be loaded since
// the sketch uses a scaleable TrueType font for the text and numerals.
// https://github.com/Bodmer/OpenFontRender

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "display.h"

// Front for Text
#include "NotoSans_Bold.h"
#include "OpenFontRender.h"
#define TTF_FONT NotoSans_Bold

// Include the PNG decoder library
#include <PNGdec.h>
#include "panda.h"			// Image is stored here in an 8-bit array
PNG png;					// PNG decoder instance
#define MAX_IMAGE_WIDTH 240 // Adjust for your images

TFT_eSPI tft = TFT_eSPI();			 // Invoke custom library with default width and height
TFT_eSprite spr = TFT_eSprite(&tft); // Declare Sprite object "spr" with pointer to "tft" object
OpenFontRender ofr;

void CIRCULAR_DISPLAY::setup(void)
{
	Serial.begin(115200);

	tft.begin();
	tft.setRotation(1);
	tft.setViewport(0, 0, 240, 240);

	// Loading a font takes a few milliseconds, so for test purposes it is done outside the test loop
	if (ofr.loadFont(TTF_FONT, sizeof(TTF_FONT)))
	{
		Serial.println("Render initialize error");
		delay(5000);
	}
	// ofr.unloadFont(); // Recover space used by font metrics etc.

	radius = tft.width() / 2;
	xpos = tft.width() / 2;
	ypos = tft.height() / 2;

	back_color = TFT_BLACK;
	font_color = TFT_PINK;
	lable_color = TFT_WHITE;
	bar_color = TFT_VIOLET;
	bar_back_color = TFT_DARKCYAN;

	clear(back_color);
}

void CIRCULAR_DISPLAY::clear(uint16_t color)
{
	tft.fillScreen(color);
}


void png_draw_callback(PNGDRAW *pDraw)
{
	int32_t x_pos = 0;
	int32_t y_pos = 0;
	uint16_t lineBuffer[MAX_IMAGE_WIDTH];
	png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
	tft.pushImage(x_pos, y_pos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void CIRCULAR_DISPLAY::draw_png()
{
	int16_t rc = png.openFLASH((uint8_t *)panda, sizeof(panda), png_draw_callback);
	if (rc == PNG_SUCCESS)
	{
		Serial.println("Successfully opened png file");
		//Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
		tft.startWrite();
		uint32_t dt = millis();
		rc = png.decode(NULL, 0);
		Serial.print(millis() - dt);
		Serial.println("ms");
		tft.endWrite();
		// png.close(); // not needed for memory->memory decode
	}
}

void CIRCULAR_DISPLAY::test_ring_meter()
{
	static int8_t value = 0;
	static int8_t ramp = 1;

	value += ramp;
	ring_meter(value, "U+15");

	if ((value < -99) || (value > 100))
	{
		ramp = -ramp;
	}

	// const u8 TFT_W = 240;
	// const u8 TFT_H = 240;
	// tft.setTextColor(TFT_BLACK, TFT_RED);
	// tft.drawCentreString("* TFT_S6D02A1 *", TFT_W / 2, 4, 1);
	// tft.setTextColor(TFT_YELLOW, TFT_BLUE);
	// tft.drawCentreString("Adapted by Bodmer", TFT_W / 2, TFT_H - 12, 1);
}


void CIRCULAR_DISPLAY::set_init_ring_meter()
{
	init_ring_meter = true;
}

void CIRCULAR_DISPLAY::ring_meter(int8_t val, const char *lable)
{
	static uint16_t last_angle = 30;
	uint8_t thickness = radius / 5;
	uint8_t val_pos_offset = 15;

	if (init_ring_meter)
	{
		init_ring_meter = false;
		last_angle = 30;
		tft.fillCircle(xpos, ypos, radius, back_color);
		// tft.drawSmoothCircle(xpos, ypos, radius, bar_color, back_color);
		tft.drawArc(xpos, ypos, radius, radius - thickness, last_angle, 330, bar_back_color, bar_color);
	}

	// Range here is 0-100 so value is scaled to an angle 30-330
	int val_angle = map(abs(val), 0, 100, 30, 330);

	if (last_angle != val_angle)
	{
		ofr.setDrawer(spr); // Link renderer to sprite (font will be rendered in sprite spr)

		// This code gets the font dimensions in pixels to determine the required the sprite size
		ofr.setFontSize((6 * radius) / 4);
		ofr.setFontColor(font_color, back_color);

		// The OpenFontRender library only has simple print functions...
		// Digit jiggle for changing values often happens with proportional fonts because
		// digit glyph width varies ( 1 narrower that 4 for example). This code prints up to
		// 3 digits with even spacing.
		// A few experimental fudge factors are used here to position the
		// digits in the sprite...
		// Create a sprite to draw the digits into
		uint8_t w = ofr.getTextWidth("444");
		uint8_t h = ofr.getTextHeight("4") + 4;
		spr.createSprite(w, h + 2);
		spr.fillSprite(back_color); // (TFT_BLUE); // (DARKER_GREY);
		char str_buf[8];			// Buffed for string
		itoa(val, str_buf, 10);		// Convert value to string (null terminated)
		uint8_t ptr = 0;			// Pointer to a digit character
		uint8_t dx = 4;				// x offset for cursor position
		if (val < 100)
			dx = ofr.getTextWidth("4") / 2; // Adjust cursor x for 2 digits
		if (val < 10)
			dx = ofr.getTextWidth("4"); // Adjust cursor x for 1 digit
		if (val < 0)
			dx = ofr.getTextWidth("4") / 2; // Adjust cursor x for negativ digit
		while ((uint8_t)str_buf[ptr] != 0)
			ptr++; // Count the characters
		while (ptr)
		{
			ofr.setCursor(w - dx - w / 20, -h / 2 + 4); // Offset cursor position in sprite
			ofr.rprintf(str_buf + ptr - 1);				// Draw a character
			str_buf[ptr - 1] = 0;						// Replace character with a null
			dx += 1 + w / 3;							// Adjust cursor for next character
			ptr--;										// Decrement character pointer
		}
		spr.pushSprite(xpos - w / 2, ypos - val_pos_offset - h / 2); // Push sprite containing the val number
		spr.deleteSprite();											 // Recover used memory

		// Print Lable
		ofr.setDrawer(tft);
		ofr.setFontColor(lable_color, back_color);
		ofr.setFontSize(radius / 2.0);
		ofr.setCursor(xpos, ypos + (radius * 0.25));
		ofr.cprintf(lable);

		// Update the arc, only the zone between last_angle and new val_angle is updated
		if (val_angle > last_angle)
		{
			tft.drawArc(xpos, ypos, radius, radius - thickness, last_angle, val_angle, bar_color, bar_back_color);
		}
		else
		{
			tft.drawArc(xpos, ypos, radius, radius - thickness, val_angle, last_angle, bar_back_color, bar_color);
		}
		last_angle = val_angle;
	}
}
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

#include "pic/kaos565.h"

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

	current_color = 0x9816;
	batter_color = 0xfdc0;
	volt_color = TFT_PINK;
	back_color = TFT_BLACK;
	bar_back_color = TFT_DARKCYAN;

	clear(back_color);
}

void CIRCULAR_DISPLAY::clear(uint16_t color)
{
	tft.fillScreen(color);
}

void CIRCULAR_DISPLAY::draw_565()
{
	// https://notisrac.github.io/FileToCArray/
	tft.pushImage(0, 0, 240, 240, KAOS_Logo_240_b);
}

void CIRCULAR_DISPLAY::set_init_metric()
{
	init_metric = true;
}

void CIRCULAR_DISPLAY::test_metric()
{
	static float volt = 0;
	static float curr = 0;
	static float batt = 0;
	static float ramp = 1;

	batt += ramp;
	volt = 40 + batt * 0.1;
	curr = batt * 2;
	update_metric(curr, abs(batt), volt);

	if ((batt < -99) || (batt > 99))
	{
		ramp = -ramp;
	}
}

void CIRCULAR_DISPLAY::update_metric(float current, float battery, float volt)
{
	static uint16_t last_angle = 180;
	uint8_t thickness = radius / 5;
	static float last_volt = 0;
	static float last_current = 0;
	static float last_battery = 0;

	if (init_metric)
	{
		init_metric = false;
		last_angle = 180;
		tft.fillCircle(xpos, ypos, radius, back_color);
		tft.drawArc(xpos, ypos, radius, radius - thickness, 30, 330, bar_back_color, current_color);
	}


	if (last_current != current)
	{
		last_current = current;
		print_val(current, "%+04.0f%c", 'A', 90, current_color, 120, 80);

		int val_angle = map(current, -200, 200, 30, 330);
		if (val_angle > 180)
		{
			if (val_angle > last_angle)
			{
				tft.drawArc(xpos, ypos, radius, radius - thickness, last_angle, val_angle, current_color, bar_back_color);
			}
			else
			{
				tft.drawArc(xpos, ypos, radius, radius - thickness, val_angle, last_angle, bar_back_color, current_color);
			}
		}
		else
		{
			if (val_angle < last_angle)
			{
				tft.drawArc(xpos, ypos, radius, radius - thickness, val_angle, last_angle, current_color, bar_back_color);
			}
			else
			{
				tft.drawArc(xpos, ypos, radius, radius - thickness, last_angle, val_angle, bar_back_color, current_color);
			}
		}
		last_angle = val_angle;
	}

	if (last_battery != battery)
	{
		last_battery = battery;
		uint16_t color = batter_color;
		if (battery <= 10)
		{
			color = TFT_RED;
		}
		print_val(battery, "%.0f%c", '%', 70, color, 120, 150);
	}

	if (last_volt != volt)
	{
		last_volt = volt;
		print_val(volt, "%.2f%c", 'V', 50, volt_color, 120, 220);
	}
}

void CIRCULAR_DISPLAY::print_val(float val, const char *format, const char sign, uint8_t font_size, uint16_t color, uint8_t pos_x, uint8_t pos_y)
{
	ofr.setDrawer(spr);
	ofr.setFontSize(font_size);
	ofr.setFontColor(color, back_color);

	uint8_t w = ofr.getTextWidth("4") * 5.8;
	uint8_t h = ofr.getTextHeight("4");

	spr.createSprite(w + 4, h + 4);
	spr.fillSprite(back_color);
	ofr.setCursor(w / 2, -h / 2);
	ofr.cprintf(format, val, sign);

	spr.pushSprite(pos_x - w / 2, pos_y - h / 2);
	spr.deleteSprite();
}

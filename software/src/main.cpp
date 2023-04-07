#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "config.h"

/*
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <Coordinate.h>
#include <ArduinoJson.h>
#include <DS3231.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <MAX485_DMX.h>
*/

// For ESP32 Dev board (only tested with ILI9341 display)
// The hardware SPI can be mapped to any pins
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 15 // Chip select control pin
#define TFT_DC 2  // Data Command control pin
#define TFT_RST 4 // Reset pin (could connect to RST pin)
// #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#define DISPLAY_MIRROR_X 1

#define MAIN_VIEW_SCALE_FACTOR 3.5f

// initialize ILI9341 TFT library
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

int mvs(int v)
{
	return ((float)v) * MAIN_VIEW_SCALE_FACTOR;
}

// RGB565
#define DARK_GREY 0x18C5  ///<  24,  24,  41
#define GREEN 0x07A0	  ///<   3, 246,   5
#define DARK_BLUE 0x0290  ///<   0,  82, 132
#define LIGHT_BLUE 0x5CBD ///<  94, 149, 234

void pixel(int16_t x, int16_t y, uint16_t color)
{
	tft.fillRect(x, y, mvs(1), mvs(1), color);
}

void drawMainViewGrid()
{
	for (int x = mvs(0); x < mvs(91); x += mvs(2))
	{
		for (int y = mvs(1); y < mvs(40); y += mvs(2)) // skip first and last line
		{
			pixel(x, y, DARK_GREY);
		}
	}

	/*
	for (int y = mvs(1); y < mvs(40); y += mvs(2))
	{ // skip first and last line
		tft.drawPixel(mvs(91), y, ILI9341_RED);
	}
	*/
}

void drawMainViewPause()
{
	tft.fillRect(mvs(60), mvs(7), mvs(3), mvs(1), GREEN);
	tft.fillRect(mvs(60), mvs(11), mvs(3), mvs(1), GREEN);
}

void drawMainViewEQAxis()
{
	bool colorFlip = false;

	for (int x = mvs(10); x < mvs(89); x += mvs(2)) // y axis
	{
		pixel(x, mvs(37), colorFlip ? DARK_BLUE : LIGHT_BLUE);
		colorFlip = !colorFlip;
	}

	colorFlip = true;

	for (int y = mvs(19); y < mvs(37); y += mvs(2)) // x axis
	{
		pixel(mvs(10), y, colorFlip ? DARK_BLUE : LIGHT_BLUE);
		colorFlip = !colorFlip;
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.println("ILI9341 Test!");

	tft.begin();

	// read diagnostics (optional but can help debug problems)
	uint8_t x = tft.readcommand8(ILI9341_RDMODE);
	Serial.print("Display Power Mode: 0x");
	Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDMADCTL);
	Serial.print("MADCTL Mode: 0x");
	Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDPIXFMT);
	Serial.print("Pixel Format: 0x");
	Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDIMGFMT);
	Serial.print("Image Format: 0x");
	Serial.println(x, HEX);
	x = tft.readcommand8(ILI9341_RDSELFDIAG);
	Serial.print("Self Diagnostic: 0x");
	Serial.println(x, HEX);

	tft.setRotation(3);

	tft.fillScreen(ILI9341_BLACK);

	// tft.drawLine(0, 0, 100, 200, ILI9341_GREEN);

	drawMainViewGrid();
	drawMainViewPause();
	drawMainViewEQAxis();
}

void loop(void)
{
}

#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
#include "M5Stack.h"
#elif defined(ARDUINO_M5STACK_Core2) // M5Stack Core2
#include "M5Core2.h"
#endif

#include "OpenFontRender.h" // Include after M5Stack.h / M5Core2.h

OpenFontRender render;

void setup()
{
	// put your setup code here, to run once:
	M5.begin();
	M5.Lcd.fillScreen(TFT_BLACK);

	render.setSerial(Serial);	  // Need to print render library message
	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont("/JKG-M_3_Tiny.ttf"))
	{
		Serial.println("Render initialize error");
		return;
	}

	render.setDrawer(M5.Lcd); // Set drawer object
	/* You can also be written as follows. */
	// render.setDrawPixel(M5.Lcd.drawPixel);
	// render.setDrawFastHLine(M5.Lcd.drawFastHLine); // optional
	// render.setStartWrite(M5.Lcd.startWrite);       // optional
	// render.setEndWrite(M5.Lcd.endWrite);           // optional

	unsigned long t_start = millis();

	render.setFontColor(TFT_WHITE);
	render.printf("Hello World\n");
	render.seekCursor(0, 10);

	render.setFontSize(30);
	render.setFontColor(TFT_GREEN);
	render.printf("完全なUnicodeサポート\n");
	render.seekCursor(0, 10);

	render.setFontSize(40);
	render.setFontColor(TFT_ORANGE);
	render.printf("こんにちは世界\n");

	unsigned long t_end = millis();
	Serial.printf("Time: %ld ms\n", t_end - t_start);
}

void loop()
{
	// put your main code here, to run repeatedly:
}

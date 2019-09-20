#include <Adafruit_NeoPixel.h>

#define FLORA
//#define GEMMA

/*#ifdef*/ FLORA
#define LED_PIN                   9 // NeoPixel strip is connected here
#define NUM_PIXELS               39
#define ONBOARD_PIN               7 // the pin that the LED is attached to
/*#endif */

//#ifdef GEMMA
//#define SENSOR_PIN                1 // Analog input for flex sensor
//#define LED_PIN                   0 // NeoPixel strip is connected here
//#define NUM_PIXELS                1
//#define ONBOARD_PIN               1 // the pin that the LED is attached to
//#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t darkRed, darkOrange, black;

void setup() {
	// put your setup code here, to run once:
	strip.begin();
	strip.show();
	Serial.begin(9600);
	darkRed = strip.Color(96, 0, 0);
	darkOrange = strip.Color(64, 32, 0);
	black = strip.Color(0, 0, 0);
}

void loop() {
	// put your main code here, to run repeatedly:
	Serial.println("Writing bunches of pixels...");
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		switch (i % 3) {
		case 0: strip.setPixelColor(i, darkRed);    break;
		case 1: strip.setPixelColor(i, darkOrange); break;
		default: strip.setPixelColor(i, black);      break;
		}
	}
}
#include <Adafruit_CircuitPlayground.h>

// math constants
#define UC_1HALF 0.5
#define UC_3SQRT2 0.866

// change this to change how much historical data is averaged
// (loop runs at about 6ms/loop with no history, will slow with more history)
#define HISTORY_LEN 50

// readings
float x, y;

// history
float history[2][HISTORY_LEN];
int history_idx;

void setup () {
	// get started
	CircuitPlayground.begin();

	// initialize memory
	history_idx = 0;
	for (int i = 0; i < HISTORY_LEN; i++) {
		history[0][i] = history[1][i] = 0;
	}
}

/**
 * @brief Scale the brightness to appear more linear since the LEDs don't 
 * luminate in a linear fashion.
 * 
 * @param x The value to scale (0 - 255)
 * @return int The scaled value
 */
int quadratic_brightness(int x) {
	return (int) round(0.003921568627 * (double) pow(x, 2));
}

/**
 * @brief Take a reading and create a color with an added Red or Blue 
 * component based on the sign of the value.
 * 
 * @param r The reading
 * @return int The color
 */
int reading_to_color(float r) {
	if (r >= 0) {
		return (quadratic_brightness((int)(0xFF * r)) << 16) | 0x2000;
	} else {
		return quadratic_brightness((int)(0xFF * -r)) | 0x2000;
	}
}

/**
 * @brief Average together historical values to smooth the output
 * 
 * @param data The data array of length HISTORY_LEN
 * @return float The average
 */
float historical_avg(float* data) {
	float avg = 0;

	for (int i = 0; i < HISTORY_LEN; i++) {
		avg += data[i];
	}

	return avg / HISTORY_LEN;
}

void loop () {
	// get accel readings

	x = CircuitPlayground.motionX();
	y = CircuitPlayground.motionY();

	// scale to unit circle and record

	history[0][history_idx] =  abs(x) > 10.0 ? 1.0 : x / 10.0;
	history[1][history_idx] = abs(y) > 10.0 ? 1.0 : y / 10.0;

	if (++history_idx > HISTORY_LEN) { history_idx = 0; }

	x = historical_avg(history[0]);
	y = historical_avg(history[1]);

	// write out values

	CircuitPlayground.setPixelColor(0, reading_to_color(-UC_1HALF * x + UC_3SQRT2 * y));
	CircuitPlayground.setPixelColor(1, reading_to_color(-UC_3SQRT2 * x + UC_1HALF * y));
	CircuitPlayground.setPixelColor(2, reading_to_color(-x));
	CircuitPlayground.setPixelColor(3, reading_to_color(-UC_3SQRT2 * x - UC_1HALF * y));
	CircuitPlayground.setPixelColor(4, reading_to_color(-UC_1HALF * x - UC_3SQRT2 * y));

	CircuitPlayground.setPixelColor(5, reading_to_color(UC_1HALF * x - UC_3SQRT2 * y));
	CircuitPlayground.setPixelColor(6, reading_to_color(UC_3SQRT2 * x - UC_1HALF * y));
	CircuitPlayground.setPixelColor(7, reading_to_color(x));
	CircuitPlayground.setPixelColor(8, reading_to_color(UC_3SQRT2 * x + UC_1HALF * y));
	CircuitPlayground.setPixelColor(9, reading_to_color(UC_1HALF * x + UC_3SQRT2 * y));
}

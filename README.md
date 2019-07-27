# circuit-playground-directional-leds

Uses the X and Y axes of the Adafruit Circuit Playground Express to light up LEDs based on if the board is titled up or down.

## about

Static green component with a red added to the LEDs that are further from the gravitational source (Earth usually) and blue added to the LEDs that are closer to the gravitational source (also usually Earth).

The two gravitational vectors are converted to a value from -1 to 1 and then trigonometry is used to calculate values of each LED using a unit circle, and the assumption that each LED is at a multiple of 1/6 pi angles from eachother.

This is meant to be a cool example to play around with, so have fun :)

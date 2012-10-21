/*!
 * \file BiDigit.ino
 * \brief demo for two 7-segement displays with common anode.
 * \author Like Ma <likemartinma@gmail.com>
 * 
 * This sketch counts down from 99 to 0, while each value keeps displaying for 2 seconds.

 * The circuit:
 *   - Pin d1 to digital pin  4
 *   - Pin d2 to digital pin 13
 *   - Pin a  to digital pin 11
 *   - Pin b  to digital pin 10
 *   - Pin c  to digital pin  8
 *   - Pin d  to digital pin  6
 *   - Pin e  to digital pin  5
 *   - Pin f  to digital pin 12
 *   - Pin g  to digital pin  7
 *   - Pin dp to digital pin  9
 */

#include <SevenSegmentDisplay.h>

SevenSegmentDisplay<true, BiDigit<13, 4> > ss(11, 10, 8, 6, 5, 12, 7, 9);

void setup() {
}

void loop() {
	unsigned i = 99;
	do {
		ss.print(i, -1, 2000);
	} while (i--);
}

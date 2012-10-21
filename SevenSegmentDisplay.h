/*!
 * \file SevenSegmentDisplay.h
 * \brief Arduino 7-Segment Display
 * \author Like Ma <likemartinma@gmail.com>
 */

#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H

#if defined(ARDUINO) && ARDUINO >= 100
#	include "Arduino.h"
#else
#	include "WProgram.h"
#endif

template <byte D1>
struct UniDigit {
	enum { N = 1 };
	static byte pin[N];
};

template <byte D1, byte D2>
struct BiDigit {
	enum { N = 2 };
	static byte pin[N];
};

template <byte D1, byte D2, byte D3>
struct TriDigit {
	enum { N = 3 };
	static byte pin[N];
};

template <byte D1, byte D2, byte D3, byte D4>
struct QuadriDigit {
	enum { N = 4 };
	static byte pin[N];
};

template <boolean ANODE, typename DIGITS, unsigned DELAY = 20>
class SevenSegmentDisplay {
public:
	/*!
	 * \brief Initialize 7-segment display.
	 *
	 * See the following ascii diagram for parameters:
	 *
	 *      a
	 *    _____
	 *   |     | 
	 * f |  g  | b
	 *   |_____|
	 *   |     |
	 * e |     | c 
	 *   |_____|    . dp
	 * 
	 *      d
	 */
	SevenSegmentDisplay(byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte dp);

	/*!
	 * \brief Display fixed point number.
	 * \param n The number to display.
	 * \param pt The position of the point ranged from 0 to DIGITS::N - 1, while 0 denotes the lowest digit.
	 * \param duration The duration for displaying.
	 */
	void print(unsigned n, byte pt = -1, unsigned long duration = 0);

	/*!
	 * \brief Display float point number.
	 * \param n The number to display.
	 * \param duration The duration for displaying.
	 */
	void print(float n, unsigned long duration = 0);

	/*!
	 * \brief light off all segments.
	 */
	void clear ();
protected:
	void pickDigit(byte i);
	void printDigit(byte n, bool dp = false);
	void printUnsigned(unsigned n, byte dp);
	void printFloat(float n);

	enum { 
		MAX_SEGMENTS = 8,
		DP_IDX = MAX_SEGMENTS - 1
	};

	byte pin_[MAX_SEGMENTS];
	byte digit_[10];
};

#include "SevenSegmentDisplay.inl"

#endif /* SEVENSEGMENTDISPLAY_H */

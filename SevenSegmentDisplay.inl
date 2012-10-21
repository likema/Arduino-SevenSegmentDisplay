/* $Id$ */

#define DIGIT_DEF(a, b, c, d, e, f, g) \
	(a + (b << 1) + (c << 2) + (d << 3) + (e << 4) + (f << 5) + (g << 6))

template <byte D1>
byte UniDigit<D1>::pin[] = { D1 };

template <byte D1, byte D2>
byte BiDigit<D1, D2>::pin[] = { D2, D1 };

template <byte D1, byte D2, byte D3>
byte TriDigit<D1, D2, D3>::pin[] = { D3, D2, D1 };

template <byte D1, byte D2, byte D3, byte D4>
byte QuadriDigit<D1, D2, D3, D4>::pin[] = { D4, D3, D2, D1 };

template <byte N>
struct UpperBound;

template <>
struct UpperBound <1> {
	enum { VALUE = 10 };
};

template <byte N>
struct UpperBound {
	enum { VALUE = 10 * UpperBound<N-1>::VALUE };
};

template <boolean ANODE>
struct SegmentTrait;

template <>
struct SegmentTrait <true> {
	static byte digit[10];
};

byte SegmentTrait<true>::digit[] = {
	DIGIT_DEF(0, 0, 0, 0, 0, 0, 1),
	DIGIT_DEF(1, 0, 0, 1, 1, 1, 1),
	DIGIT_DEF(0, 0, 1, 0, 0, 1, 0),
	DIGIT_DEF(0, 0, 0, 0, 1, 1, 0),
	DIGIT_DEF(1, 0, 0, 1, 1, 0, 0),
	DIGIT_DEF(0, 1, 0, 0, 1, 0, 0),
	DIGIT_DEF(0, 1, 0, 0, 0, 0, 0),
	DIGIT_DEF(0, 0, 0, 1, 1, 1, 1),
	DIGIT_DEF(0, 0, 0, 0, 0, 0, 0),
	DIGIT_DEF(0, 0, 0, 0, 1, 0, 0),
};

template <>
struct SegmentTrait <false> {
	static byte digit[10];
};

byte SegmentTrait<false>::digit[] = {
	DIGIT_DEF(1, 1, 1, 1, 1, 1, 0),
	DIGIT_DEF(0, 1, 1, 0, 0, 0, 0),
	DIGIT_DEF(1, 1, 0, 1, 1, 0, 1),
	DIGIT_DEF(1, 1, 1, 1, 0, 0, 1),
	DIGIT_DEF(0, 1, 1, 0, 0, 1, 1),
	DIGIT_DEF(1, 0, 1, 1, 0, 1, 1),
	DIGIT_DEF(1, 0, 1, 1, 1, 1, 1),
	DIGIT_DEF(1, 1, 1, 0, 0, 0, 0),
	DIGIT_DEF(1, 1, 1, 1, 1, 1, 1),
	DIGIT_DEF(1, 1, 1, 1, 0, 1, 1),
};

template <boolean ANODE, typename DIGITS, unsigned DELAY>
SevenSegmentDisplay<ANODE, DIGITS, DELAY>::SevenSegmentDisplay(byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte dp)
{
	pin_[0] = a;
	pin_[1] = b;
	pin_[2] = c;
	pin_[3] = d;
	pin_[4] = e;
	pin_[5] = f;
	pin_[6] = g;
	pin_[7] = dp;

	for (byte i = 0; i < MAX_SEGMENTS; ++i) {
		pinMode(pin_[i], OUTPUT);
	}

	for (byte i = 0; i < DIGITS::N; ++i) {
		pinMode(DIGITS::pin[i], OUTPUT);
		digitalWrite(DIGITS::pin[i], ANODE);
	}

	clear ();
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::printDigit(byte n, bool dp)
{
	byte m = SegmentTrait<ANODE>::digit[n];
	for (byte i = 0; i < MAX_SEGMENTS - 1; ++i) {
		digitalWrite(pin_[i], m & 0x01);
		m >>= 1;
	}
	digitalWrite(pin_[DP_IDX], dp ? !ANODE : ANODE);
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::printUnsigned(unsigned n, byte dp)
{
	for (byte i = 0; i < DIGITS::N; ++i) {
		pickDigit(i);
		printDigit(n % 10, dp == i);
		delay(DELAY / DIGITS::N);
		n /= 10;
	}
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::printFloat(float n)
{
	unsigned upper = 10;
	for (byte i = 0; i < DIGITS::N; ++i) {
		if (n < upper) {
			printUnsigned(unsigned(n * (UpperBound<DIGITS::N>::VALUE / upper)), DIGITS::N - 1 - i);
			break;
		}
		upper *= 10;
	}
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::print(unsigned n, byte pt, unsigned long duration)
{
	if (DIGITS::N > 1) {
		if (duration) {
			unsigned long endTime = millis() + duration;
			while (millis () < endTime) {
				printUnsigned(n, pt);
			}
		} else {
			printUnsigned(n, pt);
		}
	} else {
		printDigit(n % 10);
		if (duration)
			delay (duration);
	}
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::print(float n, unsigned long duration)
{
	if (DIGITS::N > 1) {
		if (duration) {
			unsigned long endTime = millis() + duration;
			while (millis () < endTime) {
				printFloat(n);
			}
		} else {
			printFloat(n);
		}
	}
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::clear()
{
	for (byte i = 0; i < MAX_SEGMENTS; ++i) {
		digitalWrite(pin_[i], ANODE);
	}
}

template <boolean ANODE, typename DIGITS, unsigned DELAY>
void SevenSegmentDisplay<ANODE, DIGITS, DELAY>::pickDigit(byte i)
{
	for (byte i = 0; i < DIGITS::N; ++i) {
		digitalWrite(DIGITS::pin[i], !ANODE);
	}
	digitalWrite(DIGITS::pin[i], ANODE);
}

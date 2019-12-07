#include <iostream>
using namespace std;

enum {
    RangeStart = 372304,    // inclusive
    RangeEnd   = 847060,
};

int main()
{
    int pwCount1 = 0;
    int pwCount2 = 0;
    uint32_t doubleBits = 0;

    int i1 = 3;
    int i2 = 7;
    int i3 = 2;
    int i4 = 3;
    int i5 = 0;
    int i6 = 4;

 i1_changed:
    if (i1 >= 9) {
        throw runtime_error("we should be done by now");
    }

 i2_changed:
    if (i2 >= 10) {
        i2 = 0;
        ++i1;
        goto i1_changed;
    }
    else if (i2 < i1) {
        i2 = i1;
    }

    if (i2 == i1) {
        doubleBits |= (1 << 4);
    }
    else {
        doubleBits &= ~(1 << 4);
    }

 i3_changed:
    if (i3 >= 10) {
        i3 = 0;
        ++i2;
        goto i2_changed;
    }
    else if (i3 < i2) {
        i3 = i2;
    }

    if (i3 == i2) {
        doubleBits |= (1 << 3);
    }
    else {
        doubleBits &= ~(1 << 3);
    }

 i4_changed:
    if (i4 >= 10) {
        i4 = 0;
        ++i3;
        goto i3_changed;
    }
    else if (i4 < i3) {
        i4 = i3;
    }

    if (i4 == i3) {
        doubleBits |= (1 << 2);
    }
    else {
        doubleBits &= ~(1 << 2);
    }

 i5_changed:
    if (i5 >= 10) {
        i5 = 0;
        ++i4;
        goto i4_changed;
    }
    else if (i5 < i4) {
        i5 = i4;
    }

    if (i5 == i4) {
        doubleBits |= (1 << 1);
    }
    else {
        doubleBits &= ~(1 << 1);
    }

 i6_changed:
    if (i6 >= 10) {
        i6 = 0;
        ++i5;
        goto i5_changed;
    }
    else if (i6 < i5) {
        i6 = i5;
    }

    if (i6 == i5) {
        doubleBits |= (1 << 0);
    }
    else {
        doubleBits &= ~(1 << 0);
    }

    //

    const int value = (i1   * 100000
                       + i2 * 10000
                       + i3 * 1000
                       + i4 * 100
                       + i5 * 10
                       + i6);
    if (value > RangeEnd) {
        goto complete;
    }

    //

    if (doubleBits == 0)
    {
        ++i6;
        goto i6_changed;
    }
    else if ((   doubleBits & 0b00011) != 0b00001
             && (doubleBits & 0b00111) != 0b00010
             && (doubleBits & 0b01110) != 0b00100
             && (doubleBits & 0b11100) != 0b01000
             && (doubleBits & 0b11000) != 0b10000
             )
    {
        ++pwCount1;

        ++i6;
        goto i6_changed;
    }
    else {
        ++pwCount1;
    }

    //

    ++pwCount2;
    ++i6;
    goto i6_changed;

    // ------

 complete:
    cout << "Different password count, first part: " << pwCount1 << endl
         << "                         second part: " << pwCount2 << endl;

    return 0;
}

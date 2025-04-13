#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// divide and conquer width
static const int mask[] = {0, 8, 12, 14};
// magic numbers for 2 bits
// 0 -> 2
// 1 -> 1
// 2 -> 0
// 3 -> 0
static const int magic[] = {2, 1, 0, 0};

unsigned clz2(uint32_t x, int c)
{
    if (!x && !c)
        return 32;
    
    uint32_t upper = (x >> (16 >> c));
    uint32_t lower = (x & (0xFFFF >> mask[c]));
    
    // 2 bits left which implies c == 3
    // c = 0 => 16 bits
    // c = 1 => 8 bits
    // c = 2 => 4 bits
    // c = 3 => 2 bits
    if (c == 3)
        return upper ? magic[upper] : 2 + magic[lower];
    return upper ? clz2(upper, c + 1) : (16 >> (c)) + clz2(lower, c + 1);
}

#define clz32(x) clz2(x, 0)

static inline int clz64(uint64_t x)
{
    return clz32(x >> 32) ? clz32(x >> 32) :
           clz32(x & 0xFFFFFFFF) + 32;
}

uint64_t sqrti(uint64_t x)
{
    uint64_t m, y = 0;
    if (x <= 1)
        return x;
    
    int total_bits = 64;

    /* clz64(x) returns the count of leading zeros in x.
     * (total_bits - 1 - clz64(x)) gives the index of the highest set bit.
     * Rounding that index down to an even number ensures our starting m is a
     * power of 4.
     */
    int shift = (total_bits - 1 - clz64(x)) & 0xFFFFFFFE;
    m = 1ULL << shift;

    while(m) {
        uint64_t b = y + m;
        y >>= 1;
        if (x >= b) {
            x -= b;
            y += m;
        }
        m >>= 2;
    }
    return y;
}

int main()
{
    uint32_t x = 1024;
    unsigned result = clz2(x, 0);
    printf("Number of leading zeros in %u: %u\n", x, result);

    uint64_t y = 1024;
    unsigned result64 = sqrti(y);
    printf("Square root of %llu: %llu\n", y, result64);
}

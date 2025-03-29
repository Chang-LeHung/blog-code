
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define u32 uint32_t
#define i32 int32_t
#define u64 uint64_t
#define i64 int64_t
#define DIGIT_BIT 30
#define DIGIT_MAX (1 << DIGIT_BIT)

typedef struct BigLong
{
    u32 size;
    u32 data[1];
} BigLong;

BigLong* NewBigLong(i32 digits)
{
    BigLong* result = malloc(sizeof(BigLong) + sizeof(u32) * (digits - 1));
    result->size = digits;
    return result;
}

void FreeBigLong(BigLong* bigLong) { free(bigLong); }

BigLong* BigLongFromLong(i64 value)
{
    // fast path for small values
    i32 sign = value < 0 ? -1 : 1;
    if (value <= DIGIT_MAX)
    {
        BigLong* result = NewBigLong(1);
        result->size = 1 * sign;
        result->data[0] = (u32)value;
        return result;
    }
    i32 n_digits = 0;
    i64 t = value;
    while (t)
    {
        t >>= DIGIT_BIT;
        n_digits++;
    }
    BigLong* result = NewBigLong(n_digits);
    result->size = n_digits * sign;
    for (i32 i = 0; i < n_digits; i++)
    {
        result->data[i] = (u32)(value & DIGIT_MAX);
        value >>= DIGIT_BIT;
    }
    return result;
}

BigLong* BigLongFromHexString(char* str)
{
    i32 len = strlen(str);
    i32 bits = len << 2;
    i32 size = bits / (DIGIT_BIT + 1) + (bits % (DIGIT_BIT + 1)) ? 1 : 0;
    BigLong* result = NewBigLong(size);
    for (i32 i = 0; i < len; i++)
    {
        char c = str[i];
        if (c >= '0' && c <= '9')
        {
            c -= '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            c -= 'a' - 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            c -= 'A' - 10;
        }
        result->data[i >> 5] |= c << ((i & 31) << 1);
    }
    return result;
}

char* BigLongToString(BigLong* bigLong) { return NULL; }

int main() { return 0; }
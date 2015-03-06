

// author: wiki908
// crc32 by notwa@github


#include <map>
#include <vector>
#include "textFinder.h"

inline textFinder::ulong textFinder::crc_reflect(ulong input)
{
    ulong reflected = 0;
    int i;
    for (i = 0; i < 4 * 8; i++) {
        reflected <<= 1;
        reflected |= input & 1;
        input >>= 1;
    }
    return reflected;
}

void textFinder::crc_fill_table(ulong * table, ulong polynomial, int big)
{
    ulong lsb = (big) ? 1 << 31 : 1; /* least significant bit */
    ulong poly = (big) ? polynomial : crc_reflect(polynomial);
    int c, i;

    for (c = 0; c < CRC_TABLE_SIZE; c++, table++) {
        *table = (big) ? c << 24 : c;
        for (i = 0; i < 8; i++) {
            if (*table & lsb) {
                *table = (big) ? *table << 1 : *table >> 1;
                *table ^= poly;
            }
            else {
                *table = (big) ? *table << 1 : *table >> 1;
            }
            *table &= 0xFFFFFFFF;
        }
    }
}

inline void textFinder::crc_le_cycle(ulong * table, ulong * remainder, char c)
{
    ulong byte = table[((*remainder) ^ c) & 0xFF];
    *remainder = ((*remainder) >> 8) ^ byte;
}

inline textFinder::ulong textFinder::crc32(const char * str, const int len)
{
    ulong remainder = crc_starting;
    // Table will be created in func `public:create'
    //crc_fill_table(crc_table, crc_polynomial);

    for (int i = 0; i < len; ++i)
        crc_le_cycle(crc_table, &remainder, str[i]);
    return remainder;
}

// rightTextOut == 0 时, 返回 rightOut 的长度
// rightTextOut != 0 时, 返回 0 成功, 返回负数失败
// leftTextIn 应为字符串, leftHashIn 应为文本的 CRC32
inline int textFinder::setfile(const char * fileName, void * flag)
{
    return setfileh(crc32(fileName, strlen(fileName)), flag);
}
inline int textFinder::find(const char * leftTextIn, char * rightTextOut, void * flag)
{
    return findh(crc32(leftTextIn, strlen(leftTextIn)), rightTextOut, flag);
}


int textFinder::setfileh(const unsigned int fileNameHash, void * flag)
{
    return 0;
}

int textFinder::findh(const int leftHashIn, char * rightTextOut, void * flag)
{
    return 0;
}

int textFinder::create(const char * buff, void * flag)
{
    return 0;
}


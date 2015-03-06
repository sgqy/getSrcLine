

// author: wiki908
// crc32 by notwa@github


#include <cstring>

#include <utility>
#include <list>

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
inline int textFinder::find(const char * leftTextIn, const char** rightTextOut, void * flag)
{
    return findh(crc32(leftTextIn, strlen(leftTextIn)), rightTextOut, flag);
}


int textFinder::create(const char * buff, void * flag)
{
    FDR_HEADER* phdr = (FDR_HEADER*)buff;

    // check package type
    if (phdr->packageInfo != FDR_PACK_INFO) throw FDR_INVALID_PACK;

    // create crc32 table
    crc_starting = phdr->CRCStarting;
    crc_polynomial = phdr->CRCPolynomial;
    crc_fill_table(crc_table, crc_polynomial);

    // record text info
    FDR_SFINFO* sfinfo = (FDR_SFINFO*)(phdr + 1);
    const char* startPoint = (const char*)(sfinfo + phdr->fileCount);

    sub_file subf;
    for (int i = 0; i < phdr->fileCount; ++i)
    {
        line_pair pair;
        line_list list;

        const char* read = startPoint + sfinfo[i].beginOffset;
        for (int j = 0; j < sfinfo[i].lineCount; ++j)
        {
            if (*read != FDR_LINE_ID) throw FDR_INVALID_PACK;
            int* pleft = (int*)(read + 1);
            pair.first = *pleft;
            read += 5; // 跳过 hash 里出现的 0
            pair.second = read;
            read += strlen(read) + 1;

            list.push_back(pair);
        }
        
        subf.first = sfinfo[i].nameHash;
        subf.second = list;
        _pack.push_back(subf);
    }

    return 0;
}



int textFinder::setfileh(const unsigned int fileNameHash, void * flag)
{

    return 0;
}

int textFinder::findh(const int leftHashIn, const char** rightTextOut, void * flag)
{

    return 0;
}


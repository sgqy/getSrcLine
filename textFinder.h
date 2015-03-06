

// author: wiki908
// crc32 by notwa@github


#pragma once

#include <cstring>
#include <map>
#include <vector>

#pragma pack(1)
struct HEADER
{
    char packageInfo[8];
    unsigned int CRCStarting;
    unsigned int CRCPolynomial;
    int headerLength;
    int fileCount;
};

struct FILEINFO
{
    unsigned int nameHash;
    int beginOffset;
    int fileLength;
};

//struct TEXTPAIR
//{
//    char* singlePair = "\x01leftHash\0\x02rightText\0";
//};

#pragma pack()

class textFinder
{
    int _history[6];

    // CRC32 Start
    typedef unsigned long ulong;
    enum { CRC_TABLE_SIZE = 0x100 };

    ulong crc_reflect(ulong input);
    void crc_fill_table(ulong *table, ulong polynomial, int big = 0);
    void crc_le_cycle(ulong *table, ulong *remainder, char c);

    ulong crc32(const char* str, const int len);

    ulong crc_table[CRC_TABLE_SIZE];
    ulong crc_starting;
    ulong crc_polynomial;

    // CRC32 End

public:

    int create(const char* buff, void* flag = 0);

    int setfile(const char* fileName, void* flag = 0);
    int setfileh(const unsigned int fileNameHash, void* flag = 0);

    // rightTextOut == 0 时, 返回 rightOut 的长度
    // rightTextOut != 0 时, 返回 0 成功, 返回负数失败
    // leftTextIn 应为字符串, leftHashIn 应为文本的 CRC32
    int find(const char* leftTextIn, char* rightTextOut = 0, void* flag = 0);
    int findh(const int leftHashIn, char* rightTextOut = 0, void* flag = 0);
};

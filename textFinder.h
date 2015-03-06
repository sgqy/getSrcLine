

// author: wiki908

#pragma once

#include <map>
#include <vector>

#pragma pack(1)
struct HEADER
{
    char packageInfo[8];
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
    char _history[8][6];

    // CRC32 Begin
    unsigned int POLYNOMIAL = 0xEDB88320;
    unsigned int crc_table[256];
    void make_table()
    {
        int i, j, crc;
        for (i = 0; i < 256; i++)
            for (j = 0, crc_table[i] = i; j < 8; j++)
                crc_table[i] = (crc_table[i] >> 1) ^ ((crc_table[i] & 1) ? POLYNOMIAL : 0);
    }
    unsigned int crc32(char *buff, int len, unsigned int crc = 0)
    {
        crc = ~crc;
        for (int i = 0; i < len; i++)
            crc = (crc >> 8) ^ crc_table[(crc ^ buff[i]) & 0xff];
        return ~crc;
    }
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


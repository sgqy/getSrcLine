

// author: wiki908
// crc32 by notwa@github


#pragma once

#include <utility>
#include <list>

#define FDR_PACK_INFO 3077401        // ���嵱ǰ�ӵ�Ψһ��ʶ
#define FDR_INVALID_PACK (0x50) // �ļ���������ʽ�������ӵ�
#define FDR_LINE_ID (0x01)

#pragma pack(1)

// Header of package
struct FDR_HEADER
{
    int packageInfo;
    unsigned int CRCStarting;
    unsigned int CRCPolynomial;
    int fileCount;
};

// Information of sub-files
struct FDR_SFINFO
{
    unsigned int nameHash;
    int beginOffset; // FDR_SFINFO ����֮��Ϊ���
    int lineCount;
};

//struct TEXTPAIR
//{
//    CRC32: 2EBC00DF  Text:����������
//    char* singlePair = "\x01\xDF\x00\xBC\x2E����������\0";
//};

#pragma pack()

class textFinder
{
    int _history[6];
    typedef const char* rtext_t;
    typedef int ltexth_t;
    typedef int fnameh_t;
    typedef std::pair<ltexth_t, rtext_t> line_pair;
    typedef std::list<line_pair> line_list;
    typedef std::pair<fnameh_t, line_list> sub_file;
    typedef std::list<sub_file> fdr_pack;

    fdr_pack _pack;

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

    // rightTextOut == 0 ʱ, ���� rightOut �ĳ���
    // rightTextOut != 0 ʱ, ���� 0 �ɹ�, ���ظ���ʧ��
    // leftTextIn ӦΪ�ַ���, leftHashIn ӦΪ�ı��� CRC32
    int find(const char* leftTextIn, const char** rightTextOut = 0, void* flag = 0);
    int findh(const int leftHashIn, const char** rightTextOut = 0, void* flag = 0);

};

template<class _T>
bool operator == (const std::pair<int, _T> &lhs, const int &rhs)
{
    return (lhs.first == rhs);
}


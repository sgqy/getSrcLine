

// author: wiki908
// crc32 by notwa@github


#pragma once

#include <utility>
#include <list>

#define FDR_PACK_INFO     (0x00432B43)   // ���嵱ǰ�ӵ�Ψһ��ʶ
#define FDR_E_PACK_TYPE   (0x50)         // �ļ���������ʽ�������ӵ�
#define FDR_E_ARG_TYPE    (0x51)         // ��������

#define FDR_W_NXF         (0x30)         // �Ҳ����ļ�
#define FDR_W_NXL         (0x31)         // �Ҳ����ı���

#define FDR_LINE_ID       (0x01)         // ÿ�п�ͷ�ı��
#define FDR_SUCC          (0x00)         // �����ɹ����

#define FDR_CRC_STARTING   (0xFFFFFFFF)  //
#define FDR_CRC_POLYNOMIAL (0x04C11DB7)  // CRC ��Ĭ��ϵ��

extern const char* szFDR_W_NXL;

#pragma pack(1)

// Header of package
struct FDR_HEADER
{
    unsigned long packageInfo;    // ʹ�� FDR_PACK_INFO ���ֲ�ͬ�Ŀ�
    unsigned long CRCStarting;    // FDR_CRC_STARTING
    unsigned long CRCPolynomial;  // FDR_CRC_POLYNOMIAL
    unsigned long fileCount;      // �ı��ļ�����
};

// Information of sub-files
struct FDR_SFINFO
{
    unsigned long nameHash;       // �ı��ļ����� CRC32
    unsigned long beginOffset;    // �� FDR_SFINFO ����֮��Ϊ���
    unsigned long lineCount;      // �ı�������
};

//struct TEXTPAIR                 // ÿ���ı��Ľṹ, ��ͷΪ FDR_LINE_ID
//{
//    CRC32: 2EBC00DF  Text:����������
//    char* singlePair = "\x01\xDF\x00\xBC\x2E����������\0";
//};

#pragma pack()

class textFinder
{
    typedef const char* rtext_t;
    typedef unsigned long ltexth_t;
    typedef unsigned long fnameh_t;
    typedef std::pair<ltexth_t, rtext_t> line_pair;
    typedef std::list<line_pair> line_list;
    typedef std::pair<fnameh_t, line_list> sub_file;
    typedef std::list<sub_file> fdr_pack;

    fdr_pack _pack;
    sub_file _curfile;
    unsigned long _curfhash;

    //line_pair _pre[2];
    //line_list::iterator _next_it;

    // CRC32 Start
    typedef unsigned long ulong;
    enum { CRC_TABLE_SIZE = 0x100 };

    ulong crc_reflect(ulong input);
    void crc_fill_table(ulong *table, ulong polynomial, long big = 0);
    void crc_le_cycle(ulong *table, ulong *remainder, char c);
public:
    ulong crc32(const char* str, const long len);
private:
    ulong crc_table[CRC_TABLE_SIZE];
    ulong crc_starting;
    ulong crc_polynomial;

    // CRC32 End


public:


    long create(const char* buff, void* flag = 0);

    long setfile(const char* fileName, void* flag = 0);
    long setfileh(const unsigned long fileNameHash, void* flag = 0);

    // rightTextOut ֱ�Ӹ��� buff �е�����
    long find(const char* leftTextIn, const char** rightTextOut, void* flag = 0);
    long findh(const long leftHashIn, const char** rightTextOut, void* flag = 0);

};

template<class _T>
bool operator == (const std::pair<unsigned long, _T> &lhs, const unsigned long &rhs)
{
    return (lhs.first == rhs);
}

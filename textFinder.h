

// author: wiki908
// crc32 by notwa@github


#pragma once

#include <utility>
#include <list>

#define FDR_PACK_INFO     3077401        // ���嵱ǰ�ӵ�Ψһ��ʶ
#define FDR_E_PACK_TYPE   (0x50)         // �ļ���������ʽ�������ӵ�
#define FDR_E_ARG_TYPE    (0x51)         // ��������

#define FDR_W_NXF         (0x30)         // �Ҳ����ļ�
#define FDR_W_NXL         (0x31)         // �Ҳ����ı���

#define FDR_LINE_ID       (0x01)
#define FDR_SUCC          (0x00)         // �����ɹ����

#pragma pack(1)

// Header of package
struct FDR_HEADER
{
    unsigned int packageInfo;
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
    typedef const char* rtext_t;
    typedef unsigned int ltexth_t;
    typedef unsigned int fnameh_t;
    typedef std::pair<ltexth_t, rtext_t> line_pair;
    typedef std::list<line_pair> line_list;
    typedef std::pair<fnameh_t, line_list> sub_file;
    typedef std::list<sub_file> fdr_pack;

    fdr_pack _pack;
    sub_file _curfile;

    class preread_t
    {
    public:
        enum { history_size = 6 };
    private:
        line_pair _rec[history_size];
        int _idx;
    public:
        void push(const line_pair& p)
        {
            _rec[_idx++ % history_size] = p;
        }
        line_pair& operator[](const int& idx)
        {
            return _rec[idx % history_size];
        }
        const line_pair* inner()
        {
            return _rec;
        }
    } _preread;
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

    // rightTextOut ֱ�Ӹ��� buff �е�����
    int find(const char* leftTextIn, const char** rightTextOut, void* flag = 0);
    int findh(const int leftHashIn, const char** rightTextOut, void* flag = 0);

};

template<class _T>
bool operator == (const std::pair<unsigned int, _T> &lhs, const unsigned int &rhs)
{
    return (lhs.first == rhs);
}

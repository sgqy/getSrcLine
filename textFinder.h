

// author: wiki908
// crc32 by notwa@github


#pragma once

#include <utility>
#include <list>

#define FDR_PACK_INFO     3077401        // 協吶輝念甚議率匯炎紛
#define FDR_E_PACK_TYPE   (0x50)         // 猟周頁凪麿鯉塀賜凪麿甚議
#define FDR_E_ARG_TYPE    (0x51)         // 歌方危列

#define FDR_W_NXF         (0x30)         // 孀音欺猟周
#define FDR_W_NXL         (0x31)         // 孀音欺猟云佩

#define FDR_LINE_ID       (0x01)         // 耽佩蝕遊議炎芝
#define FDR_SUCC          (0x00)         // 荷恬撹孔頼撹

#define FDR_CRC_STARTING   (0xFFFFFFFF)  //
#define FDR_CRC_POLYNOMIAL (0x04C11DB7)  // CRC 議潮範狼方

#pragma pack(1)

// Header of package
struct FDR_HEADER
{
    unsigned long packageInfo;    // 聞喘 FDR_PACK_INFO 曝蛍音揖議甚
    unsigned long CRCStarting;    // FDR_CRC_STARTING
    unsigned long CRCPolynomial;  // FDR_CRC_POLYNOMIAL
    unsigned long fileCount;      // 猟云猟周方楚
};

// Information of sub-files
struct FDR_SFINFO
{
    unsigned long nameHash;       // 猟云猟周兆議 CRC32
    unsigned long beginOffset;    // 參 FDR_SFINFO 會双岻朔葎軟泣
    unsigned long lineCount;      // 猟云坪佩方
};

//struct TEXTPAIR                 // 耽佩猟云議潤更, 蝕遊葎 FDR_LINE_ID
//{
//    CRC32: 2EBC00DF  Text:あいうえお
//    char* singlePair = "\x01\xDF\x00\xBC\x2Eあいうえお\0";
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

    class preread_t
    {
    public:
        enum { history_size = 6 };
    private:
        line_pair _rec[history_size];
        long _idx;
    public:
        void push(const line_pair& p)
        {
            _rec[_idx++ % history_size] = p;
        }
        line_pair& operator[](const long& idx)
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
    void crc_fill_table(ulong *table, ulong polynomial, long big = 0);
    void crc_le_cycle(ulong *table, ulong *remainder, char c);

    ulong crc32(const char* str, const long len);

    ulong crc_table[CRC_TABLE_SIZE];
    ulong crc_starting;
    ulong crc_polynomial;

    // CRC32 End


public:


    long create(const char* buff, void* flag = 0);

    long setfile(const char* fileName, void* flag = 0);
    long setfileh(const unsigned long fileNameHash, void* flag = 0);

    // rightTextOut 岷俊公竃 buff 嶄議坪否
    long find(const char* leftTextIn, const char** rightTextOut, void* flag = 0);
    long findh(const long leftHashIn, const char** rightTextOut, void* flag = 0);

};

template<class _T>
bool operator == (const std::pair<unsigned long, _T> &lhs, const unsigned long &rhs)
{
    return (lhs.first == rhs);
}

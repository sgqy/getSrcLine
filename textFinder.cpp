

// author: wiki908
// crc32 by notwa@github


#include <string.h>

#include <utility>
#include <list>

#include "textFinder.h"


long textFinder::setfile(const char * fileName, void * flag)
{
    ulong fnHash = 0;
    if (fileName != 0) fnHash = crc.Calc(fileName, strlen(fileName));
    //printf("f: %3d %08X %s\n", strlen(fileName), fnHash, fileName);
    return setfileh(fnHash, flag);
}
long textFinder::find(const char * leftTextIn, const char** rightTextOut, void * flag)
{
    ulong currHash = 0;
    if (leftTextIn != 0) currHash = crc.Calc(leftTextIn, strlen(leftTextIn));
    //printf("l: %3d %08X\n", strlen(leftTextIn), currHash);
    return findh(currHash, rightTextOut, flag);
}


long textFinder::create(const char * buff, void * flag)
{
    FDR_HEADER* phdr = (FDR_HEADER*)buff;

    // check package type
    // printf("phdr: %08X\n", phdr->packageInfo);
    if (phdr->packageInfo != FDR_PACK_INFO) throw FDR_E_PACK_TYPE;

    // create crc32 calculator
    crc.Init(0, 0, 0, phdr->CRCStarting, phdr->CRCPolynomial);

    // record text info
    FDR_SFINFO* sfinfo = (FDR_SFINFO*)(phdr + 1);
    const char* startPolong = (const char*)(sfinfo + phdr->fileCount);

    sub_file subf;
    for (ulong i = 0; i < phdr->fileCount; ++i)
    {
        line_pair pair;
        line_list list;

        const char* read = startPolong + sfinfo[i].beginOffset;
        for (ulong j = 0; j < sfinfo[i].lineCount; ++j)
        {
            if (*read != FDR_LINE_ID) throw FDR_E_PACK_TYPE;
            ltexth_t* pleft = (ltexth_t*)(read + 1);
            pair.first = *pleft;

            //printf("%08X\n", *pleft);

            read += 5; // 跳过 hash 里出现的 0
            pair.second = read;
            read += strlen(read) + 1;

            list.push_back(pair);
        }

        subf.first = sfinfo[i].nameHash;
        subf.second = list;
        _pack.push_back(subf);
    }

    // _curfile = _pack.begin(); // 初始化文件检索列表
    return FDR_SUCC;
}



long textFinder::setfileh(const unsigned long fileNameHash, void * flag)
{
    if (fileNameHash == _curfhash) return FDR_SUCC;
    for (auto s : _pack)
    {
        if (s == fileNameHash)
        {
            _curfile = s;
            return FDR_SUCC;
        }
    }
    return FDR_W_NXF;
}

const char* szFDR_W_NXL = "Non-existing Line.";

long textFinder::findh(const long leftHashIn, const char** rightTextOut, void * flag)
{
    if (rightTextOut == 0) throw FDR_E_ARG_TYPE;

    auto s = _curfile.second;


    //if (_pre[0].first == leftHashIn)
    //{
    //    *rightTextOut = _pre[0].second;
    //    return FDR_SUCC;
    //}
    //else if (_pre[1].first == leftHashIn) // 在 _pre[0] 处拦截
    //{
    //    _pre[0] = _pre[1];
    //    *rightTextOut = _pre[0].second;

    //    ++_next_it;
    //    if(_next_it != s.end())
    //        _pre[1] = *_next_it;
    //    return FDR_SUCC;
    //}
    //else
    //{
    for (auto _next_it = s.begin(); _next_it != s.end(); ++_next_it)
    {
        if (*_next_it == leftHashIn)
        {
            *rightTextOut = _next_it->second;
            //_pre[0] = *_next_it++;
            //if (_next_it != s.end())
            //    _pre[1] = *_next_it;
            return FDR_SUCC;
        }
    }

    // setfile() 失败的解决方案
    for (auto f : _pack)
    {
        auto lst = f.second;
        for (auto L : lst)
        {
            if (L == leftHashIn)
            {
                *rightTextOut = L.second;
                return FDR_SUCC;
            }
        }
    }

    // 这里真没有╮(╯▽╰)╭
    *rightTextOut = szFDR_W_NXL;
    return FDR_W_NXL;

    //}
    //return FDR_W_NXL;

}


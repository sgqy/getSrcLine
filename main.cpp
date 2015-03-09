

#if 1

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include "textFinder.h"


int wmain(int argc, wchar_t** argv)
{
    //_setmode(_fileno(stdout), _O_U16TEXT);

    try {

        if (argc != 2) return 0;

        FILE* fp = 0;
        fp = _wfopen(argv[1], L"rb");
        if (!fp) return 1;

        fseek(fp, 0, SEEK_END);
        long fsiz = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char* buff = new char[fsiz];
        fread(buff, 1, fsiz, fp);

        ///////////////////////////////////////////
        LARGE_INTEGER qFreq; // 微秒级计时器
        QueryPerformanceFrequency(&qFreq);
        LARGE_INTEGER qS, qE;

        textFinder tf;

        QueryPerformanceCounter(&qS);
        tf.create(buff); // 导入数据
        QueryPerformanceCounter(&qE);

        //printf("%08X\n", tf.crc32("CCA0001", 7)); // public 状态的 crc32() 可用
        printf("[+]Creating %.3lf ms\n", (qE.QuadPart - qS.QuadPart) / (double)(qFreq.QuadPart / 1000));


        // 设定要搜索的文件
        char input[2000];
        printf("[?]File to search:");
        scanf("%s", input);

        int rst = tf.setfile(input);
        printf("[+]File return 0x%x\n", rst);

        const char* output = 0;
        wchar_t conv[2000];
        char jat[2000];



        while (1)
        {
            // 搜索文本里的行
            printf("[?]Input query (q to exit):");
            scanf("%s", input);
            if (input[0] == 'q') break;

            QueryPerformanceCounter(&qS);
            rst = tf.find(input, &output);
            QueryPerformanceCounter(&qE);
            printf("[+]Finding %.3lf ms\n", (qE.QuadPart - qS.QuadPart) / (double)(qFreq.QuadPart / 1000));
            printf("[+]Finding return 0x%x\n", rst);


            // 932 - 936 转换, 用于 936 控制台输出
            int convcnt = MultiByteToWideChar(932, 0, output, -1, 0, 0);
            MultiByteToWideChar(932, 0, output, -1, conv, convcnt);
            convcnt = WideCharToMultiByte(936, 0, conv, -1, 0, 0, 0, 0);
            WideCharToMultiByte(936, 0, conv, -1, jat, convcnt, 0, 0);

            printf("[+]Finding result %s\n", jat);
        }

        ///////////////////////////////////////////

        delete[] buff;
    }
    catch (int& e)
    {
        printf("[-]exception: %x\n", e);
    }
    catch (...)
    {
        printf("[-]other exception\n");
    }

    return 0;
}

#endif


#if 0

#include <stdio.h>
#include "crc32.h"

int main()
{
    CRC32 crc;
    crc.Init(0);
    printf("%08X\n", crc.Calc("CCA0001", 7));
    return 0;
}

#endif

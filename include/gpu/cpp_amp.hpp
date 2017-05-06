#ifndef _CPP_AMP_HPP

#define _CPP_AMP_HPP
#define NOMINMAX


#if USE_GPU
#include <amp.h>
#include <cmath>
#include <amp_math.h>
using namespace concurrency;
#else
#include <Windows.h>
#endif

#include <string>
#include <vector>

inline std::string WStringToString
(
    std::wstring oWString
)
{
    // wstring → SJIS
    int iBufferSize = WideCharToMultiByte( CP_OEMCP, 0, oWString.c_str()
        , -1, (char *)NULL, 0, NULL, NULL );
 
    // バッファの取得
    CHAR* cpMultiByte = new CHAR[ iBufferSize ];
 
    // wstring → SJIS
    WideCharToMultiByte( CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
        , iBufferSize, NULL, NULL );
 
    // stringの生成
    std::string oRet( cpMultiByte, cpMultiByte + iBufferSize - 1 );
 
    // バッファの破棄
    delete [] cpMultiByte;
 
    // 変換結果を返す
    return( oRet );
}

inline void gpuInfo(const char* acceleratorInfo)
{
#if USE_GPU
	FILE* fp = fopen(acceleratorInfo, "w");
	if (fp == NULL) return;

	std::vector<accelerator> accs = accelerator::get_all();
	for (int i = 0; i < accs.size(); i++) 
	{		
		fprintf(fp, "[%d]description:%s\n", i, WStringToString(accs[i].description).c_str());
		fprintf(fp, "[%d]device_path:%s\n", i, WStringToString(accs[i].device_path).c_str());
		fprintf(fp, "[%d]dedicated_memory:%d\n", i, accs[i].dedicated_memory);
		fprintf(fp, "[%d]supports_double_precision:%s\n\n",
			i, accs[i].supports_double_precision ? "true" : "false");
	}

	accelerator default_acc;
	fprintf(fp, "-------------------- default accelerator -------------------------\n");
	fprintf(fp, "default_acc.description:%s\n", WStringToString(default_acc.description).c_str());
	fprintf(fp, "default_acc.device_path:%s\n", WStringToString(default_acc.device_path).c_str());
	fprintf(fp, "default_acc.dedicated_memory:%d\n", default_acc.dedicated_memory);
	fprintf(fp, "default_acc.supports_double_precision:%s\n\n",
		default_acc.supports_double_precision ? "true" : "false");

	fclose(fp);
#endif
}

inline void copy_array(const double* v, const int size, std::vector<float>& va)
{
	va.resize(size);
	for ( int i = 0; i < size; ++i )
	{
		va[i] = v[i];
	}
}
inline void copy_array(const float* v, const int size, std::vector<float>& va)
{
	va.resize(size);
	for (int i = 0; i < size; ++i)
	{
		va[i] = v[i];
	}
}
#endif
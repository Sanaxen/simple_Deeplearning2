
#ifndef _SIMPLE_DN_DLL_HPP

#define _SIMPLE_DN_DLL_HPP

#include <vector>
#include <Windows.h>

#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <cmath>
#include <random>
#include <numeric>

typedef std::vector<std::vector<std::vector<double>>> tensor;

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* tensor_p;

/*
	ニューラルネットワークの生成
	ニューラルネットワークオブジェクトへのポインターを返す
*/
typedef void*(WINAPI *dn_CreateNET)();

/*
	ニューラルネットワークの削除
*/
typedef void(WINAPI *dn_DeleteNET)(void* net_p);


/*
	ニューラルネットワークのハイパーパラメータ設定
	net_p = ニューラルネットワークオブジェクトへのポインター
	eps = 学習率(正の値）set_learning_rateで設定しなければ 0.001
*/
typedef void(WINAPI *dn_set_learning_rate)(const double eps, void* net_p);



/*
	ニューラルネットワークのハイパーパラメータ設定
	net_p = ニューラルネットワークオブジェクトへのポインター
	lambda = 荷重減衰(0以上の値）set_learning_lambdaで設定しなければ 0.0
*/
typedef void(WINAPI *dn_set_learning_lambda)(const double lambda, void* net_p);



/*
	ニューラルネットワークのハイパーパラメータ設定
	net_p = ニューラルネットワークオブジェクトへのポインター
	batchsize = ミニバッチサイズ(1以上の値）set_learning_batchsizeで設定しなければならない。
*/
typedef void(WINAPI *dn_set_learning_batchsize)(const int batchsize, void* net_p);



/*
	ニューラルネットワークのハイパーパラメータ設定
	net_p = ニューラルネットワークオブジェクトへのポインター
	epoch = EPOCH数(1以上の値）set_learning_epochで設定しなければならない。
	※学習データを１順する時を1EPOCH
*/
typedef void(WINAPI *dn_set_learning_epoch)(const int epoch, void* net_p);



/*
	ニューラルネットワークの保存
	net_p = ニューラルネットワークオブジェクトへのポインター
	"NET.txt"　で保存される
*/
typedef void(WINAPI *dn_SaveNet)(void* net_p);



/*
	ニューラルネットワークの読み込み
	net_p = ニューラルネットワークオブジェクトへのポインター
	"NET.txt"　から読み込まれる
*/
typedef void(WINAPI *dn_LoadNet)(void* net_p);



/*
	レイヤーの読み込み
	レイヤーオブジェクトへのポインターを返す
	"LAYER.txt"　から読み込まれる
*/
typedef void*(WINAPI *dn_LoadLayer)();



/*
	ニューラルネットワークの構築
	net_p = ニューラルネットワークオブジェクトへのポインター
	layer_p = レイヤーオブジェクトへのポインター
*/
typedef void(WINAPI *dn_set_layers)( void* net_p, void* layer_p);



/*
	レイヤーの保存
	layer_p = レイヤーオブジェクトへのポインター
	"LAYER.txt"　に保存される
*/
typedef void(WINAPI *dn_SaveLayer)(void* layer_p);



/*
	レイヤーのウェイト保存
	net_p = ニューラルネットワークオブジェクトへのポインター
	"Learned.dat_layer_#"　に保存される
*/
typedef void(WINAPI *dn_save_weight)(void* net_p);



/*
	レイヤーのウェイトの読み込み
	net_p = ニューラルネットワークオブジェクトへのポインター
	"Learned.dat_layer_#"　から読み込まれる
*/
typedef void(WINAPI *dn_load_weight)(void* net_p);



/*
	深層学習
	X = 入力データ集
	Y = 出力データ集
	net_p = ニューラルネットワークオブジェクトへのポインター
*/
typedef void(WINAPI *dn_learning)(void* net_p, tensor& X, tensor& Y, int iter);



/*
	深層学習の結果による予測
	X = 入力データ集
	net_p = ニューラルネットワークオブジェクトへのポインター
*/
typedef tensor(WINAPI *dn_predict)(void* net_p, tensor& X);



#define DNN_DEF_FUNC(f)	dn_ ## f f ## _dn = NULL;
#define DNN_FUNC(f)	f ## _dn = (dn_ ## f)GetProcAddress(__hModule, # f);if ( f ## _dn == NULL ) printf("load %s error.\n", #f);

HMODULE __hModule = NULL;
DNN_DEF_FUNC(CreateNET);
DNN_DEF_FUNC(DeleteNET);
DNN_DEF_FUNC(set_learning_rate);
DNN_DEF_FUNC(set_learning_lambda);
DNN_DEF_FUNC(set_learning_batchsize);
DNN_DEF_FUNC(set_learning_epoch);
DNN_DEF_FUNC(SaveNet);
DNN_DEF_FUNC(LoadNet);
DNN_DEF_FUNC(LoadLayer);
DNN_DEF_FUNC(set_layers);
DNN_DEF_FUNC(SaveLayer);
DNN_DEF_FUNC(save_weight);
DNN_DEF_FUNC(load_weight);
DNN_DEF_FUNC(learning);
DNN_DEF_FUNC(predict);

inline int simple_dnn_init(const char* this_dll)
{
 // DLLのロード
  __hModule = LoadLibraryA(this_dll);
  if (__hModule == NULL)
  {
    printf("%s", "DLLのロードに失敗しました。");
    return -1;
  }

  //// 関数のアドレス取得
  //dn_CreateNET CreateNET = (dn_CreateNET)GetProcAddress(__hModule, "CreateNET");
  //if (CreateNET == NULL)
  //{
  //  printf("%s", "関数のアドレス取得に失敗しました。");
  //  FreeLibrary(__hModule);
  //  return 0;
  //}

  DNN_FUNC(CreateNET);
  DNN_FUNC(DeleteNET);
  DNN_FUNC(set_learning_rate);
  DNN_FUNC(set_learning_lambda);
  DNN_FUNC(set_learning_batchsize);
  DNN_FUNC(set_learning_epoch);
  DNN_FUNC(SaveNet);
  DNN_FUNC(LoadNet);
  DNN_FUNC(LoadLayer);
  DNN_FUNC(set_layers);
  DNN_FUNC(SaveLayer);
  DNN_FUNC(save_weight);
  DNN_FUNC(load_weight);
  DNN_FUNC(learning);
  DNN_FUNC(predict);
  return 0;
}
inline void simple_dnn_term()
{
	FreeLibrary(__hModule);
}

#ifdef __cplusplus
};
#endif

#endif

/*
MIT License

Copyright (c) 2017 Sanaxen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*/
#ifndef _SIMPLE_DN_DLL_HPP

#define _SIMPLE_DN_DLL_HPP

#include <vector>
#define NOMINMAX

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

#ifdef USE_FLOAT
typedef float dnn_double;
#else
typedef double dnn_double;
#endif

typedef std::vector<std::vector<std::vector<dnn_double>>> tensor;

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* tensor_p;
typedef double(*loss_function_t)(dnn_double* mat1, int m1, int n1, dnn_double* mat2, int m2, int n2);

/*
	Generation of neural network
	Returns a pointer to a neural network object
*/
typedef void*(WINAPI *dn_CreateNET)();

/*
	Delete neural network
*/
typedef void(WINAPI *dn_DeleteNET)(void* net_p);


/*
	Hyper parameter setting of neural network
	Net_p = Pointer to neural network object
	Eps = learning rate (positive value) 0.001 if you do not set it with set_learning_rate
*/
typedef void(WINAPI *dn_set_learning_rate)(const double eps, void* net_p);



/*
	Hyper parameter setting of neural network
	Net_p = Pointer to neural network object
	Lambda = load attenuation (value greater than or equal to 0) 0.0 if set_learning_lambda is not used
*/
typedef void(WINAPI *dn_set_learning_lambda)(const double lambda, void* net_p);



/*
	Hyper parameter setting of neural network
	Net_p = Pointer to neural network object
	Batchsize = Mini batch size (1 or more value) Must be set with set_learning_batchsize.
*/
typedef void(WINAPI *dn_set_learning_batchsize)(const int batchsize, void* net_p);



/*
	Hyper parameter setting of neural network
	Net_p = Pointer to neural network object
	Epoch = EPOCH number (value of 1 or more) Must be set with set_learning_epoch.
	��Learning data 1 When going around 1 EPOCH
*/
typedef void(WINAPI *dn_set_learning_epoch)(const int epoch, void* net_p);



/*
	Preservation of neural network
	Net_p = Pointer to neural network object
	Saved with "NET.txt"
*/
typedef void(WINAPI *dn_SaveNet)(void* net_p);



/*
	Load neural network
	Net_p = Pointer to neural network object
	Read from "NET.txt"
*/
typedef void(WINAPI *dn_LoadNet)(void* net_p);



/*
	Load layer
	Return a pointer to a layer object
	Read from "LAYER.txt"
*/
typedef void*(WINAPI *dn_LoadLayer)();


/*
	Setting Test Data of Neural Network
	Net_p = Pointer to neural network object
	X = test data
	Y = test data label
	Read from "NET.txt"
*/
typedef void(WINAPI *dn_set_test_data)(void* net_p, tensor& X, tensor& Y);


/*
	Construction of neural network
	Net_p = Pointer to neural network object
	Layer_p = Pointer to layer object
*/
typedef void(WINAPI *dn_set_layers)( void* net_p, void* layer_p);



/*
	Save layer
	Layer_p = Pointer to layer object
	It is stored in "LAYER.txt"
*/
typedef void(WINAPI *dn_SaveLayer)(void* layer_p);



/*
	Save layer weight
	Net_p = Pointer to neural network object
	It is stored in filename
*/
typedef void(WINAPI *dn_save_weight)(void* net_p, std::string& filename);



/*
	Read weight of layer
	Net_p = Pointer to neural network object
	Read from filename
*/
typedef void(WINAPI *dn_load_weight)(void* net_p, std::string& filename);



/*
	Deep learning
	X = input data collection
	Y = output data collection
	Net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_learning)(void* net_p, tensor& X, tensor& Y, int iter);



/*
	Prediction based on the result of deep learning
	X = input data collection
	Net_p = Pointer to neural network object
*/
typedef tensor(WINAPI *dn_predict)(void* net_p, tensor& X);


/*
	Specification of multi-core calculation mode using thread
	thread_num = threads number
	Net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_enable_threads_mode)(void* net_p, int thread_num);

/*
Adjustment of automatic learning rate
net_p = Pointer to neural network object
c = Adjustment factor
*/
typedef void(WINAPI *dn_adjustment_learning_rate)(void* net_p, const double c);

/*
	Specified control for deriving the derivative of loss function by numerical differentiation
	net_p = Pointer to neural network object
	flag = 1 'ON'
	flag = 0 'OFF'
*/
typedef void(WINAPI *dn_set_loss_Numerical_differentiation)(void* net_p, int flag);

/*
	Definition of loss function
	net_p = Pointer to neural network object
	fnc = Function pointer

*/
typedef void(WINAPI *dn_set_Loss_user_function)(void* net_p, loss_function_t fnc);

	
/*
	Does cache data exist?
	net_p = Pointer to neural network object
*/
typedef int(WINAPI *dn_is_Cached_data_exists)(void* net_p);

/*
	Save the input learning data in the cache
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_create_cach_X)(void* net_p, int number, std::vector<std::vector<dnn_double>>& x);

/*
	Save the output learning data in the cache
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_create_cach_Y)(void* net_p, int number, std::vector<std::vector<dnn_double>>& x);

/*
	Save the input test data in the cache
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_create_cach_testX)(void* net_p, int number, std::vector<std::vector<dnn_double>>& x);

/*
	Save the output test data in the cache
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_create_cach_testY)(void* net_p, int number, std::vector<std::vector<dnn_double>>& x);

/*
	End of cache generation
	net_p = Pointer to neural network object
	X.size() == 0 ( and Y.size() == 0 ) --> The data is in the cache file and is not on the memory 
*/
typedef void(WINAPI *dn_create_cach_end)(void* net_p, tensor& X, tensor& Y);

/*
	Setting of total learning data number
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_setDataNum)(void* net_p, int data_num);

/*
	Setting of total test data number
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_setTestDataNum)(void* net_p, int data_num);


/*
	Forcibly set the number
	Do not use it for purposes other than limiting the amount of data.

	Setting of total learning data number
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_setDataNumEx)(void* net_p, int data_num);

/*
	Forcibly set the number
	Do not use it for purposes other than limiting the amount of data.

	Setting of total learning data number
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_setTestDataNumEx)(void* net_p, int data_num);


/*
	Reading the number of cached learning & test data
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_loadCachDataNum)(void* net_p);

/*
	Acquisition of the number of learning data
	net_p = Pointer to neural network object
*/
typedef int(WINAPI *dn_getCachDataNum)(void* net_p);

/*
	Acquisition of the number of test data
	net_p = Pointer to neural network object
*/
typedef int(WINAPI *dn_getCachTestDataNum)(void* net_p);


/*
	Read all cached training data
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_read_cach_data)(void* net_p, tensor& X, tensor& Y);

/*
	Read all cached test data
	net_p = Pointer to neural network object
*/
typedef void(WINAPI *dn_read_cach_test_data)(void* net_p, tensor& X, tensor& Y);



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
DNN_DEF_FUNC(enable_threads_mode);
DNN_DEF_FUNC(set_test_data);
DNN_DEF_FUNC(adjustment_learning_rate);
DNN_DEF_FUNC(set_loss_Numerical_differentiation);
DNN_DEF_FUNC(set_Loss_user_function);

DNN_DEF_FUNC(is_Cached_data_exists);
DNN_DEF_FUNC(create_cach_X);
DNN_DEF_FUNC(create_cach_Y);
DNN_DEF_FUNC(create_cach_testX);
DNN_DEF_FUNC(create_cach_testY);
DNN_DEF_FUNC(create_cach_end);
DNN_DEF_FUNC(setDataNum);
DNN_DEF_FUNC(setTestDataNum);
DNN_DEF_FUNC(setDataNumEx);
DNN_DEF_FUNC(setTestDataNumEx);
DNN_DEF_FUNC(loadCachDataNum);
DNN_DEF_FUNC(getCachDataNum);
DNN_DEF_FUNC(getCachTestDataNum);
DNN_DEF_FUNC(read_cach_data);
DNN_DEF_FUNC(read_cach_test_data);


inline int simple_dnn_init(const char* this_dll)
{
  __hModule = LoadLibraryA(this_dll);
  if (__hModule == NULL)
  {
    printf("%s", "Failed to load DLL.�B");
    return -1;
  }

  //// Obtain address of function
  //dn_CreateNET CreateNET = (dn_CreateNET)GetProcAddress(__hModule, "CreateNET");
  //if (CreateNET == NULL)
  //{
  //  printf("%s", "Failed to get address of function.");
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
  DNN_FUNC(enable_threads_mode);
  DNN_FUNC(set_test_data);
  DNN_FUNC(adjustment_learning_rate);
  DNN_FUNC(set_loss_Numerical_differentiation);
  DNN_FUNC(set_Loss_user_function);
	
	DNN_FUNC(is_Cached_data_exists);
	DNN_FUNC(create_cach_X);
	DNN_FUNC(create_cach_Y);
	DNN_FUNC(create_cach_testX);
	DNN_FUNC(create_cach_testY);
	DNN_FUNC(create_cach_end);
	DNN_FUNC(setDataNum);
	DNN_FUNC(setTestDataNum);
	DNN_FUNC(setDataNumEx);
	DNN_FUNC(setTestDataNumEx);
	DNN_FUNC(loadCachDataNum);
	DNN_FUNC(getCachDataNum);
	DNN_FUNC(getCachTestDataNum);
	DNN_FUNC(read_cach_data);
	DNN_FUNC(read_cach_test_data);
	
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

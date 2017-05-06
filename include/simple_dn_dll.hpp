
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
	�j���[�����l�b�g���[�N�̐���
	�j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[��Ԃ�
*/
typedef void*(WINAPI *dn_CreateNET)();

/*
	�j���[�����l�b�g���[�N�̍폜
*/
typedef void(WINAPI *dn_DeleteNET)(void* net_p);


/*
	�j���[�����l�b�g���[�N�̃n�C�p�[�p�����[�^�ݒ�
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	eps = �w�K��(���̒l�jset_learning_rate�Őݒ肵�Ȃ���� 0.001
*/
typedef void(WINAPI *dn_set_learning_rate)(const double eps, void* net_p);



/*
	�j���[�����l�b�g���[�N�̃n�C�p�[�p�����[�^�ݒ�
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	lambda = �׏d����(0�ȏ�̒l�jset_learning_lambda�Őݒ肵�Ȃ���� 0.0
*/
typedef void(WINAPI *dn_set_learning_lambda)(const double lambda, void* net_p);



/*
	�j���[�����l�b�g���[�N�̃n�C�p�[�p�����[�^�ݒ�
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	batchsize = �~�j�o�b�`�T�C�Y(1�ȏ�̒l�jset_learning_batchsize�Őݒ肵�Ȃ���΂Ȃ�Ȃ��B
*/
typedef void(WINAPI *dn_set_learning_batchsize)(const int batchsize, void* net_p);



/*
	�j���[�����l�b�g���[�N�̃n�C�p�[�p�����[�^�ݒ�
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	epoch = EPOCH��(1�ȏ�̒l�jset_learning_epoch�Őݒ肵�Ȃ���΂Ȃ�Ȃ��B
	���w�K�f�[�^���P�����鎞��1EPOCH
*/
typedef void(WINAPI *dn_set_learning_epoch)(const int epoch, void* net_p);



/*
	�j���[�����l�b�g���[�N�̕ۑ�
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	"NET.txt"�@�ŕۑ������
*/
typedef void(WINAPI *dn_SaveNet)(void* net_p);



/*
	�j���[�����l�b�g���[�N�̓ǂݍ���
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	"NET.txt"�@����ǂݍ��܂��
*/
typedef void(WINAPI *dn_LoadNet)(void* net_p);



/*
	���C���[�̓ǂݍ���
	���C���[�I�u�W�F�N�g�ւ̃|�C���^�[��Ԃ�
	"LAYER.txt"�@����ǂݍ��܂��
*/
typedef void*(WINAPI *dn_LoadLayer)();



/*
	�j���[�����l�b�g���[�N�̍\�z
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	layer_p = ���C���[�I�u�W�F�N�g�ւ̃|�C���^�[
*/
typedef void(WINAPI *dn_set_layers)( void* net_p, void* layer_p);



/*
	���C���[�̕ۑ�
	layer_p = ���C���[�I�u�W�F�N�g�ւ̃|�C���^�[
	"LAYER.txt"�@�ɕۑ������
*/
typedef void(WINAPI *dn_SaveLayer)(void* layer_p);



/*
	���C���[�̃E�F�C�g�ۑ�
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	"Learned.dat_layer_#"�@�ɕۑ������
*/
typedef void(WINAPI *dn_save_weight)(void* net_p);



/*
	���C���[�̃E�F�C�g�̓ǂݍ���
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
	"Learned.dat_layer_#"�@����ǂݍ��܂��
*/
typedef void(WINAPI *dn_load_weight)(void* net_p);



/*
	�[�w�w�K
	X = ���̓f�[�^�W
	Y = �o�̓f�[�^�W
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
*/
typedef void(WINAPI *dn_learning)(void* net_p, tensor& X, tensor& Y, int iter);



/*
	�[�w�w�K�̌��ʂɂ��\��
	X = ���̓f�[�^�W
	net_p = �j���[�����l�b�g���[�N�I�u�W�F�N�g�ւ̃|�C���^�[
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
 // DLL�̃��[�h
  __hModule = LoadLibraryA(this_dll);
  if (__hModule == NULL)
  {
    printf("%s", "DLL�̃��[�h�Ɏ��s���܂����B");
    return -1;
  }

  //// �֐��̃A�h���X�擾
  //dn_CreateNET CreateNET = (dn_CreateNET)GetProcAddress(__hModule, "CreateNET");
  //if (CreateNET == NULL)
  //{
  //  printf("%s", "�֐��̃A�h���X�擾�Ɏ��s���܂����B");
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

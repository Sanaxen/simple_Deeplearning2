//#define USE_GPU	10
//#include "../include/gpu/cpp_amp.hpp"
#define NOMINMAX

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>

#include "../include/util/filecheck.hpp"
#include "../include/simple_dn_dll.hpp"
#include "mnist_image.hpp"

int main(int argc, char** argv)
{
	//simple �f�B�[�v���[�j���O
	simple_dnn_init("simple_dnn.dll");

	FileExitsCheck filechk;

	//�f�[�^�f�B���N�g��
	std::string dataDir;
	
	if ( argc == 2 ) dataDir = argv[1];
	else dataDir = "..\\..\\..\\dataset\\mnist";	//default!!

	//�摜�t�@�C�����W�J����Ă��邩
	if (!filechk.isExist(dataDir + "\\train\\*.bmp"))
	{
		//�摜�t�@�C�����W�J����Ă��Ȃ���ΓW�J����
		printf("%s\n", (dataDir + "\\*.bmp").c_str());
		printf("create image file.\n");
		LoadMinist(dataDir);
	}

	tensor im;		//�g���[�j���O�摜
	tensor label;	//���x��

	tensor test_im;		//Test�摜
	tensor test_label;	//���x��
					
	//�g���[�j���O�摜�E���x����ǂݍ���
	image_to_tensor(dataDir, im, label, test_im, test_label);

	//�l�b�g���[�N�̐���
	void* net = CreateNET_dn();
	LoadNet_dn(net);

	//���C���[�̐���
	void* layer = LoadLayer_dn();

	//�l�b�g���[�N�Ƀ��C���[��ݒ肷��
	set_layers_dn(net, layer);


	adjustment_learning_rate_dn(net, 0.001);

	SaveLayer_dn(layer);
	SaveNet_dn(net);


	//�܂��w�K���I����Ă��Ȃ����H
	if (!PathFileExistsA("mnist.model"))
	{
		set_test_data_dn(net, test_im, test_label);

		//�w�K
		learning_dn(net, im, label, -1);
		fprintf(stderr, "===== learning END =====\n"); fflush(stderr);
		
		//�w�K���ʂ�ۑ�
		save_weight_dn(net, std::string("mnist.model"));
	}else
	{
		//�w�K���ʂ�ǂݍ���
		load_weight_dn(net, std::string("mnist.model"));
		fprintf(stderr, "===== Read learned data =====\n");

		if (argc == 3)
		{
			//Test�摜��ǂݍ���
			tensor input;
			int lb = get_image_and_label( argv[2], input);

			//�摜��^���ă��x����\�������Ă݂�
			auto label_test = predict_dn(net, input);

			//��ԉ\���̍������x���͂Ȃ񂾂����H
			int max_i = -1;
			float max_f = -100000000.0;

			float sum = 0.0;
			for (int k = 0; k < label_test[0][0].size(); k++)
			{
				sum += label_test[0][0][k];
			}
			for (int k = 0; k < label_test[0][0].size(); k++)
			{
				label_test[0][0][k] = label_test[0][0][k] / sum;
			}

			for (int k = 0; k < label_test[0][0].size(); k++)
			{
				if (label_test[0][0][k] > max_f)
				{
					max_f = label_test[0][0][k];
					max_i = k;
				}
			}

			//�\�����ʂ���������
			FILE* fp = fopen("answer.txt", "w");
			fprintf(fp, "=======================================\n");
			fprintf(fp, "%d (%f)%%\n", max_i, max_f*100.0);

			if ( max_i != lb )
			{
				//�\���������ʂƎ��ۂ̃��x������v
				fprintf(fp, "The answer is %.4f%% '%d', but really '%d'\n", max_f*100.0, max_i, lb);
			}else
			{
				//�\���������ʂƎ��ۂ̃��x�����s��v
				fprintf(fp, "The answer is correct at %.4f%% '%d'\n", max_f*100.0, max_i);
			}
			fprintf(fp, "=======================================\n");
			
			//���̑��̊m�x���o��
			for (int k = 0; k < label_test[0][0].size(); k++)
			{
				fprintf(fp, "%d (%f)%%\n", k, label_test[0][0][k] *100.0);
			}
			fclose(fp);
			return 0;
		}
	}


all_check:;

	if (1)
	{
		//�S�Ẵe�X�g�摜��]�����Ă݂�
		const int N = 1000;
		tensor input;

		FILE* fp = fopen("success_rate.txt", "w");
		int mm = 0;
		float ok = 0;
		float ng = 0;

		FILE* fp2 = fopen((dataDir + "\\test_files.txt").c_str(), "r");

		char file[256];
		for (int i = 0; i < N; ++i)
		{
			if (!fgets(file, 256, fp2))
			{
				break;
			}
			file[strlen(file) - 1] = '\0';

			int lb = get_image_and_label( file, input);

			auto label_test = predict_dn(net, input);
			int max_i = -1;
			float max_f = -100000000.0;

			for (int k = 0; k < label_test[0][0].size(); k++)
			{
				if (label_test[0][0][k] > max_f)
				{
					max_f = label_test[0][0][k];
					max_i = k;
				}
			}
			if (lb == max_i) ok++;
			else ng++;
			if ( i % 20 == 0 ) fprintf(stderr, "%d %f%%\n", i, 100.0*ok / (float)(i+1));
		}
		fprintf(fp, "%f%%\n", 100.0*ok / (float)(N));
		fclose(fp);
	}

	simple_dnn_term();
}

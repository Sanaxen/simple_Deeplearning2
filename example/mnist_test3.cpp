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
	//simple ディープラーニング
	simple_dnn_init("simple_dnn.dll");

	FileExitsCheck filechk;

	//データディレクトリ
	std::string dataDir;
	
	if ( argc == 2 ) dataDir = argv[1];
	else dataDir = "..\\..\\..\\dataset\\mnist";	//default!!

	//画像ファイルが展開されているか
	if (!filechk.isExist(dataDir + "\\train\\*.bmp"))
	{
		//画像ファイルが展開されていなければ展開する
		printf("%s\n", (dataDir + "\\*.bmp").c_str());
		printf("create image file.\n");
		LoadMinist(dataDir);
	}

	tensor im;		//トレーニング画像
	tensor label;	//ラベル

	tensor test_im;		//Test画像
	tensor test_label;	//ラベル
					
	//トレーニング画像・ラベルを読み込む
	image_to_tensor(dataDir, im, label, test_im, test_label);

	//ネットワークの生成
	void* net = CreateNET_dn();
	LoadNet_dn(net);

	//レイヤーの生成
	void* layer = LoadLayer_dn();

	//ネットワークにレイヤーを設定する
	set_layers_dn(net, layer);


	adjustment_learning_rate_dn(net, 0.001);

	SaveLayer_dn(layer);
	SaveNet_dn(net);


	//まだ学習が終わっていないか？
	if (!PathFileExistsA("mnist.model"))
	{
		set_test_data_dn(net, test_im, test_label);

		//学習
		learning_dn(net, im, label, -1);
		fprintf(stderr, "===== learning END =====\n"); fflush(stderr);
		
		//学習結果を保存
		save_weight_dn(net, std::string("mnist.model"));
	}else
	{
		//学習結果を読み込む
		load_weight_dn(net, std::string("mnist.model"));
		fprintf(stderr, "===== Read learned data =====\n");

		if (argc == 3)
		{
			//Test画像を読み込む
			tensor input;
			int lb = get_image_and_label( argv[2], input);

			//画像を与えてラベルを予測させてみる
			auto label_test = predict_dn(net, input);

			//一番可能性の高いラベルはなんだっか？
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

			//予測結果を書き込む
			FILE* fp = fopen("answer.txt", "w");
			fprintf(fp, "=======================================\n");
			fprintf(fp, "%d (%f)%%\n", max_i, max_f*100.0);

			if ( max_i != lb )
			{
				//予測した結果と実際のラベルが一致
				fprintf(fp, "The answer is %.4f%% '%d', but really '%d'\n", max_f*100.0, max_i, lb);
			}else
			{
				//予測した結果と実際のラベルが不一致
				fprintf(fp, "The answer is correct at %.4f%% '%d'\n", max_f*100.0, max_i);
			}
			fprintf(fp, "=======================================\n");
			
			//その他の確度も出力
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
		//全てのテスト画像を評価してみる
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

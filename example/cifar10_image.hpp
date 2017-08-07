#ifndef _CIFAR10_IMAGE_HPP
#define _CIFAR10_IMAGE_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <random>

#include "../include/util/Image.hpp"

int numAugmentation = 0;
void LoadCifar10_0(std::string dataDir, FILE* fp, int trainFlg, int& cnt, std::mt19937& mt, std::uniform_real_distribution<double>& rnd, std::vector<std::string>& train, std::vector<std::string>& test);
void LoadCifar10(std::string dataDir)
{
	std::mt19937 mt;
	std::uniform_real_distribution<double> d_rand;
	d_rand = std::uniform_real_distribution<double>(0.0, 1.0);

	std::vector<std::string> train;
	std::vector<std::string> test;

	int cnt = 0;
	FILE* fp = fopen((dataDir + "\\data_batch_1.bin").c_str(), "rb");
	for (int i = 0; i < 10000; i++)
	{
		LoadCifar10_0(dataDir, fp, 1, cnt, mt, d_rand, train, test);
	}
	fclose(fp);

	fp = fopen((dataDir + "\\data_batch_2.bin").c_str(), "rb");
	for (int i = 0; i < 10000; i++)
	{
		LoadCifar10_0(dataDir, fp, 1, cnt, mt, d_rand, train, test);
	}
	fclose(fp);

	fp = fopen((dataDir + "\\data_batch_3.bin").c_str(), "rb");
	for (int i = 0; i < 10000; i++)
	{
		LoadCifar10_0(dataDir, fp, 1, cnt, mt, d_rand, train, test);
	}
	fclose(fp);

	fp = fopen((dataDir + "\\data_batch_4.bin").c_str(), "rb");
	for (int i = 0; i < 10000; i++)
	{
		LoadCifar10_0(dataDir, fp, 1, cnt, mt, d_rand, train, test);
	}
	fclose(fp);

	fp = fopen((dataDir + "\\data_batch_5.bin").c_str(), "rb");
	for (int i = 0; i < 10000; i++)
	{
		LoadCifar10_0(dataDir, fp, 1, cnt, mt, d_rand, train, test);
	}
	fclose(fp);

	fp = fopen((dataDir + "\\test_batch.bin").c_str(), "rb");
	for (int i = 0; i < 10000; i++)
	{
		LoadCifar10_0(dataDir, fp, 0, cnt, mt, d_rand, train, test);
	}
	fclose(fp);


	// シャッフル
	std::shuffle(train.begin(), train.end(), std::mt19937());
	std::shuffle(test.begin(), test.end(), std::mt19937());

	fp = fopen((dataDir + "\\train_files.txt").c_str(), "w");
	for (int i = 0; i < train.size(); i++)
	{
		fprintf(fp, "%s\n", train[i].c_str());
	}
	fclose(fp);
	fp = fopen((dataDir + "\\test_files.txt").c_str(), "w");
	for (int i = 0; i < test.size(); i++)
	{
		fprintf(fp, "%s\n", test[i].c_str());
	}
	fclose(fp);
}


void LoadCifar10_0(std::string dataDir, FILE* fp, int trainFlg, int& cnt, std::mt19937& mt, std::uniform_real_distribution<double>& d_rand, std::vector<std::string>& train, std::vector<std::string>& test)
{
	const char id[][32] =
	{
		"airplane",

		"automobile",

		"bird",

		"cat",

		"deer",

		"dog",

		"frog",

		"horse",

		"ship",

		"truck",
	};

	unsigned char buf[4096];
	fread((void*)buf, 1, 1 + 1024 + 1024 + 1024, fp);

	char label = buf[0];
	//printf("%s\n", id[label]);

	int x = 32, y = 32;
	int nbit = 3;

	unsigned char* image = ((unsigned char*)buf) + 1;
	unsigned char* R = image;
	unsigned char* G = R + 1024;
	unsigned char* B = G + 1024;

	unsigned char *data = new unsigned char[3 * x * y];
	for (int i = 0; i < x*y; i++) {
		data[i * 3 + 0] = R[i];
		data[i * 3 + 1] = G[i];
		data[i * 3 + 2] = B[i];
	}

	char filename[256];
	if (trainFlg)
	{
		sprintf(filename, "%s\\train\\%s_%03d.bmp", dataDir.c_str(), id[label], cnt);
		train.push_back(filename);
	}
	else
	{
		sprintf(filename, "%s\\test\\%s_%03d.bmp", dataDir.c_str(), id[label], cnt);
		test.push_back(filename);
	}
	stbi_write_bmp(filename, 32, 32, 3, (void*)data);
	cnt++;


	//訓練データの水増し
	if (trainFlg)
	{
		Augmentation aug(&mt, &d_rand);

		aug.gamma = 0.05;
		aug.rl = 0.3;
		aug.color_nize = 0.01;
		aug.rotation = 0.3;
		aug.rotation_max = 270;
		aug.sift = 0.3;
		aug.sift_max = 6;
		aug.rnd_noize = 0.01;

		std::vector<std::vector<unsigned char>> imageaug = ImageAugmentation(data, x, y, aug);

		for (int i = 0; i < imageaug.size(); i++)
		{
			sprintf(filename, "%s\\train_augment\\%s_%03d.bmp", dataDir.c_str(), id[label], cnt);
			train.push_back(filename);
			stbi_write_bmp(filename, 32, 32, 3, (void*)&(imageaug[i][0]));
			cnt++;
			numAugmentation++;
		}
	}
	delete[] data;
}


inline int filename_to_label(char* filename)
{
	const char id[][32] =
	{
		"airplane",

		"automobile",

		"bird",

		"cat",

		"deer",

		"dog",

		"frog",

		"horse",

		"ship",

		"truck",
	};
	int lb = -1;
	for (int k = 0; k < 10; k++)
	{
		if (strstr(filename, id[k]))
		{
			lb = k;
			break;
		}
	}
	return lb;
}


inline int image_to_tensor(std::string& dataDir, tensor& im, tensor& label, tensor& test_im, tensor& test_label)
{
	const int M = 60000;
	const int N = 10000;

	unsigned char buf[4096];

	bool Normalization = false;
	{
		FILE* fp = fopen("Normalization.txt", "r");
		if ( fp )
		{
			Normalization = true;
			fclose(fp);
		}
	}
	char file[256];
	if (!PathFileExistsA("cifar10.model"))
	{

		FILE* fp = fopen((dataDir+ "\\train_files.txt").c_str(), "r");
		if (!fp) return -1;

		for (int i = 0; i < M; ++i)
		{
			fprintf(stderr, "%d/%d           \r", (i + 1), M );
			if (!fgets(file, 256, fp))
			{
				break;
			}
			file[strlen(file) - 1] = '\0';

			Image* img = readImage(file);

			vector<vector<double>> tmp(3, vector<double>(img->height*img->width));
			vector<vector<double>> tmp_lb(1, vector<double>(10, 0));

			if ( Normalization )
			{
				int sz = img->height*img->width;
				double* whitening_img = image_whitening<double>(img);
#pragma omp parallel for
				for (int k = 0; k < sz; k++)
				{
					tmp[0][k] = whitening_img[3 * k + 0];
					tmp[1][k] = whitening_img[3 * k + 1];
					tmp[2][k] = whitening_img[3 * k + 2];
				}
				delete [] whitening_img;
			}else
			{
				int sz = img->height*img->width;
#pragma omp parallel for
				for (int k = 0; k < sz; k++)
				{
					tmp[0][k] = (double)img->data[k].r / 255.0;
					tmp[1][k] = (double)img->data[k].g / 255.0;
					tmp[2][k] = (double)img->data[k].b / 255.0;
				}
			}

			int lb = filename_to_label(file);
			if ( lb >= 0 && lb < 10 ) tmp_lb[0][lb] = 1.0;

			im.push_back(tmp);
			label.push_back(tmp_lb);

			delete img;
		}
		fclose(fp);

		fprintf(stderr, "total %d set\n", im.size());
		
		fp = fopen((dataDir + "\\test_files.txt").c_str(), "r");
		if (!fp) return -1;

		for (int i = 0; i < N; ++i)
		{
			fprintf(stderr, "%d/%d           \r", (i + 1), N);
			if (!fgets(file, 256, fp))
			{
				break;
			}
			file[strlen(file) - 1] = '\0';

			Image* img = readImage(file);

			vector<vector<double>> tmp(3, vector<double>(img->height*img->width));
			vector<vector<double>> tmp_lb(1, vector<double>(10, 0));

			if (Normalization)
			{
				int sz = img->height*img->width;
				double* whitening_img = image_whitening<double>(img);
#pragma omp parallel for
				for (int k = 0; k < sz; k++)
				{
					tmp[0][k] = whitening_img[3 * k + 0];
					tmp[1][k] = whitening_img[3 * k + 1];
					tmp[2][k] = whitening_img[3 * k + 2];
				}
				delete[] whitening_img;
			}
			else
			{
				int sz = img->height*img->width;
#pragma omp parallel for
				for (int k = 0; k < sz; k++)
				{
					tmp[0][k] = (double)img->data[k].r / 255.0;
					tmp[1][k] = (double)img->data[k].g / 255.0;
					tmp[2][k] = (double)img->data[k].b / 255.0;
				}
			}

			int lb = filename_to_label(file);
			if (lb >= 0 && lb < 10) tmp_lb[0][lb] = 1.0;

			test_im.push_back(tmp);
			test_label.push_back(tmp_lb);

			delete img;
		}
		fclose(fp);

		fprintf(stderr, "test total %d set\n", test_im.size());
	}
	return 0;
}

inline int get_image_and_label( char* imageFile, tensor& image)
{
	tensor& input = image;
	input.resize(1);
	input[0].resize(3);

	input[0][0].resize(32 * 32);
	input[0][1].resize(32 * 32);
	input[0][2].resize(32 * 32);

	Image* img = readImage(imageFile);
	if ( img == NULL ) return -1;

	int sz = img->height*img->width;
	double* whitening_img = image_whitening<double>(img);
#pragma omp parallel for
	for (int k = 0; k < sz; k++)
	{
		input[0][0][k] = whitening_img[3 * k + 0];
		input[0][1][k] = whitening_img[3 * k + 1];
		input[0][2][k] = whitening_img[3 * k + 2];
	}
	delete[] whitening_img;
	delete img;

	return filename_to_label(imageFile);
}


#endif
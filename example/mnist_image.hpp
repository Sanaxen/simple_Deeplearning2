#ifndef _MINIST_IMAGE_HPP
#define _MINIST_IMAGE_HPP

#include "../include/util/Image.hpp"

inline void LoadMinist(std::string dataDir)
{
	std::vector<std::string> train;
	std::vector<std::string> test;

	unsigned char tmp[28*28*3];

	// read a test data of MNIST(http://yann.lecun.com/exdb/mnist/).
	const int N = 6000;
	ifstream train_image(dataDir+"\\train-images.idx3-ubyte", ios_base::binary);
	if( !train_image.is_open() ){
		cerr << "\"train-images.idx3-ubyte\" is not found!" << endl;
		return ;
	}
	ifstream train_label(dataDir + "\\train-labels.idx1-ubyte", ios_base::binary);
	if( !train_label.is_open() ){
		cerr << "\"train-labels.idx1-ubyte\" is not found!" << endl;
		return;
	}

	train_image.seekg(4*4, ios_base::beg);
	train_label.seekg(4*2, ios_base::beg);
	for( int i = 0; i < N; ++i )
	{
		unsigned char tmp_lab;
		train_label.read((char*)&tmp_lab, sizeof(unsigned char));
		
		for( int j = 0; j < 28*28; ++j ){
			unsigned char c;
			train_image.read((char*)&c, sizeof(unsigned char));
			tmp[3*j+0] = c;
			tmp[3*j+1] = c;
			tmp[3*j+2] = c;
		}
		char filename[256];
		sprintf(filename, "%s/train\\N%d_%03d.bmp", dataDir.c_str(), (int)tmp_lab, i);

		train.push_back(filename);

		stbi_write_bmp(filename, 28, 28, 3, (void*)tmp);
	}

	const int M = 1000;
	ifstream test_image(dataDir + "\\t10k-images.idx3-ubyte", ios_base::binary);
	if( !test_image.is_open() ){
		cerr << "\"t10k-images.idx3-ubyte\" is not found!" << endl;
		return ;
	}
	ifstream test_label(dataDir + "\\t10k-labels.idx1-ubyte", ios_base::binary);
	if( !test_label.is_open() ){
		cerr << "\"t10k-labels-idx1-ubyte\" is not found!" << endl;
		return;
	}
	test_image.seekg(4*4, ios_base::beg);
	test_label.seekg(4*2, ios_base::beg);
	for( int i = 0; i < N; ++i )
	{
		unsigned char tmp_lab;
		test_label.read((char*)&tmp_lab, sizeof(unsigned char));
		
		for( int j = 0; j < 28*28; ++j ){
			unsigned char c;
			test_image.read((char*)&c, sizeof(unsigned char));
			tmp[3*j+0] = c;
			tmp[3*j+1] = c;
			tmp[3*j+2] = c;
		}
		
		char filename[256];
		sprintf(filename, "%s\\test\\N%d_%03d.bmp", dataDir.c_str(), (int)tmp_lab, i);

		test.push_back(filename);

		stbi_write_bmp(filename, 28, 28, 3, (void*)tmp);
	}



	FILE* fp = fopen( (dataDir + "\\train_files.txt").c_str(), "w");
	for (int i = 0; i < train.size(); i++)
	{
		fprintf(fp, "%s\n", train[i].c_str());
	}
	fclose(fp);
	fp = fopen( (dataDir + "\\test_files.txt").c_str(), "w");
	for (int i = 0; i < test.size(); i++)
	{
		fprintf(fp, "%s\n", test[i].c_str());
	}
	fclose(fp);
}

inline int filename_to_label(char* filename)
{
	const char id[][3] =
	{
		"N0",

		"N1",

		"N2",

		"N3",

		"N4",

		"N5",

		"N6",

		"N7",

		"N8",

		"N9",
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


inline int image_to_tensor(std::string& dataDir, tensor& im, tensor& label)
{
	const int M = 6000;
	const int N = 1000;

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
	if (!PathFileExistsA("Learned.dat_layer_0"))
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

			vector<vector<double>> tmp(1, vector<double>(img->height*img->width));
			vector<vector<double>> tmp_lb(1, vector<double>(10, 0));

			if ( Normalization )
			{
				int sz = img->height*img->width;
				double* whitening_img = image_whitening(img);
#pragma omp parallel for
				for (int k = 0; k < sz; k++)
				{
					tmp[0][k] = whitening_img[3*k+0];
				}
				delete [] whitening_img;
			}else
			{
				int sz = img->height*img->width;
#pragma omp parallel for
				for (int k = 0; k < sz; k++)
				{
					tmp[0][k] = (double)img->data[k].r/255.0;
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
	}
}

inline int get_image_and_label( char* imageFile, tensor& image)
{
	tensor& input = image;
	input.resize(1);
	input[0].resize(1);

	input[0][0].resize(28 * 28);

	Image* img = readImage(imageFile);
	if ( img == NULL ) return -1;

	int sz = img->height*img->width;
	double* whitening_img = image_whitening(img);
#pragma omp parallel for
	for (int k = 0; k < sz; k++)
	{
		input[0][0][k] = whitening_img[3 * k + 0];
	}
	delete[] whitening_img;
	delete img;

	return 0;
}


#endif
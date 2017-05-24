#ifndef __IMAGE_HPP

#undef __IMAGE_HPP


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"

#pragma warning(disable : 4244)

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

using namespace std;

typedef struct Rgb_ {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char alp;
	~Rgb_() {}
	Rgb_() {}
	inline Rgb_(int x, int y, int z)
	{
		r = x;
		g = y;
		b = z;
		alp = 255;
	}
	inline Rgb_(const int* x)
	{
		r = x[0];
		g = x[1];
		b = x[2];
		alp = 255;
	}
	inline Rgb_(const unsigned char* x)
	{
		r = x[0];
		g = x[1];
		b = x[2];
		alp = 255;
	}
	inline Rgb_(const unsigned char x)
	{
		r = x;
		g = x;
		b = x;
		alp = 255;
	}
} Rgb;


class Image
{
public:
	unsigned int height;
	unsigned int width;
	Rgb *data;

	Image()
	{
		data = 0;
	}
	~Image()
	{
		if (data) delete[] data;
	}
};

inline Image* readImage(char *filename)
{
	int i, j;
	//int real_width;
	//unsigned int width, height;
	//unsigned int color;
	//FILE *fp;
	unsigned char *bmp_line_data;
	Image *img;

	unsigned char *data = 0;
	int x, y;
	int nbit;
	data = stbi_load(filename, &x, &y, &nbit, 0);
	if (data == NULL)
	{
		printf("image file[%s] read error.\n", filename);
		return NULL;
	}
	//printf("height %d   width %d \n", y, x);

	img = new Image;
	img->data = new Rgb[x*y];
	memset(img->data, '\0', sizeof(Rgb)*x*y);
	img->height = y;
	img->width = x;

	for (i = 0; i<y; i++) {
		for (j = 0; j<x; j++) {
			if (nbit == 1)	//8bit
			{
				int pos = (i*x + j);
				img->data[pos].r = data[pos];
				img->data[pos].g = data[pos];
				img->data[pos].b = data[pos];
				img->data[pos].alp = 255;
			}
			if (nbit == 2)	//16bit
			{
				int pos = (i*x + j);
				img->data[pos].r = data[pos * 2 + 0];
				img->data[pos].g = data[pos * 2 + 0];
				img->data[pos].b = data[pos * 2 + 0];
				img->data[pos].alp = data[pos * 2 + 1];
			}
			if (nbit == 3)	//24
			{
				int pos = (i*x + j);
				img->data[pos].r = data[pos * 3 + 0];
				img->data[pos].g = data[pos * 3 + 1];
				img->data[pos].b = data[pos * 3 + 2];
				img->data[pos].alp = 255;
			}
			if (nbit == 4)	//32
			{
				int pos = (i*x + j);
				img->data[pos].r = data[pos * 4 + 0];
				img->data[pos].g = data[pos * 4 + 1];
				img->data[pos].b = data[pos * 4 + 2];
				img->data[pos].alp = data[pos * 4 + 3];
			}
		}
	}
	stbi_image_free(data);

	return img;
}

inline double* image_whitening(Image* img, double eps = 0.0)
{
	double av = 0.0;
	const int sz = img->height*img->width;
	double* data  = new double[3*sz];

#pragma omp parallel for reduction(+:av)
	for (int k = 0; k < sz; k++)
	{
		data[3*k+0] = img->data[k].r / 255.0;
		data[3*k+1] = img->data[k].g / 255.0;
		data[3*k+2] = img->data[k].b / 255.0;
		av += data[3*k+0];
		av += data[3*k+1];
		av += data[3*k+2];
	}
	av /= (double)(3 * sz);

	double sd = 0.0;
#pragma omp parallel for reduction(+:sd)
	for (int k = 0; k < sz; k++)
	{
		sd += pow(data[3*k+0] - av, 2.0);
		sd += pow(data[3*k+1] - av, 2.0);
		sd += pow(data[3*k+2] - av, 2.0);
	}
	sd = sqrt(sd / (double)(3 * sz)) + eps;
	//if ( fabs(sd) < 1.0e-16 ) return;

#pragma omp parallel for
	for (int k = 0; k < sz; k++)
	{
		data[3*k+0] = (data[3*k+0] - av) / sd;
		data[3*k+1] = (data[3*k+1] - av) / sd;
		data[3*k+2] = (data[3*k+2] - av) / sd;
	}

	return data;
}

class img_greyscale
{
public:
	void greyscale(Image* img)
	{
		for (int i = 0; i < img->height*img->width; i++)
		{
			double c = (0.299 * img->data[i].r + 0.587 * img->data[i].g + 0.114 * img->data[i].b);
			img->data[i].r = c;
			img->data[i].g = c;
			img->data[i].b = c;
		}
	}
	void greyscale(double* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			double c = (0.299 * data[3 * i + 0] + 0.587 * data[3 * i + 1] + 0.114 * data[3 * i + 2]);
			data[3 * i + 0] = c;
			data[3 * i + 1] = c;
			data[3 * i + 2] = c;
		}
	}
	void greyscale(unsigned char* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			double c = (0.299 * data[3 * i + 0] + 0.587 * data[3 * i + 1] + 0.114 * data[3 * i + 2]);
			data[3 * i + 0] = c;
			data[3 * i + 1] = c;
			data[3 * i + 2] = c;
		}
	}

};
class img_gamma
{
	double gamma_;
public:
	img_gamma(double gamma)
	{
		gamma_ = gamma;
	}
	void gamma(Image* img)
	{
		for (int i = 0; i < img->height*img->width; i++)
		{
			img->data[i].r = 255 * pow(img->data[i].r / 255.0, 1.0 / gamma_);
			img->data[i].g = 255 * pow(img->data[i].g / 255.0, 1.0 / gamma_);
			img->data[i].b = 255 * pow(img->data[i].b / 255.0, 1.0 / gamma_);
		}
	}
	void gamma(double* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			data[3 * i + 0] = 255 * pow(data[3 * i + 0] / 255.0, 1.0 / gamma_);
			data[3 * i + 1] = 255 * pow(data[3 * i + 1] / 255.0, 1.0 / gamma_);
			data[3 * i + 2] = 255 * pow(data[3 * i + 2] / 255.0, 1.0 / gamma_);
		}
	}
	void gamma(unsigned char* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			data[3 * i + 0] = 255 * pow(data[3 * i + 0] / 255.0, 1.0 / gamma_);
			data[3 * i + 1] = 255 * pow(data[3 * i + 1] / 255.0, 1.0 / gamma_);
			data[3 * i + 2] = 255 * pow(data[3 * i + 2] / 255.0, 1.0 / gamma_);
		}
	}
};

class img_contrast
{
	int min_table;
	int max_table;
	int diff_table;

	float LUT_HC[255];
	float LUT_LC[255];

public:
	img_contrast()
	{
		// ルックアップテーブルの生成
		min_table = 50;
		max_table = 205;
		diff_table = max_table - min_table;

		//ハイコントラストLUT作成
		for (int i = 0; i < min_table; i++)	LUT_HC[i] = 0;
		for (int i = min_table; i < max_table; i++)	LUT_HC[i] = 255.0 * (i - min_table) / (float)diff_table;
		for (int i = max_table; i < 255; i++)	LUT_HC[i] = 255.0;

		// ローコントラストLUT作成
		for (int i = 0; i < 255; i++) LUT_LC[i] = min_table + i * (diff_table) / 255.0;
	}

	void high(Image* img)
	{
		for (int i = 0; i < img->height*img->width; i++) 
		{
			img->data[i].r = (unsigned char)(std::min(255.0f, LUT_HC[img->data[i].r] * img->data->r));
			img->data[i].g = (unsigned char)(std::min(255.0f, LUT_HC[img->data[i].g] * img->data->r));
			img->data[i].b = (unsigned char)(std::min(255.0f, LUT_HC[img->data[i].b] * img->data->r));
		}
	}
	void high(double* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			data[3 * i + 0] = (unsigned char)(std::min(255.0, LUT_HC[(int)std::max(0.0, std::max(255.0, data[3 * i + 0]))] * data[3 * i + 0]));
			data[3 * i + 1] = (unsigned char)(std::min(255.0, LUT_HC[(int)std::max(0.0, std::max(255.0, data[3 * i + 1]))] * data[3 * i + 1]));
			data[3 * i + 2] = (unsigned char)(std::min(255.0, LUT_HC[(int)std::max(0.0, std::max(255.0, data[3 * i + 2]))] * data[3 * i + 2]));
		}
	}
	void high(unsigned char* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			data[3 * i + 0] = LUT_HC[data[3 * i + 0]] * data[3 * i + 0];
			data[3 * i + 1] = LUT_HC[data[3 * i + 1]] * data[3 * i + 1];
			data[3 * i + 2] = LUT_HC[data[3 * i + 2]] * data[3 * i + 2];
		}
	}
	void low(Image* img)
	{
		for (int i = 0; i < img->height*img->width; i++)
		{
			img->data[i].r = (unsigned char)(std::min(255.0f, LUT_LC[img->data[i].r] * img->data->r));
			img->data[i].g = (unsigned char)(std::min(255.0f, LUT_LC[img->data[i].g] * img->data->r));
			img->data[i].b = (unsigned char)(std::min(255.0f, LUT_LC[img->data[i].b] * img->data->r));
		}
	}
	void low(double* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			data[3 * i + 0] = (unsigned char)(std::min(255.0, LUT_LC[(int)std::max(0.0, std::min(255.0, data[3 * i + 0]))] * data[3 * i + 0]));
			data[3 * i + 1] = (unsigned char)(std::min(255.0, LUT_LC[(int)std::max(0.0, std::min(255.0, data[3 * i + 1]))] * data[3 * i + 1]));
			data[3 * i + 2] = (unsigned char)(std::min(255.0, LUT_LC[(int)std::max(0.0, std::min(255.0, data[3 * i + 2]))] * data[3 * i + 2]));
		}
	}
	void low(unsigned char* data, int x, int y)
	{
		for (int i = 0; i < x*y; i++)
		{
			data[3 * i + 0] = LUT_LC[data[3 * i + 0]] * data[3 * i + 0];
			data[3 * i + 1] = LUT_LC[data[3 * i + 1]] * data[3 * i + 1];
			data[3 * i + 2] = LUT_LC[data[3 * i + 2]] * data[3 * i + 2];
		}
	}
};

class img_noize
{
	std::mt19937 mt;
	double sigma_;
	std::uniform_real_distribution<double> rand_a;
	double r;
public:
	img_noize(double sigma = 15.0, double r_ = 0.3)
	{
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		mt = engine;
		sigma_ = sigma;
		std::uniform_real_distribution<double> rand_aa(0.0, 1.0);
		rand_a = rand_aa;
		r = r_;
	}

	void noize(Image* img)
	{
		std::uniform_real_distribution<double> d_rand(-sigma_, sigma_);

		for (int i = 0; i < img->height*img->width; i++)
		{
			if (rand_a(mt) < r)
			{
				img->data[i].r = (unsigned char)(std::max(0.0, std::min(255.0, img->data[i].r + d_rand(mt))));
				img->data[i].g = (unsigned char)(std::max(0.0, std::min(255.0, img->data[i].g + d_rand(mt))));
				img->data[i].b = (unsigned char)(std::max(0.0, std::min(255.0, img->data[i].b + d_rand(mt))));
			}
		}
	}
	void noize(double* data, int x, int y)
	{
		std::uniform_real_distribution<double> d_rand(-sigma_, sigma_);

		for (int i = 0; i < x*y; i++)
		{
			if (rand_a(mt) < r)
			{
				data[3 * i + 0] = (unsigned char)(std::max(0.0, std::min(255.0, data[3 * i + 0] + d_rand(mt))));
				data[3 * i + 1] = (unsigned char)(std::max(0.0, std::min(255.0, data[3 * i + 1] + d_rand(mt))));
				data[3 * i + 2] = (unsigned char)(std::max(0.0, std::min(255.0, data[3 * i + 2] + d_rand(mt))));
			}
		}
	}
	void noize(unsigned char* data, int x, int y)
	{
		std::normal_distribution<double> d_rand(0.0, sigma_);

		for (int i = 0; i < x*y; i++)
		{
			if (rand_a(mt) < r)
			{
				data[3 * i + 0] = (unsigned char)(std::max(0.0, std::min(255.0, data[3 * i + 0] + d_rand(mt))));
				data[3 * i + 1] = (unsigned char)(std::max(0.0, std::min(255.0, data[3 * i + 1] + d_rand(mt))));
				data[3 * i + 2] = (unsigned char)(std::max(0.0, std::min(255.0, data[3 * i + 2] + d_rand(mt))));
			}
		}
	}
};

class img_filter
{
	double weight[3][3];
public:
	img_filter(double* filter = NULL)
	{
		if (filter)
		{
			weight[0][0] = filter[0];
			weight[0][1] = filter[1];
			weight[0][2] = filter[2];
			weight[1][0] = filter[3];
			weight[1][1] = filter[4];
			weight[1][2] = filter[5];
			weight[2][0] = filter[6];
			weight[2][1] = filter[7];
			weight[2][2] = filter[8];
		}
		else
		{
			weight[0][0] = 1.0;
			weight[0][1] = 1.0;
			weight[0][2] = 1.0;
			weight[1][0] = 1.0;
			weight[1][1] = 1.0;
			weight[1][2] = 1.0;
			weight[2][0] = 1.0;
			weight[2][1] = 1.0;
			weight[2][2] = 1.0;
		}
	}

	void filter(Image* img)
	{
		const int x = img->width;
		const int y = img->height;
		for ( int i = 0; i < y; i++ )
		{
			for ( int j = 0; j < x; j++ )
			{
				double r, g, b;
				
				r = g = b = 0.0;
				for ( int ii = 0; ii < 3; ii++ )
				{
					for ( int jj = 0; jj < 3; jj++ )
					{
							int pos = ((i+ii)*x + (j+jj));
							if (pos >= x*y) continue;
							r += img[pos].data->r * weight[ii][jj];
							g += img[pos].data->g * weight[ii][jj];
							b += img[pos].data->b * weight[ii][jj];
					}
				}
				r /= 9.0;
				g /= 9.0;
				b /= 9.0;
				int pos = i*x + j;
				img[pos].data->r = (unsigned char)std::min(255.0, r);
				img[pos].data->g = (unsigned char)std::min(255.0, g);
				img[pos].data->b = (unsigned char)std::min(255.0, b);
			}
		}
	}
	void filter(double* data, int x, int y)
	{
		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				double r, g, b;

				r = g = b = 0.0;
				for (int ii = 0; ii < 3; ii++)
				{
					for (int jj = 0; jj < 3; jj++)
					{
						int pos = ((i + ii)*x + (j + jj));
						if (pos >= x*y) continue;
						r += data[3 * pos +  0] * weight[ii][jj];
						g += data[3 * pos +  1] * weight[ii][jj];
						b += data[3 * pos +  2] * weight[ii][jj];
					}
				}
				r /= 9.0;
				g /= 9.0;
				b /= 9.0;
				int pos = (i*x + j);
				data[3 * pos + 0] = (unsigned char)std::min(255.0, r);
				data[3 * pos + 1] = (unsigned char)std::min(255.0, g);
				data[3 * pos + 2] = (unsigned char)std::min(255.0, b);
			}
		}
	}
	void filter(unsigned char* data, int x, int y)
	{
		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				double r, g, b;

				r = g = b = 0.0;
				for (int ii = 0; ii < 3; ii++)
				{
					for (int jj = 0; jj < 3; jj++)
					{
						int pos = ((i + ii)*x + (j + jj));
						if (pos >= x*y) continue;
						r += data[3 * pos + 0] * weight[ii][jj];
						g += data[3 * pos + 1] * weight[ii][jj];
						b += data[3 * pos + 2] * weight[ii][jj];
					}
				}
				r /= 9.0;
				g /= 9.0;
				b /= 9.0;
				int pos = (i*x + j);
				data[3 * pos + 0] = (unsigned char)std::min(255.0, r);
				data[3 * pos + 1] = (unsigned char)std::min(255.0, g);
				data[3 * pos + 2] = (unsigned char)std::min(255.0, b);
			}
		}
	}
};


class img_rotation
{
public:
	img_rotation() {}

	void rotation(Image* img, const double rad)
	{
		const int x = img->width;
		const int y = img->height;
		double centerX = x / 2.0;
		double centerY = y / 2.0;
		double cosRadian = cos(rad);
		double sinRadian = sin(rad);

		Rgb* data = new Rgb[x*y];
		memcpy(data, img->data, sizeof(Rgb)*x*y);

		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				int pos = i*x + j;
				
				int pointX = (int)((j - centerX) * cosRadian - (i - centerY) * sinRadian + centerX);
				int pointY = (int)((j - centerX) * sinRadian + (i - centerY) * cosRadian + centerY);

				// poiuntX, pointYが入力画像の有効範囲にあれば出力画像へ代入する
				if (pointX >= 0 && pointX < x && pointY >= 0 && pointY < y) {
					img->data[pos] = data[pointY * x + pointX];
				}
				else {
					img->data[pos] = Rgb(0,0,0);
				}
			}
		}
		delete[] data;
	}

	void rotation(double* data, int x, int y, const double rad)
	{
		double centerX = x / 2.0;
		double centerY = y / 2.0;
		double cosRadian = cos(rad);
		double sinRadian = sin(rad);

		double* data2 = new double[x*y*3];
		for (int i = 0; i < x*y*3; i++)
		{
			data2[i] = data[i];
		}

		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				int pos = i*x + j;

				int pointX = (int)((j - centerX) * cosRadian - (i - centerY) * sinRadian + centerX);
				int pointY = (int)((j - centerX) * sinRadian + (i - centerY) * cosRadian + centerY);

				// poiuntX, pointYが入力画像の有効範囲にあれば出力画像へ代入する
				if (pointX >= 0 && pointX < x && pointY >= 0 && pointY < y) {
					int pos2 = pointY * x + pointX;
					data[3 * pos + 0] = data2[3 * pos2 + 0];
					data[3 * pos + 1] = data2[3 * pos2 + 1];
					data[3 * pos + 2] = data2[3 * pos2 + 2];
				}
				else {
					data[3 * pos + 0] = 0;
					data[3 * pos + 1] = 0;
					data[3 * pos + 2] = 0;
				}
			}
		}
		delete[] data2;
	}

	void rotation(unsigned char* data, int x, int y, const double rad)
	{
		double centerX = x / 2.0;
		double centerY = y / 2.0;
		double cosRadian = cos(rad);
		double sinRadian = sin(rad);

		double* data2 = new double[x*y * 3];
		for (int i = 0; i < x*y * 3; i++)
		{
			data2[i] = (double)data[i];
		}

		for (int i = 0; i < y; i++)
		{
			for (int j = 0; j < x; j++)
			{
				int pos = i*x + j;

				int pointX = (int)((j - centerX) * cosRadian - (i - centerY) * sinRadian + centerX);
				int pointY = (int)((j - centerX) * sinRadian + (i - centerY) * cosRadian + centerY);

				// poiuntX, pointYが入力画像の有効範囲にあれば出力画像へ代入する
				if (pointX >= 0 && pointX < x && pointY >= 0 && pointY < y) {
					int pos2 = pointY * x + pointX;
					data[3 * pos + 0] = (unsigned char)std::min(255.0, std::max(0.0, data2[3 * pos2 + 0]));
					data[3 * pos + 1] = (unsigned char)std::min(255.0, std::max(0.0, data2[3 * pos2 + 1]));
					data[3 * pos + 2] = (unsigned char)std::min(255.0, std::max(0.0, data2[3 * pos2 + 2]));
				}
				else {
					data[3 * pos + 0] = 0;
					data[3 * pos + 1] = 0;
					data[3 * pos + 2] = 0;
				}
			}
		}
		delete[] data2;
	}
};
#undef STB_IMAGE_IMPLEMENTATION

#endif

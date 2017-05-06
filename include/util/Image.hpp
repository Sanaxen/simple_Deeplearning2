#ifndef __IMAGE_HPP

#undef __IMAGE_HPP

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"

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
	int real_width;
	unsigned int width, height;
	unsigned int color;
	FILE *fp;
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

inline double* image_whitening(Image* img)
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
	sd = sqrt(sd / (double)(3 * sz));
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
#undef STB_IMAGE_IMPLEMENTATION

#endif

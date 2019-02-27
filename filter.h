/* simple filters */

#ifndef FILTER_H
#define FILTER_H

#include <vector>

#define WIDTH 4.0

/* normalize mask so it integrates to one */
static void normalize(std::vector<float> &mask) {
  int len = mask.size();
  float sum = 0;
  int i=0;
  for (i = 1; i < len; i++) {
    sum += fabs(mask[i]);
  }
  sum = 2*sum + fabs(mask[0]);
  for (i = 0; i < len; i++) {
    mask[i] /= sum;
  }
}

/* make filters */
#define MAKE_FILTER(name, fun)                                \
static std::vector<float> make_ ## name (float sigma) {       \
  sigma = __max(sigma, 0.01F);			      \
  int len = (int)ceil(sigma * WIDTH) + 1;                     \
  std::vector<float> mask(len);                               \
  for (int i = 0; i < len; i++) {                             \
    mask[i] = fun;                                            \
  }                                                           \
  return mask;                                                \
}

MAKE_FILTER(fgauss, exp(-0.5*square(i/sigma)));

static void convolve_even(float*src,int width,int height, float *dst,std::vector<float> &mask) 
{
	int len = mask.size();
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
		{
			float sum = mask[0] *(*(src+x+y*width));
			for (int i = 1; i < len; i++)
			{		 
				sum += mask[i] *(*(src+__max(x-i,0)+y*width) + *(src+__min(x+i, width-1)+width*y));
			}
			*(dst+y+x*height) = sum;
		}
	}
}
static void convolve_odd(float*src,int width,int height, float *dst,
						 std::vector<float> &mask) {
	int len = mask.size();
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float sum = mask[0] * src[x+y*width];
			for (int i = 1; i < len; i++)
			{
				sum += mask[i] *(*(src+__max(x-i,0)+y*width) + *(src+__min(x+i, width-1)+width*y));
			}
			*(dst+y+x*height) = sum;
			
		}
	}
}
static  void SmoothJh( float *src, float sigma,int w,int h) {
  std::vector<float> mask = make_fgauss(sigma);
  normalize(mask);

  float*tmp = new float[h*w];
  //float*dst = new float[w*h];
  convolve_even(src,w,h, tmp, mask);
  convolve_even(tmp,h,w, src, mask);//a little dangerous
  //for convenience since src are new allocated.

  delete tmp;
  //return dst;
}

#endif

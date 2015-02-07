#define c1d16 0.9807852804032304
#define c1d8  0.9238795325112867
#define c3d16 0.8314696123025452
#define c1d4  0.7071067811865476
#define c5d16 0.5555702330196023
#define c3d8  0.38268343236508984
#define c7d16 0.19509032201612833
#include <stdio.h>
void dct_c(const float in[64], float out[64])
{
  for (int i=0; i< 8 ; i++)
  {
    float a0 = (in[8*i+ 0] + in[8*i+ 7]) ;
    float a1 = (in[8*i+ 1] + in[8*i+ 6]) ;
    float a2 = (in[8*i+ 2] + in[8*i+ 5]) ;
    float a3 = (in[8*i+ 3] + in[8*i+ 4]) ;
    float c3 = (in[8*i+ 0] - in[8*i+ 7]) ;
    float c2 = (in[8*i+ 1] - in[8*i+ 6]) ;
    float c1 = (in[8*i+ 2] - in[8*i+ 5]) ;
    float c0 = (in[8*i+ 3] - in[8*i+ 4]) ;
    float b0 = a0 + a3;
    float b1 = a1 + a2;
    float b2 = a1 - a2;
    float b3 = a0 - a3;
    out[i + 8 * 0] =  c1d4 * (b0 +b1);
    out[i + 8 * 4] =  c1d4 * (b0 -b1);
    out[i + 8 * 2] =  c3d8 * b2 + c1d8 * b3;
    out[i + 8 * 6] =  c3d8 * b3 - c1d8 * b2;
    b0 = c1d4 * (c2 - c1);
    b1 = c1d4 * (c2 + c1);
    a0 = c0 + b0 ;
    a1 = c0 - b0;
    a2 = c3 - b1;
    a3 = c3 + b1;
    out[i + 8 * 1] = c7d16 * a0 + c1d16 * a3;
    out[i + 8 * 3] = c3d16 * a2 - c5d16 * a1;
    out[i + 8 * 5] = c3d16 * a1 + c5d16 * a2;
    out[i + 8 * 7] = c7d16 * a3 - c1d16 * a0;
  }
}
void dct_r(const float in[64], float out[64])
{
  for (int i=0; i< 8 ; i++)
  {
    float a0 = (in[8*i+ 0] + in[8*i+ 7]) ;
    float a1 = (in[8*i+ 1] + in[8*i+ 6]) ;
    float a2 = (in[8*i+ 2] + in[8*i+ 5]) ;
    float a3 = (in[8*i+ 3] + in[8*i+ 4]) ;
    float c3 = (in[8*i+ 0] - in[8*i+ 7]) ;
    float c2 = (in[8*i+ 1] - in[8*i+ 6]) ;
    float c1 = (in[8*i+ 2] - in[8*i+ 5]) ;
    float c0 = (in[8*i+ 3] - in[8*i+ 4]) ;
    float b0 = a0 + a3;
    float b1 = a1 + a2;
    float b2 = a1 - a2;
    float b3 = a0 - a3;

    out[i * 8 + 0] = (c1d4 * (b0 +b1))/4;
    out[i * 8 + 4] = (c1d4 * (b0 -b1))/4;
    out[i * 8 + 2] = (c3d8 * b2 + c1d8 * b3)/4;
    out[i * 8 + 6] = (c3d8 * b3 - c1d8 * b2)/4;
    b0 = c1d4 * (c2 - c1);
    b1 = c1d4 * (c2 + c1);
    a0 = c0 + b0 ;
    a1 = c0 - b0;
    a2 = c3 - b1;
    a3 = c3 + b1;
    out[i * 8 + 1] =(c7d16 * a0 + c1d16 * a3)/4;
    out[i * 8 + 3] =(c3d16 * a2 - c5d16 * a1)/4;
    out[i * 8 + 5] =(c3d16 * a1 + c5d16 * a2)/4;
    out[i * 8 + 7] =(c7d16 * a3 - c1d16 * a0)/4;
  }
}

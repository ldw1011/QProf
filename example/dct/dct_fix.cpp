#include <stdio.h>

#define NO_MULTIPLY
#ifdef NO_MULTIPLY
#define LS(r,s) ((r) << (s))
#define RS(r,s) ((r) >> (s))       /* Caution with rounding... */
#else
#define LS(r,s) ((r) * (1 << (s)))
#define RS(r,s) ((r) / (1 << (s))) /* Correct rounding */
#endif

#define MSCALE(expr)  RS((expr),9)

/* Cos constants */

#define c1d4 362
#define c1d8 473
#define c3d8 196
#define c1d16 502
#define c3d16 426
#define c5d16 284
#define c7d16 100
inline
short CLIP(int tmp)
{
   short tval=tmp;
   tval = (((tval < 0) ? (tval - 4) : (tval + 4)) / 8);
   if (tval < -1023)
   {
     tval = -1023;
   }
   else if (tval > 1023)
   {
     tval = 1023;
   }
	 return tval;
}


void dct(short inqueue[64],short outqueue[64])
{
  {

	  int i;// aptr;
	  short a0,  a1,  a2,  a3;
	  short b0,  b1,  b2,  b3;
	  short tb0,  tb1,  ta0, ta1,  ta2,  ta3;
	  short c0,  c1,  c2,  c3;
	  short v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7;
	  //yuv_t in_block[64];
	  short tmp[64];


	  for (i = 0; i < 8; i++)
	  {

		//#pragma HLS PIPELINE II=8
		//aptr = i;
		v0=inqueue[8*i+0];
		v1=inqueue[8*i+1];
		v2=inqueue[8*i+2];
		v3=inqueue[8*i+3];
		v4=inqueue[8*i+4];
		v5=inqueue[8*i+5];
		v6=inqueue[8*i+6];
		v7=inqueue[8*i+7];
    
//   printf("%d\n",v0);
//   printf("%d\n",v1);
//   printf("%d\n",v2);
//   printf("%d\n",v3);
//   printf("%d\n",v4);
//   printf("%d\n",v5);
//   printf("%d\n",v6);
//   printf("%d\n",v7);

		a0 = LS((v0 + v7),  2); c3 = LS((v0 - v7),  2);
		a1 = LS((v1 + v6),  2); c2 = LS((v1 - v6),  2);
		a2 = LS((v2 + v5),  2); c1 = LS((v2 - v5),  2);
		a3 = LS((v3 + v4),  2); c0 = LS((v3 - v4),  2);
		b0 = a0 + a3;
		b1 = a1 + a2;
		b2 = a1 - a2;
		b3 = a0 - a3;
		tmp[i] =      MSCALE(((c1d4 * (b0 + b1))));
		tmp[i + 32] = MSCALE(((c1d4 * (b0 - b1))));
		tmp[i + 16] = MSCALE(((c3d8 * b2) + (c1d8 * b3)));
		tmp[i + 48] = MSCALE(((c3d8 * b3) - (c1d8 * b2)));
		b0 =          MSCALE(((c1d4 * (c2 - c1))));
		b1 =          MSCALE(((c1d4 * (c2 + c1))));
		a0 = c0 + b0;
		a1 = c0 - b0;
		a2 = c3 - b1;
		a3 = c3 + b1;
		tmp[i + 8]  = MSCALE(((c7d16 * a0) + (c1d16 * a3)));
		tmp[i + 24] = MSCALE(((c3d16 * a2) - (c5d16 * a1)));
		tmp[i + 40] = MSCALE(((c3d16 * a1) + (c5d16 * a2)));
		tmp[i + 56] = MSCALE(((c7d16 * a3) - (c1d16 * a0)));
	  }

	  for (i = 0; i < 8; i++)
	  {

		#pragma HLS PIPELINE II=8
		//aptr = LS(i,  3);
		v0 = tmp[i*8+0];// aptr++;
		v1 = tmp[i*8+1]; //aptr++;
		v2 = tmp[i*8+2]; //aptr++;
		v3 = tmp[i*8+3]; //aptr++;
		v4 = tmp[i*8+4];// aptr++;
		v5 = tmp[i*8+5]; //aptr++;
		v6 = tmp[i*8+6]; //aptr++;
		v7 = tmp[i*8+7];
		c3 = RS((v0 - v7),  1); a0 = RS((v0 + v7),  1);
		c2 = RS((v1 - v6),  1); a1 = RS((v1 + v6),  1);
		c1 = RS((v2 - v5),  1); a2 = RS((v2 + v5),  1);
		c0 = RS((v3 - v4),  1); a3 = RS((v3 + v4),  1);
		b0 = a0 + a3; b1 = a1 + a2; b2 = a1 - a2; b3 = a0 - a3;
		tb0 =          MSCALE(((c1d4 * (c2 - c1))));
		tb1 =          MSCALE(((c1d4 * (c2 + c1))));
		ta0 = c0 + tb0;
		ta1 = c0 - tb0;
		ta2 = c3 - tb1;
		ta3 = c3 + tb1;
		outqueue[i*8+0]=(CLIP(MSCALE(((c1d4 * (b0 + b1))))));
    printf("%d %d %d\n",b0,b1,outqueue[i*8]);
		outqueue[i*8+1]=(CLIP(MSCALE(((c7d16*  ta0) + (c1d16* ta3)))));
		outqueue[i*8+2]=(CLIP(MSCALE(((c3d8 * b2)   + (c1d8 *b3)))));
		outqueue[i*8+3]=(CLIP(MSCALE(((c3d16*  ta2) - (c5d16* ta1)))));
		outqueue[i*8+4]=(CLIP(MSCALE(((c1d4 * (b0 - b1))))));
		outqueue[i*8+5]=(CLIP(MSCALE(((c3d16*  ta1) + (c5d16* ta2)))));
		outqueue[i*8+6]=(CLIP(MSCALE(((c3d8 * b3)   - (c1d8 *b2)))));
		outqueue[i*8+7]=(CLIP(MSCALE(((c7d16*  ta3) - (c1d16* ta0)))));
	  }

  }
}

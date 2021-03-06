#include <stdio.h>
void dct_c(const float in[64], float out[64]);
void dct_r(const float in[64], float out[64]);
void dct(short in[64], short  out[64]);
void idct(int* in, int* out);


int main()
{
  float in[64];
  short in_f[64];
  short out_f[64];
  float out[64];
  float out_r[64];
  float tmp[64];
  int geti_f[64];
  int get_f[64];
  FILE* ifile=fopen("test_vector.csv","r");
  for (int i=0; i<1; i++)
  {

    for (int j=0; j<64; j++)
    {
      fscanf(ifile, "%f,%f\n", &in[j],&out_r[j]);
      in_f[j]=in[j];
    }
    dct_c(in,tmp);
    dct_r(tmp,out);
    dct(in_f,out_f);
    for(int j=0; j<8; j++)
    {
      for(int k=0;k<8; k++)
        geti_f[j*8+k]=out_f[k*8+j];
    }
    idct(geti_f, get_f);
    for (int j=0; j<64; j++)
    {
      printf("d %d %d\n", in_f[j],get_f[j]);
      /*if(out_r[j]!=out[j])
      {
        printf("error %f %f %f\n",out_r[j],out[j],out_f[j]/1.0);
      }*/
    }
  }
  fclose(ifile);
  return 0;
}

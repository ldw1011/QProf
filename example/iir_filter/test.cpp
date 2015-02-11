#include <stdio.h>
#include <math.h>
#define LEN 1000
void lowpass(double x[LEN], double dt, double RC, double y[LEN]);
int main()
{
  double x[LEN];
  double y[LEN];
  for(int i=0 ; i<LEN; i++)
  {
    if(i%100>50)
    {
      x[i]=0;
    }
    else
    {
      x[i]=1;
    }
  }
  lowpass(x,0.001,1/M_PI/4,y);
  for(int i=1 ; i<LEN; i++)
  {
    printf("%f\n",y[i]);
  }
}

#include <iostream>
#include <stdio.h>
void test(float a[8], float b[8]);
int main()
{
  float input[]={0.,1.,2.,3.,4.,5.,6.,0.7};
  float out[8];
  test(input,out);
  for(int i=0 ;i <8; i++)
  {
    std::cout<<out[i]<<" ";
  }
  std::cout<<std::endl;
  
  return 0;
}

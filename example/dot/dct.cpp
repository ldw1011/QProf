
void test(float a[8], float b[8])
{
  const float coeff[8]={1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8};
  for (int i=0; i<8; i++)
  {
    b[i]=a[i]*coeff[i];
    if(b[i]>0)
    {
      b[i]=1;
    }
  }
  return;
}

void test(int a[4],int b[4])
{
  for(int i=0; i<3; i++)
  {
    if(i<3)
      b[a[i]]=1;
    else
      b[a[i]]=2;
  }
}

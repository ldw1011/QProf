int if_conv(int a)
{
  int b=0;
  if(a>0)
  {
    b=a*3;
  }
  else
  {
    b=a+3;
  }

  if(b>0)
  {
    return b;
  }
  else
  {
    return b+a;
  }
}

// Return RC low-pass filter output samples, given input samples,
// time interval dt, and time constant RC
#define LEN 1000
void lowpass(double x[LEN], double dt, double RC, double y[LEN])
{
  double alpha= dt / (RC + dt);
  y[0] = x[0];
  for (int i=0; i< LEN; i++)
  {
    y[i]=y[i-1] + alpha * (x[i] - y[i-1]);
  }
  return;
}

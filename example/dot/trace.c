extern "C"
{
  

  void _trace(int id, int idx, double ret, double op0, double op1);
  void trace(int id, int idx, double ret, double op0, double op1)
  {
    _trace(id, idx, ret, op0, op1);
  }
  void _traceFCMP(int id, int idx, bool  ret, double op0, double op1);
  void traceFCMP(int id, int idx, bool  ret, double op0, double op1)
  {
    _traceFCMP(id, idx, ret, op0, op1);
  }
  void _traceSelect(int id, int idx, double ret,bool cond, double op0, double op1);
  void traceSelect(int id, int idx, double ret,bool cond, double op0, double op1)
  {
    _traceSelect(id, idx, ret,cond, op0, op1);
  }
}

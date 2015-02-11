#include <iostream>
#include <map>
using namespace std;
struct statistics
{
  statistics()
    : mean(0), moment(0), n(0)
  {
  }
  void update(double value)
  {
    n=n+1;
    mean= mean + 1/n * (value - mean);
    moment= moment + 1/n * (value*value - moment);
  }
  double mean;
  double moment;
  double n;
};
ostream& operator<<(ostream& os, const statistics& is) 
{
  os<<"Mean: "<<is.mean<<" Moment: "<<is.moment<<" var: "<<is.moment - is.mean*is.mean <<" n: "<<is.n;
  return os;
}
class Stats
{
  public:
    Stats()
    {

    }
    void trace(int id, int idx, double ret, double op0, double op1)
    {
      statistics& st0=st[id][idx][0]; // ret;
      st0.update(ret);
      statistics& st1=st[id][idx][1]; // ret;
      st1.update(op0);
      statistics& st2=st[id][idx][2]; // ret;
      st2.update(op1);
    }
    void traceFCMP(int id, int idx, bool  ret, double op0, double op1)
    {
      statistics& st0=st[id][idx][0]; // ret;
      st0.update(ret);
      statistics& st1=st[id][idx][1]; // ret;
      st1.update(op0);
      statistics& st2=st[id][idx][2]; // ret;
      st2.update(op1);
    }
    void traceSelect(int id, int idx, double ret,bool cond, double op0, double op1)
    {
      statistics& st0=st[id][idx][0]; // ret;
      st0.update(ret);
      statistics& st1=st[id][idx][1]; // ret;
      st1.update(cond);
      statistics& st2=st[id][idx][2]; // ret;
      st2.update(op0);
      statistics& st3=st[id][idx][3]; // ret;
      st3.update(op1);
    }
    map<int, map<int, map<int, statistics> > >st;
    ~Stats()
    {
      for(auto& i : st)
      {
        cout<<i.first<<"\n";
        for(auto& j : i.second)
        {
          cout<<"\t"<<j.first<<"\n";
          for(auto& k : j.second)
          {
            cout<<"\t"<<"\t"<<k.first<<" ";
            cout<<k.second<<"\n"; 
          }
        }
      }
    }
};
Stats& getStats()
{
  static Stats st;
  return st;
}
extern "C"
{
  void _trace(int id, int idx, double ret, double op0, double op1)
  {
    Stats& st= getStats();
    st.trace(id, idx, ret, op0, op1); 
  }
  void _traceFCMP(int id, int idx, bool ret, double op0, double op1)
  {
    Stats& st= getStats();
    st.traceFCMP(id, idx, ret, op0, op1); 
  }
  void _traceSelect(int id, int idx, double ret, bool cond, double op0, double op1)
  {
    Stats& st= getStats();
    st.traceSelect(id, idx, ret, cond, op0, op1); 
  }
}

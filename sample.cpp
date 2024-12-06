
#include <stdexcept>
#include <sstream>
#include "sample.hpp"

using namespace std;


Sample::Sample(const string& tm, const string& si, const string& para, double res, const string& un, const string& ma, const string& co):
    time(tm), site(si), parameter(para), result(res), unit(un), material(ma), compliance(co)
{
  ostringstream error;
}


ostream& operator<<(ostream& out, const Sample& sample)
{
  return out << "Time: " << sample.getTime()
             << "\nSite: " << sample.getSite() 
             << "\nParameter: " << sample.getParameter() 
             << "\nResult: " << sample.getResult() 
             << "\nUnit: " << sample.getUnit()
             << "\nMaterial: " << sample.getMaterial()
             << "\nCompliance: " << sample.getCompliance() << endl;;
}

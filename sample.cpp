
#include <stdexcept>
#include <sstream>
#include "sample.hpp"

using namespace std;


Sample::Sample(const string& tm, const string& si, const string& para, double res, const string& un):
  time(tm), site(si), parameter(para), result(res), unit(un)
{
  ostringstream error;
/*
  if (latitude < MIN_LATITUDE or latitude > MAX_LATITUDE) {
    error << "Invalid latitude: " << latitude;
    throw out_of_range(error.str());
  }

  if (longitude < MIN_LONGITUDE or longitude > MAX_LONGITUDE) {
    error << "Invalid longitude: " << longitude;
    throw out_of_range(error.str());
  }

  if (depth < 0.0) {
    error << "Invalid depth: " << depth;
    throw out_of_range(error.str());
  }

  if (magnitude < 0.0) {
    error << "Invalid magnitude: " << magnitude;
    throw out_of_range(error.str());
  }*/
}


ostream& operator<<(ostream& out, const Sample& sample)
{
  return out << "Time: " << sample.getTime()
             << "\nSite: " << sample.getSite() 
             << "\nParameter: " << sample.getParameter() 
             << "\nResult: " << sample.getResult() 
             << "\nUnit: " << sample.getUnit() << endl;
}

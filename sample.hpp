
#pragma once

#include <string>
#include <iostream>

class Sample
{
  public:
    Sample(const std::string&, const std::string&, const std::string&, double, const std::string&);
    std::string getTime() const { return time; }
    std::string getSite() const { return site; }
    std::string getParameter() const { return parameter; }
    double getResult() const { return result; }
    std::string getUnit() const { return unit; }

  private:
    std::string time;
    std::string site;
    std::string parameter;
    double result;
    std::string unit;
};

std::ostream& operator << (std::ostream&, const Sample&);

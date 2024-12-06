
#pragma once

#include <string>
#include <iostream>

class Sample
{
  public:
    Sample(const std::string&, const std::string&, const std::string&, double, const std::string&, const std::string&, const std::string&);
    std::string getTime() const { return time; }
    std::string getSite() const { return site; }
    std::string getParameter() const { return parameter; }
    double getResult() const { return result; }
    std::string getUnit() const { return unit; }
    std::string getMaterial() const { return material; }
    std::string getCompliance() const { return compliance; }

  private:
    std::string time;
    std::string site;
    std::string parameter;
    double result;
    std::string unit;
    std::string material;
    std::string compliance;
};

std::ostream& operator << (std::ostream&, const Sample&);

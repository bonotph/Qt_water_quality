
#pragma once

#include <vector>
#include <set>
#include <string>
#include "sample.hpp"

class SampleDataset
{
  public:
    SampleDataset() {}
    SampleDataset(const std::string& filename) { loadData(filename); }
    void loadData(const std::string&);
    int size() const { return data.size(); }
    Sample operator[](int index) const { return data.at(index); }
    std::set<std::string> getUniquePollutants() const;
    /*Sample strongest() const;
    Sample shallowest() const;
    double meanDepth() const;
    double meanMagnitude() const;*/

  private:
    std::vector<Sample> data;
    void checkDataExists() const;
};

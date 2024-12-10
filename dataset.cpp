
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"

using namespace std;


void SampleDataset::loadData(const string& filename)
{
  csv::CSVReader reader(filename);

  data.clear();

  for (const auto& row: reader) {
    Sample Sample{
      row["sample.sampleDateTime"].get<>(), //date and time
      row["sample.samplingPoint.label"].get<>(), //site
      row["determinand.label"].get<>(), //parameter
      row["result"].get<double>(),
      row["determinand.unit.label"].get<>(), //unit
      row["sample.sampledMaterialType.label"].get<>(), //material
      row["sample.isComplianceSample"].get<>(), //compliance status
      row["sample.sampledMaterialType.label"].get<>() //Water type
    };
    data.push_back(Sample);
  }
}

void SampleDataset::checkDataExists() const
{
  if (size() == 0) {
    throw std::runtime_error("Dataset is empty!");
  }
}

// Implementation of getUniquePollutants()
std::set<std::string> SampleDataset::getUniquePollutants() const {
    std::set<std::string> uniquePollutants;

    for (const auto& sample : data) {
        uniquePollutants.insert(sample.getParameter());  // Add parameter to the set
    }

    return uniquePollutants;  // Return the set of unique pollutants
}
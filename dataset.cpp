
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

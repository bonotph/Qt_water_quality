
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
      row["determinand.unit.label"].get<>() //unit
    };
    data.push_back(Sample);
  }
}

/*
Sample SampleDataset::strongest() const
{
  checkDataExists();

  auto sample = max_element(data.begin(), data.end(),
    [](Sample a, Sample b) { return a.getMagnitude() < b.getMagnitude(); });

  return *sample;
}


Sample SampleDataset::shallowest() const
{
  checkDataExists();

  auto sample = min_element(data.begin(), data.end(),
    [](Sample a, Sample b) { return a.getDepth() < b.getDepth(); });

  return *sample;
}


double Sample::meanDepth() const
{
  checkDataExists();

  auto sum = accumulate(data.begin(), data.end(), 0.0,
    [](double total, Sample q) { return total + q.getDepth(); });

  return sum / data.size();
}


double SampleDataset::meanMagnitude() const
{
  checkDataExists();

  auto sum = accumulate(data.begin(), data.end(), 0.0,
    [](double total, Sample q) { return total + q.getMagnitude(); });

  return sum / data.size();
}
*/

void SampleDataset::checkDataExists() const
{
  if (size() == 0) {
    throw std::runtime_error("Dataset is empty!");
  }
}

// COMP2811 Coursework 2: data model

#include "model.hpp"


void SampleModel::updateFromFile(const QString& filename)
{
  beginResetModel();
  dataset.loadData(filename.toStdString());
  endResetModel();
}


QVariant SampleModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }

  if (role == Qt::TextAlignmentRole) {
    return int(Qt::AlignRight | Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole) {
    Sample q = dataset[index.row()];
    switch (index.column()) {
      case 0: return QVariant(q.getTime().c_str());
      case 1: return QVariant(QString::fromStdString(q.getSite()));
      case 2: return QVariant(q.getParameter().c_str());
      case 3: return QVariant(q.getResult());
      case 4: return QVariant(q.getUnit().c_str());
      case 5: return QVariant(q.getMaterial().c_str());
      case 6: return QVariant(q.getCompliance().c_str());
    }
  }

  return QVariant();
}


QVariant SampleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Vertical) {
    return QVariant(section + 1);
  }

  switch (section) {
    case 0: return QString("Time");
    case 1: return QString("Site");
    case 2: return QString("Parameter");
    case 3: return QString("Result");
    case 4: return QString("Unit");
    case 5: return QString("Material");
    case 6: return QString("Compliance");
    default: return QVariant();
  }
}

QStringList SampleModel::getUniquePollutants() const {
    QStringList uniqueList;

    for (const auto& pollutant : dataset.getUniquePollutants()) {
        uniqueList.append(QString::fromStdString(pollutant));
    }

    uniqueList.sort();
    return uniqueList;
}
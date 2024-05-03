#ifndef CLIPFILTEROPTION_H
#define CLIPFILTEROPTION_H

#include "FilterOptionInterface.h"
#include <QWidget>
#include <qtmetamacros.h>
#include <vtkClipDataSet.h>
#include <vtkSmartPointer.h>

namespace Ui {
class ClipFilterOption;
}

class ClipFilterOption : public QWidget, public Filter::FilterOptionInterface {
  Q_OBJECT

public:
  explicit ClipFilterOption(
      QWidget *parent = nullptr,
      vtkSmartPointer<vtkAlgorithm> filterPointer = nullptr);
  ~ClipFilterOption();
  void SetValue() override;

private slots:
  void handleNormalX(double value);
  void handleNormalY(double value);
  void handleNormalZ(double value);
  void handleOriginX(double value);
  void handleOriginY(double value);
  void handleOriginZ(double value);

private:
  Ui::ClipFilterOption *ui;

  vtkSmartPointer<vtkClipDataSet> filter;

  float originX, originY, originZ;
  float normalX, normalY, normalZ;
};

#endif // CLIPFILTEROPTION_H

#ifndef SHRINKFILTEROPTION_H
#define SHRINKFILTEROPTION_H

#include "FilterOptionInterface.h"
#include <QWidget>
#include <vtkAlgorithm.h>
#include <vtkShrinkFilter.h>
#include <vtkSmartPointer.h>

namespace Ui {
class ShrinkFilterOption;
}

class ShrinkFilterOption : public QWidget,
                           public Filter::FilterOptionInterface {
  Q_OBJECT

public:
  explicit ShrinkFilterOption(
      QWidget *parent = nullptr,
      vtkSmartPointer<vtkAlgorithm> shrinkFilterPointer = nullptr);
  ~ShrinkFilterOption();
  void SetValue() override;

private slots:
  void handleSlider(int value);
  void handleSpinBox(double value);

private:
  Ui::ShrinkFilterOption *ui;
  vtkSmartPointer<vtkShrinkFilter> filter;
  float shrinkFactor;
};

#endif // SHRINKFILTEROPTION_H

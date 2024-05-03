#include "shrinkfilteroption.h"
#include "ui_shrinkfilteroption.h"
#include <vtkAlgorithm.h>
#include <vtkShrinkFilter.h>
#include <vtkSmartPointerBase.h>

ShrinkFilterOption::ShrinkFilterOption(
    QWidget *parent, vtkSmartPointer<vtkAlgorithm> shrinkFilterPointer)
    : QWidget(parent), ui(new Ui::ShrinkFilterOption) {
  ui->setupUi(this);
  // TODO: Get shrinkFactor from the filter

  filter = vtkShrinkFilter::SafeDownCast(shrinkFilterPointer);

  shrinkFactor = filter->GetShrinkFactor();

  ui->doubleSpinBox->setValue(shrinkFactor);
  ui->horizontalSlider->setValue(shrinkFactor *
                                 ui->horizontalSlider->maximum());

  connect(ui->horizontalSlider, &QSlider::valueChanged, this,
          &ShrinkFilterOption::handleSlider);

  connect(ui->doubleSpinBox, &QDoubleSpinBox::valueChanged, this,
          &ShrinkFilterOption::handleSpinBox);

  qDebug() << "Maximum value: " << ui->horizontalSlider->maximum();
}

void ShrinkFilterOption::handleSlider(int value) {
  float updatedValue =
      static_cast<float>(value) / ui->horizontalSlider->maximum();
  if (shrinkFactor == updatedValue) {
    return;
  }
  shrinkFactor = updatedValue;
  qDebug() << "ShrinkFactor: " << shrinkFactor << "Value: " << value;
  ui->doubleSpinBox->setValue(shrinkFactor);
}

void ShrinkFilterOption::handleSpinBox(double value) {
  if (shrinkFactor == value) {
    return;
  }
  shrinkFactor = value;
  int sliderValue = shrinkFactor * ui->horizontalSlider->maximum();
  qDebug() << "ShrinkFactor: " << shrinkFactor << "Value: " << sliderValue;
  ui->horizontalSlider->setValue(sliderValue);
}

void ShrinkFilterOption::SetValue() { filter->SetShrinkFactor(shrinkFactor); }

ShrinkFilterOption::~ShrinkFilterOption() {
  qDebug() << "ShrinkFilterOption deleted";
  delete ui;
}

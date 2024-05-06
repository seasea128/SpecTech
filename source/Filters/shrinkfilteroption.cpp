#include "shrinkfilteroption.h"
#include "ui_shrinkfilteroption.h"
#include <vtkAlgorithm.h>
#include <vtkShrinkFilter.h>
#include <vtkSmartPointerBase.h>

ShrinkFilterOption::ShrinkFilterOption(
    QWidget *parent, vtkSmartPointer<vtkAlgorithm> filterPointer)
    : QWidget(parent), ui(new Ui::ShrinkFilterOption) {
  ui->setupUi(this);

  // Get the filter in the correct type
  filter = vtkShrinkFilter::SafeDownCast(filterPointer);

  // Get the property that can be changed with this widget
  shrinkFactor = filter->GetShrinkFactor();

  // Set the property to the UI
  ui->doubleSpinBox->setValue(shrinkFactor);
  ui->horizontalSlider->setValue(shrinkFactor *
                                 ui->horizontalSlider->maximum());

  // Connect the UI with slot
  connect(ui->horizontalSlider, &QSlider::valueChanged, this,
          &ShrinkFilterOption::handleSlider);
  connect(ui->doubleSpinBox, &QDoubleSpinBox::valueChanged, this,
          &ShrinkFilterOption::handleSpinBox);
}

void ShrinkFilterOption::handleSlider(int value) {
  float updatedValue =
      static_cast<float>(value) / ui->horizontalSlider->maximum();

  // Need to do this check, otherwise the value would not update correctly
  // because setting the slider will trigger handleSpinBox, which will trigger
  // this slot again.
  if (shrinkFactor == updatedValue) {
    return;
  }
  shrinkFactor = updatedValue;
  ui->doubleSpinBox->setValue(shrinkFactor);
}

void ShrinkFilterOption::handleSpinBox(double value) {
  // Need to do this check, otherwise the value would not update correctly
  // because setting the spinbox will trigger handleSlider, which will trigger
  // this slot again.
  if (shrinkFactor == value) {
    return;
  }
  shrinkFactor = value;
  int sliderValue = shrinkFactor * ui->horizontalSlider->maximum();
  ui->horizontalSlider->setValue(sliderValue);
}

void ShrinkFilterOption::SetValue() { filter->SetShrinkFactor(shrinkFactor); }

ShrinkFilterOption::~ShrinkFilterOption() { delete ui; }

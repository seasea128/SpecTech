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

/**
 * Widget for editing shrink filter.
 */
class ShrinkFilterOption : public QWidget,
                           public Filter::FilterOptionInterface {
  Q_OBJECT

public:
  /**
   * Constructor for ShrinkFilterOption.
   * @param parent is the widget that create this widget.
   * @param filterPointer is the pointer to the filter that is being edited.
   */
  explicit ShrinkFilterOption(
      QWidget *parent = nullptr,
      vtkSmartPointer<vtkAlgorithm> filterPointer = nullptr);

  /**
   * Destrutor for ShrinkFilterOption.
   */
  ~ShrinkFilterOption();

  /**
   * Function that set the value edited to the filter
   */
  void SetValue() override;

private slots:
  /**
   * Slot for updating shrinkFactor with slider.
   * @param value is the new value of shrinkFactor that is multiplied by the
   * maximum value of the slider.
   */
  void handleSlider(int value);

  /**
   * Slot for updating shrinkFactor with doubleSpinBox.
   * @param value is the new value of shrinkFactor.
   */
  void handleSpinBox(double value);

private:
  /**
   *  UI component of this widget.
   */
  Ui::ShrinkFilterOption *ui;

  /**
   * Pointer to the filter that is being edited.
   */
  vtkSmartPointer<vtkShrinkFilter> filter;

  /**
   * Shrink factor of the filter.
   */
  float shrinkFactor;
};

#endif // SHRINKFILTEROPTION_H

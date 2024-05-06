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

/**
 * Widget for editing clip filter.
 */
class ClipFilterOption : public QWidget, public Filter::FilterOptionInterface {
  Q_OBJECT

public:
  /**
   * Constructor for ClipFilterOption.
   * @param parent is the widget that create this widget.
   * @param filterPointer is the pointer to the filter that is being edited.
   */
  explicit ClipFilterOption(
      QWidget *parent = nullptr,
      vtkSmartPointer<vtkAlgorithm> filterPointer = nullptr);

  /**
   *  Destructor for ClipFilterOption.
   */
  ~ClipFilterOption();

  /**
   * Function that set the value edited to the filter
   */
  void SetValue() override;

private slots:
  /**
   * Slot for updating normal in X axis
   * @param value is the updated value.
   */
  void handleNormalX(double value);
  /**
   * Slot for updating normal in Y axis
   * @param value is the updated value.
   */
  void handleNormalY(double value);
  /**
   * Slot for updating normal in Z axis
   * @param value is the updated value.
   */
  void handleNormalZ(double value);
  /**
   * Slot for updating origin in X axis
   * @param value is the updated value.
   */
  void handleOriginX(double value);
  /**
   * Slot for updating origin in Y axis
   * @param value is the updated value.
   */
  void handleOriginY(double value);
  /**
   * Slot for updating origin in Z axis
   * @param value is the updated value.
   */
  void handleOriginZ(double value);

private:
  /**
   * UI component of this widget.
   */
  Ui::ClipFilterOption *ui;

  /**
   * Pointer to the filter that is being edited
   */
  vtkSmartPointer<vtkClipDataSet> filter;

  float originX; /**< X Coordinate of origin point */
  float originY; /**< Y Coordinate of origin point */
  float originZ; /**< Z Coordinate of origin point */
  float normalX; /**< X Coordinate of normal point */
  float normalY; /**< Y Coordinate of normal point */
  float normalZ; /**< Z Coordinate of normal point */
};

#endif // CLIPFILTEROPTION_H

#ifndef FILTEROPTION_H
#define FILTEROPTION_H

#include "Filters/shrinkfilteroption.h"
#include "ModelPart.h"
#include <QWidget>

namespace Ui {
class FilterOption;
}

/**
 * Widget for holding all filter related widgets
 * This is the main part that has the list.
 */
class FilterOption : public QWidget {
  Q_OBJECT

public:
  /**
   * Constructor for FilterOption.
   * @param parent is the widget that creates this widget.
   * @param part is the ModelPart that is being passed through.
   */
  explicit FilterOption(QWidget *parent = nullptr, ModelPart *part = nullptr);

  /**
   * Destructor for FilterOption.
   */
  ~FilterOption();

  /**
   * Sets the value of the filter inside the list and copy the temporary list
   * over to ModelPart. Also sets the filter chain up too.
   */
  void SetValue();

private slots:
  /**
   * Slot for handling clicking on listWidget.
   * Changes the widget inside tabWidget to the selected one.
   */
  void handleListClick();

  /**
   * Slot for adding clip filter.
   * Create new filter, add it to the filterList and also add new widgets to the
   * chain.
   */
  void on_actionAdd_clip_filter_triggered();

  /**
   * Slot for adding shrink filter.
   * Create new filter, add it to the filterList and also add new widgets to the
   * chain.
   */
  void on_actionAdd_shrink_filter_triggered();

  /**
   * Slot for handling remove button.
   * Remove the child widget and filter inside the list. Also readjust the
   * childWidget index to match with filterList.
   */
  void handleRemoveButton();

private:
  /**
   * UI component of FilterOption.
   */
  Ui::FilterOption *ui;

  /**
   * Pointer to currently selected ModelPart.
   */
  ModelPart *part;

  /**
   * Pointer to menu of the add button.
   */
  QMenu *menu;

  /**
   *  A copy of filterList from ModelPart.
   */
  std::vector<Filter::FilterData> filterList;
};

#endif // FILTEROPTION_H

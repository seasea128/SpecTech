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
  explicit FilterOption(QWidget *parent = nullptr, ModelPart *part = nullptr);
  ~FilterOption();
  void SetValue();

private slots:
  void handleListClick();

  void on_actionAdd_clip_filter_triggered();

  void on_actionAdd_shrink_filter_triggered();

private:
  void handleRemoveButton();

  Ui::FilterOption *ui;
  ModelPart *part;
  QMenu *menu;

  std::vector<Filter::FilterData> filterList;
};

#endif // FILTEROPTION_H

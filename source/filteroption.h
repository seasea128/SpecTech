#ifndef FILTEROPTION_H
#define FILTEROPTION_H

#include "ModelPart.h"
#include <QWidget>

namespace Ui {
class FilterOption;
}

class FilterOption : public QWidget {
  Q_OBJECT

public:
  explicit FilterOption(QWidget *parent = nullptr, ModelPart *part = nullptr);
  ~FilterOption();
  void SetValue();

private:
  Ui::FilterOption *ui;
  ModelPart *part;
};

#endif // FILTEROPTION_H

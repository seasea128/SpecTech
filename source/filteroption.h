#ifndef FILTEROPTION_H
#define FILTEROPTION_H

#include <QWidget>

namespace Ui {
class FilterOption;
}

class FilterOption : public QWidget {
  Q_OBJECT

public:
  explicit FilterOption(QWidget *parent = nullptr);
  ~FilterOption();

private:
  Ui::FilterOption *ui;
};

#endif // FILTEROPTION_H

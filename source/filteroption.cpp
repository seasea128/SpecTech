#include "filteroption.h"
#include "ui_filteroption.h"

FilterOption::FilterOption(QWidget *parent)
    : QWidget(parent), ui(new Ui::FilterOption) {
  ui->setupUi(this);
}

FilterOption::~FilterOption() { delete ui; }

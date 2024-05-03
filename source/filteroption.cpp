#include "filteroption.h"
#include "ui_filteroption.h"

FilterOption::FilterOption(QWidget *parent, ModelPart *part)
    : QWidget(parent), ui(new Ui::FilterOption), part(part) {
  ui->setupUi(this);
}

FilterOption::~FilterOption() { delete ui; }

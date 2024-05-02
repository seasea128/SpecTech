#include "optiondialogwithlist.h"
#include "mainwindow.h"
#include "ui_optiondialogwithlist.h"
#include <memory>

OptionDialogWithList::OptionDialogWithList(QWidget *parent, ModelPart *part)
    : QDialog(parent), ui(new Ui::OptionDialogWithList),
      optionDialog(std::make_unique<OptionDialog>(this, part)),
      filterOption(std::make_unique<FilterOption>(this)) {

  ui->setupUi(this);
  ui->listWidget->setViewMode(QListWidget::ListMode);
  const std::array<std::pair<QString, QWidget *>, 2> optionList = {
      {{"General", optionDialog.get()}, {"Filters", filterOption.get()}}};
  for (const auto &option : optionList) {
    auto item = new QListWidgetItem(option.first);
    item->setData(Qt::UserRole, QVariant::fromValue(option.second));
    ui->listWidget->addItem(item);
  }

  connect(ui->listWidget, &QListWidget::itemSelectionChanged, this,
          &OptionDialogWithList::handleListClick);
  ui->listWidget->setCurrentRow(0);
  ui->tabWidget->addTab(optionList[0].second, optionList[0].first);
  adjustSize();
}

void OptionDialogWithList::handleListClick() {
  QList<QListWidgetItem *> item = ui->listWidget->selectedItems();

  if (item.isEmpty())
    return;

  QWidget *widget = qvariant_cast<QWidget *>(item[0]->data(Qt::UserRole));

  ui->tabWidget->clear();
  ui->tabWidget->addTab(widget, item[0]->text());
  adjustSize();
}

OptionDialogWithList::~OptionDialogWithList() { delete ui; }

#include "optiondialogwithlist.h"
#include "mainwindow.h"
#include "ui_optiondialogwithlist.h"
#include <QPushButton>
#include <memory>
#include <qabstractbutton.h>
#include <qdialogbuttonbox.h>

OptionDialogWithList::OptionDialogWithList(QWidget *parent, ModelPart *part)
    : QDialog(parent), ui(new Ui::OptionDialogWithList),
      optionDialog(std::make_unique<OptionDialog>(this, part)),
      filterOption(std::make_unique<FilterOption>(this, part)) {

  ui->setupUi(this);
  filterOption->hide();
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
  resize(sizeHint());

  connect(ui->buttonBox, &QDialogButtonBox::clicked, this,
          &OptionDialogWithList::SetValue);
}

void OptionDialogWithList::SetValue(QAbstractButton *button) {
  QDialogButtonBox::ButtonRole role = ui->buttonBox->buttonRole(button);
  if (role == QDialogButtonBox::AcceptRole ||
      role == QDialogButtonBox::ApplyRole) {
    optionDialog->SetValue();
    filterOption->SetValue();
  }
}

void OptionDialogWithList::handleListClick() {
  QList<QListWidgetItem *> item = ui->listWidget->selectedItems();

  if (item.isEmpty())
    return;

  QWidget *widget = qvariant_cast<QWidget *>(item[0]->data(Qt::UserRole));

  ui->tabWidget->clear();
  ui->tabWidget->addTab(widget, item[0]->text());
  adjustSize();
  resize(sizeHint());
}

OptionDialogWithList::~OptionDialogWithList() { delete ui; }

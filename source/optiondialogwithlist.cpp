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

  // Hide the widgets so that it doesn't appear on the dialog
  filterOption->hide();
  optionDialog->hide();

  // Set viewMode of listWidget to list mode. This will arrange the item from
  // top to bottom and disable movement on the list.
  ui->listWidget->setViewMode(QListWidget::ListMode);

  // Create array of widgets and it's name and set it to listWidget
  const std::array<std::pair<QString, QWidget *>, 2> optionList = {
      {{"General", optionDialog.get()}, {"Filters", filterOption.get()}}};
  for (const auto &option : optionList) {
    auto item = new QListWidgetItem(option.first);
    item->setData(Qt::UserRole, QVariant::fromValue(option.second));
    ui->listWidget->addItem(item);
  }

  // Connect the UI with slot
  connect(ui->buttonBox, &QDialogButtonBox::clicked, this,
          &OptionDialogWithList::SetValue);
  connect(ui->listWidget, &QListWidget::itemSelectionChanged, this,
          &OptionDialogWithList::handleListClick);

  // Set the default widget to General by default
  ui->listWidget->setCurrentRow(0);
  ui->tabWidget->addTab(optionList[0].second, optionList[0].first);
  adjustSize();
  resize(sizeHint());
}

void OptionDialogWithList::SetValue(QAbstractButton *button) {
  // Get the button role of the activated button and only set value if it's
  // AcceptRole (Ok) or ApplyRole (Apply)
  QDialogButtonBox::ButtonRole role = ui->buttonBox->buttonRole(button);
  if (role == QDialogButtonBox::AcceptRole ||
      role == QDialogButtonBox::ApplyRole) {
    optionDialog->SetValue();
    filterOption->SetValue();
  }
}

void OptionDialogWithList::handleListClick() {
  // Get the item and set the correct widget to tabWidget
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

#include "filteroption.h"
#include "Filters/FilterOptionInterface.h"
#include "Filters/clipfilteroption.h"
#include "Filters/shrinkfilteroption.h"
#include "ui_filteroption.h"
#include <qmenu.h>
#include <qnamespace.h>
#include <qtoolbutton.h>
#include <vtkPlane.h>
#include <vtkShrinkFilter.h>

using namespace Filter;

FilterOption::FilterOption(QWidget *parent, ModelPart *part)
    : QWidget(parent), ui(new Ui::FilterOption), part(part) {
  ui->setupUi(this);

  // Retrive a copy of filterList.
  filterList = part->getFilterList();
  // Create list from filterList.
  for (int i = 0; i < filterList.size(); i++) {
    QString filterName = "";
    auto item = new QListWidgetItem();
    switch (filterList[i].filterType) {
    case FilterType::ClipFilter: {

      filterName = "Clip Filter";
      item->setText(filterName);
      // Need to use raw pointer here because tabWidget can't use std::memory
      // and QSharedPointer.
      QWidget *clipWidget =
          new ClipFilterOption(this, filterList[i].filterPointer);
      clipWidget->hide();
      item->setData(Qt::UserRole,
                    QList<QVariant>({i, QVariant::fromValue(clipWidget)}));
      break;
    }
    case FilterType::ShrinkFilter: {
      filterName = "Shrink Filter";
      item->setText(filterName);
      QWidget *shrinkWidget =
          new ShrinkFilterOption(this, filterList[i].filterPointer);
      shrinkWidget->hide();
      item->setData(Qt::UserRole,
                    QList<QVariant>({i, QVariant::fromValue(shrinkWidget)}));
      break;
    }
    }
    ui->listWidget->addItem(item);
  }

  if (ui->listWidget->count() > 0) {
    ui->listWidget->setCurrentRow(0);
    auto firstItem = ui->listWidget->currentItem();
    QList<QVariant> firstItemList =
        qvariant_cast<QList<QVariant>>(firstItem->data(Qt::UserRole));
    QWidget *firstWidget = qvariant_cast<QWidget *>(firstItemList[1]);
    ui->tabWidget->addTab(firstWidget, firstItem->text());
  }
  adjustSize();
  resize(sizeHint());

  connect(ui->listWidget, &QListWidget::itemSelectionChanged, this,
          &FilterOption::handleListClick);

  menu = new QMenu();
  menu->addAction(ui->actionAdd_clip_filter);
  menu->addAction(ui->actionAdd_shrink_filter);

  ui->addButton->setMenu(menu);
  connect(ui->removeButton, &QToolButton::released, this,
          &FilterOption::handleRemoveButton);
}

void FilterOption::handleRemoveButton() {
  qDebug() << "Remove button triggered";
  if (ui->listWidget->currentRow() < 0) {
    qDebug() << "Current row less than zero, not removing any data";
    return;
  }
  int currentRow = ui->listWidget->currentRow();
  auto item = ui->listWidget->takeItem(currentRow);
  QList<QVariant> list =
      qvariant_cast<QList<QVariant>>(item->data(Qt::UserRole));
  QWidget *widget = qvariant_cast<QWidget *>(list[1]);
  delete widget;

  for (int i = currentRow; i < ui->listWidget->count(); i++) {
    qDebug() << "Updated index: " << i;
    auto list = qvariant_cast<QList<QVariant>>(item->data(Qt::UserRole));
    list[0] = i - 1;
    item->setData(Qt::UserRole, list);
  }
  int index = qvariant_cast<int>(list[0]);
  filterList.erase(filterList.begin() + index);
  delete item;
}

void FilterOption::SetValue() {
  // TODO: Loop through every widget in list and call SetValue();
  for (int i = 0; i < ui->listWidget->count(); i++) {
    auto item = ui->listWidget->item(i);
    QList<QVariant> list =
        qvariant_cast<QList<QVariant>>(item->data(Qt::UserRole));
    QWidget *widget = qvariant_cast<QWidget *>(list[1]);

    FilterOptionInterface *filterOption =
        dynamic_cast<FilterOptionInterface *>(widget);
    if (filterOption != nullptr) {
      filterOption->SetValue();
    }
  }
  part->setFilterList(filterList);
  part->setFilterFromList();
}

void FilterOption::handleListClick() {
  QList<QListWidgetItem *> item = ui->listWidget->selectedItems();

  if (item.isEmpty())
    return;

  QList<QVariant> list =
      qvariant_cast<QList<QVariant>>(item[0]->data(Qt::UserRole));
  QWidget *widget = qvariant_cast<QWidget *>(list[1]);

  ui->tabWidget->clear();
  ui->tabWidget->addTab(widget, item[0]->text());
  adjustSize();
  resize(sizeHint());
}

FilterOption::~FilterOption() {
  for (int i = 0; i < ui->listWidget->count(); i++) {
    auto item = ui->listWidget->item(i);
    QList<QVariant> list =
        qvariant_cast<QList<QVariant>>(item->data(Qt::UserRole));
    QWidget *widget = qvariant_cast<QWidget *>(list[1]);
    delete widget;
  }
  delete menu;
  delete ui;
}

void FilterOption::on_actionAdd_clip_filter_triggered() {
  qDebug() << "Add clip filter triggered";
  vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
  plane->SetOrigin(0, 0, 0);
  plane->SetNormal(0, 0, 0);

  vtkSmartPointer<vtkClipDataSet> clipFilter =
      vtkSmartPointer<vtkClipDataSet>::New();
  clipFilter->SetClipFunction(plane);
  filterList.push_back({Filter::FilterType::ClipFilter, clipFilter});

  QString filterName = "";
  auto item = new QListWidgetItem();

  filterName = "Clip Filter";
  item->setText(filterName);
  // Need to use raw pointer here because tabWidget can't use std::memory
  // and QSharedPointer.
  QWidget *clipWidget = new ClipFilterOption(this, clipFilter);
  int index = filterList.size() - 1;
  clipWidget->hide();
  item->setData(Qt::UserRole,
                QList<QVariant>({index, QVariant::fromValue(clipWidget)}));
  ui->listWidget->addItem(item);
}

void FilterOption::on_actionAdd_shrink_filter_triggered() {
  qDebug() << "Add shrink filter triggered";
  vtkSmartPointer<vtkShrinkFilter> shrinkFilter =
      vtkSmartPointer<vtkShrinkFilter>::New();
  shrinkFilter->SetShrinkFactor(1.);
  filterList.push_back({Filter::FilterType::ShrinkFilter, shrinkFilter});

  QString filterName = "";
  auto item = new QListWidgetItem();

  filterName = "Shrink Filter";
  item->setText(filterName);
  // Need to use raw pointer here because tabWidget can't use std::memory
  // and QSharedPointer.
  QWidget *clipWidget = new ShrinkFilterOption(this, shrinkFilter);
  int index = filterList.size() - 1;
  clipWidget->hide();
  item->setData(Qt::UserRole,
                QList<QVariant>({index, QVariant::fromValue(clipWidget)}));
  ui->listWidget->addItem(item);
}

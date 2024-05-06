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

  // Create list of widget from filterList.
  for (auto &filter : filterList) {
    QString filterName = "";
    auto item = new QListWidgetItem();
    switch (filter.filterType) {
    case FilterType::ClipFilter: {
      filterName = "Clip Filter";
      item->setText(filterName);
      // Need to use raw pointer here because tabWidget can't use std::memory
      // and QSharedPointer.
      QWidget *clipWidget = new ClipFilterOption(this, filter.filterPointer);
      clipWidget->hide(); // Hiding the widget is required to stop the widget
                          // from rendering on top of dialog.
      item->setData(Qt::UserRole, QVariant::fromValue(clipWidget));
      break;
    }

    case FilterType::ShrinkFilter: {
      filterName = "Shrink Filter";
      item->setText(filterName);
      // Same here
      QWidget *shrinkWidget =
          new ShrinkFilterOption(this, filter.filterPointer);
      shrinkWidget->hide(); // Hiding the widget is required to stop the widget
                            // from rendering on top of dialog.
      item->setData(Qt::UserRole, QVariant::fromValue(shrinkWidget));
      break;
    }
    }
    ui->listWidget->addItem(item);
  }

  // Set the current widget to the first one in the list if the list has
  // something inside it.
  if (ui->listWidget->count() > 0) {
    ui->listWidget->setCurrentRow(0);
    auto firstItem = ui->listWidget->currentItem();
    QWidget *firstWidget =
        qvariant_cast<QWidget *>(firstItem->data(Qt::UserRole));
    ui->tabWidget->addTab(firstWidget, firstItem->text());
  }

  adjustSize();
  resize(sizeHint());

  // Menu for adding filters
  menu = new QMenu();
  menu->addAction(ui->actionAdd_clip_filter);
  menu->addAction(ui->actionAdd_shrink_filter);
  ui->addButton->setMenu(menu);

  // Connect UI elements to the handler.
  connect(ui->listWidget, &QListWidget::itemSelectionChanged, this,
          &FilterOption::handleListClick);
  connect(ui->removeButton, &QToolButton::released, this,
          &FilterOption::handleRemoveButton);
}

void FilterOption::handleRemoveButton() {
  // Should remove the filter from list and the UI representation of that
  // filter.
  qDebug() << "Remove button triggered";
  if (ui->listWidget->currentRow() < 0) {
    qDebug() << "Current row less than zero, not removing any data";
    return;
  }
  int currentRow = ui->listWidget->currentRow();
  auto item = ui->listWidget->takeItem(currentRow);
  QWidget *widget = qvariant_cast<QWidget *>(item->data(Qt::UserRole));
  delete widget;

  filterList.erase(filterList.begin() + currentRow);
  delete item;
}

void FilterOption::SetValue() {
  // Loop through every widget in list and call SetValue();
  for (int i = 0; i < ui->listWidget->count(); i++) {
    auto item = ui->listWidget->item(i);

    // Need to cast twice here since qvariant_cast can only cast to class that
    // are derived from QVariant.
    QWidget *widget = qvariant_cast<QWidget *>(item->data(Qt::UserRole));
    FilterOptionInterface *filterOption =
        dynamic_cast<FilterOptionInterface *>(widget);

    if (filterOption != nullptr) {
      filterOption->SetValue();
    }
  }

  // This is for copying the filter list with new data to the part and setting
  // filter chain up.
  part->setFilterList(filterList);
  part->setFilterChainFromList();
}

void FilterOption::handleListClick() {
  // Get the selected item and change the UI to the one associated with the
  // item.
  QList<QListWidgetItem *> item = ui->listWidget->selectedItems();

  if (item.isEmpty())
    return;

  QWidget *widget = qvariant_cast<QWidget *>(item[0]->data(Qt::UserRole));

  ui->tabWidget->clear();
  ui->tabWidget->addTab(widget, item[0]->text());
  adjustSize();
  resize(sizeHint());
}

FilterOption::~FilterOption() {
  // Delete all of the manually allocated pointers

  // Delete the child widgets
  for (int i = 0; i < ui->listWidget->count(); i++) {
    auto item = ui->listWidget->item(i);
    QWidget *widget = qvariant_cast<QWidget *>(item->data(Qt::UserRole));
    delete widget;
  }

  // Delete menu
  delete menu;
  delete ui;
}

void FilterOption::on_actionAdd_clip_filter_triggered() {
  // Add new clip filter to the list, also add new widget to the listWidget.
  qDebug() << "Add clip filter triggered";
  vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
  plane->SetOrigin(0, 0, 0);
  plane->SetNormal(0, 0, 0);

  vtkSmartPointer<vtkClipDataSet> clipFilter =
      vtkSmartPointer<vtkClipDataSet>::New();
  clipFilter->SetClipFunction(plane);
  filterList.push_back({Filter::FilterType::ClipFilter, clipFilter});

  auto item = new QListWidgetItem();
  item->setText("Clip Filter");
  QWidget *clipWidget = new ClipFilterOption(this, clipFilter);
  clipWidget->hide();
  item->setData(Qt::UserRole, QVariant::fromValue(clipWidget));
  ui->listWidget->addItem(item);
}

void FilterOption::on_actionAdd_shrink_filter_triggered() {
  // Add new shrink filter to the list with widgets.
  qDebug() << "Add shrink filter triggered";
  vtkSmartPointer<vtkShrinkFilter> shrinkFilter =
      vtkSmartPointer<vtkShrinkFilter>::New();
  shrinkFilter->SetShrinkFactor(1.);
  filterList.push_back({Filter::FilterType::ShrinkFilter, shrinkFilter});

  auto item = new QListWidgetItem();
  item->setText("Shrink Filter");
  QWidget *clipWidget = new ShrinkFilterOption(this, shrinkFilter);
  clipWidget->hide();
  item->setData(Qt::UserRole, QVariant::fromValue(clipWidget));
  ui->listWidget->addItem(item);
}

#include "clipfilteroption.h"
#include "ui_clipfilteroption.h"
#include <qspinbox.h>
#include <vtkClipDataSet.h>
#include <vtkImplicitFunction.h>
#include <vtkPlane.h>
#include <vtkSmartPointer.h>

ClipFilterOption::ClipFilterOption(QWidget *parent,
                                   vtkSmartPointer<vtkAlgorithm> filterPointer)
    : QWidget(parent), ui(new Ui::ClipFilterOption) {
  ui->setupUi(this);

  filter = vtkClipDataSet::SafeDownCast(filterPointer);

  vtkSmartPointer<vtkImplicitFunction> clipFunction = filter->GetClipFunction();

  vtkSmartPointer<vtkPlane> plane = vtkPlane::SafeDownCast(clipFunction);

  double origin[3] = {0};
  plane->GetOrigin(origin);

  qDebug() << "Origin: " << origin;
  // Assume X is first, Y is second, Z is third
  originX = origin[0];
  originY = origin[1];
  originZ = origin[2];

  double normal[3] = {0};
  plane->GetNormal(normal);

  normalX = normal[0];
  normalY = normal[1];
  normalZ = normal[2];

  ui->normalX->setValue(normalX);
  ui->normalY->setValue(normalY);
  ui->normalZ->setValue(normalZ);
  ui->originX->setValue(originX);
  ui->originY->setValue(originY);
  ui->originZ->setValue(originZ);

  connect(ui->normalX, &QDoubleSpinBox::valueChanged, this,
          &ClipFilterOption::handleNormalX);
  connect(ui->normalY, &QDoubleSpinBox::valueChanged, this,
          &ClipFilterOption::handleNormalY);
  connect(ui->normalZ, &QDoubleSpinBox::valueChanged, this,
          &ClipFilterOption::handleNormalZ);
  connect(ui->originX, &QDoubleSpinBox::valueChanged, this,
          &ClipFilterOption::handleOriginX);
  connect(ui->originY, &QDoubleSpinBox::valueChanged, this,
          &ClipFilterOption::handleOriginY);
  connect(ui->originZ, &QDoubleSpinBox::valueChanged, this,
          &ClipFilterOption::handleOriginZ);
}

void ClipFilterOption::handleNormalX(double value) { normalX = value; }

void ClipFilterOption::handleNormalY(double value) { normalY = value; }

void ClipFilterOption::handleNormalZ(double value) { normalZ = value; }

void ClipFilterOption::handleOriginX(double value) { originX = value; }

void ClipFilterOption::handleOriginY(double value) { originY = value; }

void ClipFilterOption::handleOriginZ(double value) { originZ = value; }

ClipFilterOption::~ClipFilterOption() { delete ui; }

void ClipFilterOption::SetValue() {
  vtkSmartPointer<vtkImplicitFunction> clipFunction = filter->GetClipFunction();

  vtkSmartPointer<vtkPlane> plane = vtkPlane::SafeDownCast(clipFunction);

  plane->SetOrigin(originX, originY, originZ);
  plane->SetNormal(normalX, normalY, normalZ);
}

#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QColorDialog>
#include <QDialog>
#include <QPalette>

OptionDialog::OptionDialog(QWidget *parent, ModelPart *part)
    : QDialog(parent), ui(new Ui::OptionDialog) {
  ui->setupUi(this);
  this->part = part;
  ui->lineEdit->setText(part->data(0).toString());
  ui->comboBox->setCurrentIndex(part->visible() ? 0 : 1);
  color = QColor(part->getColourR(), part->getColourG(), part->getColourB());
  ui->DispColour->setStyleSheet("background-color: " + color.name() +
                                "; border:none;");
}

OptionDialog::~OptionDialog() { delete ui; }

void OptionDialog::SetValue() {
  int R = color.red();
  int G = color.green();
  int B = color.blue();
  qDebug() << R << " " << G << " " << B;

  part->setColour(R, G, B);

  part->setVisible(ui->comboBox->currentIndex() == 0 ? true : false);

  part->set(0, ui->lineEdit->text());
}

void OptionDialog::on_ColourDialog_clicked() {
  QColor Dialogcolor = QColorDialog::getColor(Qt::white, this, "Choose Color");
  if (Dialogcolor.isValid()) {
    color = Dialogcolor;
    ui->DispColour->setStyleSheet("background-color: " + color.name() +
                                  "; border:none;");
  }
}

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

  // Set initial slider values
  roughnessValue = part->getRoughness();
  metallicValue = part->getMetallic();
  anisotropyValue = part->getAnisotropy();
  anisotropyRotationValue = part->getAnisotropyRotation();

  float max = 100.;

  qDebug() <<roughnessValue;
  qDebug() << static_cast<int>(part->getRoughness()*max);

  ui->MetallicSlider->setValue(static_cast<int>(part->getMetallic() * max));
  ui->RoughnessSlider->setValue(static_cast<int>(part->getRoughness() * max));
  ui->AnisotropySlider->setValue(static_cast<int>(part->getAnisotropy() * max));
  ui->AnisotropyRotation->setValue(static_cast<int>(part->getAnisotropyRotation() * max));

  connect(ui->RoughnessSlider, &QSlider::sliderMoved, this, &OptionDialog::on_RoughnessSlider_sliderMoved);
  connect(ui->MetallicSlider, &QSlider::sliderMoved, this, &OptionDialog::on_MetallicSlider_sliderMoved);
  connect(ui->AnisotropySlider, &QSlider::sliderMoved, this, &OptionDialog::on_AnisotropySlider_sliderMoved);
  connect(ui->AnisotropyRotation, &QSlider::sliderMoved, this, &OptionDialog::on_AnisotropyRotation_sliderMoved);

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

  qDebug() << roughnessValue;

  part->setRoughness(roughnessValue);

  qDebug() << part->getRoughness();
  part->setMetallic(metallicValue);
  part->setAnisotropy(anisotropyValue);
  part->setAnisotropyRotation(anisotropyRotationValue);

}

void OptionDialog::on_ColourDialog_clicked() {
  QColor Dialogcolor = QColorDialog::getColor(Qt::white, this, "Choose Color");
  if (Dialogcolor.isValid()) {
    color = Dialogcolor;
    ui->DispColour->setStyleSheet("background-color: " + color.name() +
                                  "; border:none;");
  }
}

void OptionDialog::on_RoughnessSlider_sliderMoved(int position) {
  roughnessValue = static_cast<float>(position) / 100;
  qDebug() << "This is Roughness: " << roughnessValue;
}

void OptionDialog::on_MetallicSlider_sliderMoved(int position) {
  metallicValue = static_cast<float>(position) / 100;
  qDebug() << "This is Metallic: " << metallicValue;
}

void OptionDialog::on_AnisotropySlider_sliderMoved(int position) {
  anisotropyValue = static_cast<float>(position) / 100;
  qDebug() << "This is Anisotrophy: " << anisotropyValue;
}

void OptionDialog::on_AnisotropyRotation_sliderMoved(int position) {
  anisotropyRotationValue = static_cast<float>(position) / 100;
  qDebug() << "This is AnisotropyRotation: " << anisotropyRotationValue;
}

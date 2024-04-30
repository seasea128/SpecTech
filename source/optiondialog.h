#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include "ModelPart.h"

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog {
  Q_OBJECT

public slots:

public:
  explicit OptionDialog(QWidget *parent = nullptr, ModelPart *part = nullptr);
  ~OptionDialog();

  void SetValue();
  void updateColorDisplay();

private slots:

  void on_ColourDialog_clicked();

  void on_RoughnessSlider_sliderMoved(int position);

  void on_MetallicSlider_sliderMoved(int position);

  void on_AnisotropySlider_sliderMoved(int position);

  void on_AnisotropyRotation_sliderMoved(int position);

private:
  Ui::OptionDialog *ui;
  ModelPart *part;
  QColor color;
};

#endif // OPTIONDIALOG_H

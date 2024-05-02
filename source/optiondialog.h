#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include "ModelPart.h"

#include <QDialog>

namespace Ui {
class OptionDialog;
}

/**
 * Dialog for changing options of ModelPart.
 */
class OptionDialog : public QDialog {
  Q_OBJECT

public slots:

public:
  /**
   * Constructor of OptionDialog.
   * @param parent is the parent of the dialog.
   * @param part is the current part that are being modified.
   */
  explicit OptionDialog(QWidget *parent = nullptr, ModelPart *part = nullptr);

  /**
   * Destructor of OptionDialog.
   */
  ~OptionDialog();

  /**
   * Set value to the part.
   */
  void SetValue();

private slots:

  /**
   * Slot for when color dialog button is clicked.
   */
  void on_ColourDialog_clicked();

  /**
   * Slot for when roughness slider is moved.
   */
  void on_RoughnessSlider_sliderMoved(int position);

  /**
   * Slot for when metallic slider is moved.
   */
  void on_MetallicSlider_sliderMoved(int position);

  /**
   * Slot for when anisotropy slider is moved.
   */
  void on_AnisotropySlider_sliderMoved(int position);

  /**
   * Slot for when anisotropy rotation slider is moved.
   */
  void on_AnisotropyRotation_sliderMoved(int position);

private:
  /**
   * The UI component of OptionDialog.
   */
  Ui::OptionDialog *ui;

  /**
   * ModelPart pointer to current part that are modifying.
   */
  ModelPart *part;

  /**
   * Color that is stored before actually applying to the part.
   */
  QColor color;
  float roughnessValue;
  float metallicValue;
  float anisotropyValue;
  float anisotropyRotationValue;
};

#endif // OPTIONDIALOG_H

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include "ModelPart.h"

#include <QDialog>


namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

        public slots:


public:
    explicit OptionDialog(QWidget *parent = nullptr, ModelPart* part = nullptr);
    ~OptionDialog();

    void SetValue();
    void updateColorDisplay();

private slots:

    void on_ColourDialog_clicked();

private:
    Ui::OptionDialog *ui;
    ModelPart* part;
    QColor color;
};

#endif // OPTIONDIALOG_H

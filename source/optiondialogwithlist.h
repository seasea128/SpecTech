#ifndef OPTIONDIALOGWITHLIST_H
#define OPTIONDIALOGWITHLIST_H

#include "optiondialog.h"
#include <QDialog>

namespace Ui {
class OptionDialogWithList;
}

class OptionDialogWithList : public QDialog {
  Q_OBJECT

public:
  explicit OptionDialogWithList(QWidget *parent = nullptr,
                                ModelPart *part = nullptr);
  ~OptionDialogWithList();

private slots:
  void handleListClick();

private:
  enum ListItem { General, Filters };
  Ui::OptionDialogWithList *ui;
  std::unique_ptr<OptionDialog> optionDialog;
};

#endif // OPTIONDIALOGWITHLIST_H

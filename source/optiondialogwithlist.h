#ifndef OPTIONDIALOGWITHLIST_H
#define OPTIONDIALOGWITHLIST_H

#include "filteroption.h"
#include "optiondialog.h"
#include <QDialog>
#include <qabstractbutton.h>

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
  void SetValue(QAbstractButton *button);

private:
  enum ListItem { General, Filters };
  Ui::OptionDialogWithList *ui;
  std::unique_ptr<OptionDialog> optionDialog;
  std::unique_ptr<FilterOption> filterOption;
};

#endif // OPTIONDIALOGWITHLIST_H

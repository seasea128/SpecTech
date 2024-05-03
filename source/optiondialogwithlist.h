#ifndef OPTIONDIALOGWITHLIST_H
#define OPTIONDIALOGWITHLIST_H

#include "filteroption.h"
#include "optiondialog.h"
#include <QDialog>
#include <qabstractbutton.h>

namespace Ui {
class OptionDialogWithList;
}

/**
 * Option dialog with list.
 * This is made to support multiple child widget, so that the UI can be changed
 * based on the item selected in listWidget.
 */
class OptionDialogWithList : public QDialog {
  Q_OBJECT

public:
  /**
   * Constructor for OptionDialogWithList.
   * @param parent is the parent that create this dialog.
   * @param part is the currently selected ModelPart.
   */
  explicit OptionDialogWithList(QWidget *parent = nullptr,
                                ModelPart *part = nullptr);

  /**
   * Destructor for OptionDialogWithList.
   */
  ~OptionDialogWithList();

private slots:
  /**
   * Slot for handling listWidget click signal.
   * Change the widget based on what is currently selected.
   */
  void handleListClick();

  /**
   * Slot for handling buttonBox with multiple button.
   * This is for the OK and Cancel button the dialog box.
   */
  void SetValue(QAbstractButton *button);

private:
  /**
   * Enum of item inside the list.
   */
  enum ListItem { General, Filters };

  /**
   * UI component of the widget.
   */
  Ui::OptionDialogWithList *ui;

  /**
   * Unique pointer to OptionDialog.
   */
  std::unique_ptr<OptionDialog> optionDialog;

  /**
   * Unique pointer to FilterOption.
   */
  std::unique_ptr<FilterOption> filterOption;
};

#endif // OPTIONDIALOGWITHLIST_H

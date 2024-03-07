#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ModelPart.h"
#include "ModelPartList.h"
#include "RenderThread/RenderThread.h"
#include <qfiledialog.h>

#include <QMainWindow>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public slots:
  // A slot is a function handled in response to a signal from a widget
  void handleButton();
  void handleButton_2();
  void handleTreeClicked();
  void on_actionItem_Options_triggered();

signals:
  void statusUpdateMessage(const QString &message, int timeout);

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  ModelPart *GetSelectedPart();
  void updateRender();
  void updateRenderFromTree(const QModelIndex &index);
  void scaleToFit(vtkRenderer *renderer);

private slots:
  void on_actionOpen_File_triggered();

  void on_actionOpen_VR_triggered();

  void on_actionOpenDir_triggered();

private:
  void loadToRenderThread(ModelPart *part);
  Ui::MainWindow *ui;
  ModelPartList *partList;
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
  RenderThread *renderThread;
};

#endif // MAINWINDOW_H

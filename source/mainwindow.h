#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ModelPart.h"
#include "ModelPartList.h"
#include "RenderThread/RenderThread.h"
#include <qfiledialog.h>

#include <QMainWindow>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkHDRReader.h>
#include <vtkOpenGLRenderer.h>
#include <vtkSkybox.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * Main window of the program
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

public slots:
  // A slot is a function handled in response to a signal from a widget

  /**
   * Slot for handling button pressed.
   */
  void handleButton();

  /**
   * Slot for handling modify part button.
   */
  void handleModifyPartButton();

  /**
   * Slot for handling clicking on TreeView. It will change the UI state
   * according to selected part.
   */
  void handleTreeClicked();

  /**
   * Slot for Item Option action, will run handleModifyPartButton.
   */
  void on_actionItem_Options_triggered();

signals:

  /**
   *  Signal for updating status message.
   *  @param message is the new message.
   *  @param timeout is the amount of time it take to disappear.
   */
  void statusUpdateMessage(const QString &message, int timeout);

public:
  /**
   * Constructor for MainWindow
   *
   * @param parent is the parent QWidget. (Here it is nullptr because
   * MainWindow always is the top level QWidget)
   */
  MainWindow(QWidget *parent = nullptr);

  /**
   * Destructor for MainWindow
   */
  ~MainWindow();

  /**
   * Function that gets the currently selected part in TreeView.
   * @return Part that is currently selected.
   */
  ModelPart *GetSelectedPart();

  /**
   * Function that update the renderer by deleting all actor and readding it.
   */
  void updateRender();

  /**
   * Function that update renderer with actor from TreeView recursively.
   * @param index is the index to recurse from.
   */
  void updateRenderFromTree(const QModelIndex &index);

  /**
   * Function that scales the camera to fit the models.
   * @param renderer is the renderer that need to fit the models.
   */
  void scaleToFit(vtkRenderer *renderer);

private slots:
  /**
   * Slot for handling open file action.
   */
  void on_actionOpen_File_triggered();

  /**
   * Slot for handling open VR action.
   */
  void on_actionOpen_VR_triggered();

  /**
   * Slot for handling open directory action.
   */
  void on_actionOpenDir_triggered();

  /**
   * Slot for handling red slider movement.
   */
  void on_Slider_R_sliderMoved(int position);

  /**
   * Slot for handling green slider movement.
   */
  void on_Slider_G_sliderMoved(int position);

  /**
   * Slot for handling blue slider movement.
   */
  void on_Slider_B_sliderMoved(int position);

  /**
   * Slot for handling delete action.
   */
  void on_actiondelete_triggered();

  /**
   * Slot for handling stop VR action.
   */
  void on_actionstopbutton_triggered();

  /**
   * Slot for handling rotation speed change in X axis
   * @param position is the new position of the slider
   */
  void on_XRotSpeed_sliderMoved(int position);

  /**
   * Slot for handling rotation speed change in Y axis
   * @param position is the new position of the slider
   */
  void on_YRotSpeed_sliderMoved(int position);

  /**
   * Slot for handling rotation speed change in Z axis
   * @param position is the new position of the slider
   */
  void on_ZRotSpeed_sliderMoved(int position);

private:
  /**
   * Function for recursively scan and load actor to RenderThread from TreeView.
   * @param part is the pointer to current ModelPart.
   */
  void loadToRenderThread(ModelPart *part);

  /**
   * Function for splitting path and return the file/folder name without the
   * path.
   * @param fileName is the path that need to be split.
   * @return Folder name or file name without the path.
   */
  QString splitFileName(const QString &fileName);

  /**
   * Function for recursively searching given directory and add any STL files to
   * the list.
   * @param dir is the starting directory of the recursive search.
   * @param root is where the new loaded ModelPart will be inserted into.
   */
  void recursiveDirSearch(QFileInfoList dir, ModelPart *root);

  /**
   * Function for updating colour of currently selected ModelPart.
   */
  void updateColour();

  /**
   * Function for updating the renderer. This is needed when the actor is
   * updated because Qt doesn't know when it need to redraw the widget.
   */
  void ReRender();

  /**
   * Function for loading the skybox and enabling PBR rendering.
   * @param hdr_fileName is the filename of the skybox that are used.
   */
  void loadPBR(std::string const &hdr_fileName);

private:
  /**
   * The UI part of MainWindow.
   */
  Ui::MainWindow *ui;

  /**
   * ModelPartList to store the part that can be visualize in TreeView.
   */
  ModelPartList *partList;

  /**
   * Smart pointer to the skybox that is used for lighting.
   */
  vtkSmartPointer<vtkSkybox> skybox;

  /**
   * Smart pointer to the HDR reader that is used to load the skybox.
   */
  vtkSmartPointer<vtkHDRReader> reader;

  /**
   * Smart pointer to the renderer for Qt GUI.
   */
  vtkSmartPointer<vtkOpenGLRenderer> renderer;

  /**
   * Smart pointer to the render window for Qt GUI.
   */
  vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

  /**
   * Pointer to renderThread for VR.
   */
  RenderThread *renderThread;

  /**
   * File name of the skybox in HDR format.
   */
  std::string hdr_fileName;

  float XSpeed;
  float YSpeed;
  float ZSpeed;
};

#endif // MAINWINDOW_H

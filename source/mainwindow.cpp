#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRRenderer.h>

#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "optiondialog.h"
#include <QFile>
#include <QMessageBox>
#include <openvr.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkLight.h>
#include <vtkLightActor.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), renderThread(nullptr) {

  ui->setupUi(this);
  connect(ui->pushButton, &QPushButton::released, this,
          &MainWindow::handleButton);
  connect(ui->ModifyPart, &QPushButton::released, this,
          &MainWindow::handleButton_2);
  connect(ui->treeView, &QTreeView::clicked, this,
          &MainWindow::handleTreeClicked);
  connect(this, &MainWindow::statusUpdateMessage, ui->statusbar,
          &QStatusBar::showMessage);

  this->partList = new ModelPartList("PartsList");
  ui->treeView->setModel(this->partList);
  ui->treeView->addAction(ui->actionItem_Options);
  ui->treeView->addAction(ui->actiondelete);
  ModelPart *rootItem = this->partList->getRootItem();

  /* This needs adding to MainWindow constructor */
  /* Link a render windows with the Qt widget */
  renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  ui->vtkWidget->setRenderWindow(renderWindow);

  /* Add a renderer */
  renderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(renderer);

  vtkNew<vtkNamedColors> colors;

  double lightPosition[3] = {0, 0, 1};

  // Create a light
  double lightFocalPoint[3] = {0, 0, 0};

  vtkNew<vtkLight> light;
  light->SetLightTypeToSceneLight();
  light->SetPosition(lightPosition[0], lightPosition[1], lightPosition[2]);
  light->SetPositional(true); // required for vtkLightActor below
  light->SetConeAngle(10);
  light->SetFocalPoint(lightFocalPoint[0], lightFocalPoint[1],
                       lightFocalPoint[2]);
  light->SetDiffuseColor(colors->GetColor3d("Red").GetData());
  light->SetAmbientColor(colors->GetColor3d("Green").GetData());
  light->SetSpecularColor(colors->GetColor3d("Blue").GetData());

  // Display where the light is
  // vtkNew<vtkLightActor> lightActor;
  // lightActor->SetLight(light);
  // renderer->AddViewProp(lightActor);

  // Display where the light is focused
  vtkNew<vtkSphereSource> lightFocalPointSphere;
  lightFocalPointSphere->SetCenter(lightFocalPoint);
  lightFocalPointSphere->SetRadius(0.1);
  lightFocalPointSphere->Update();

  // Create an object and add to renderer (this will change later to display a
  // CAD model) Will just copy and paste  cylinder example from before This
  // creates a polygonal cylinder model with eight circumferential facets (i.e.
  // in pratice an octagonal prism)
  vtkNew<vtkCylinderSource> cylinder;
  cylinder->SetResolution(8);

  // The mapper is responsibile for pushing the geometry into the graphics
  // library. It may also do color mapping. If scalars or other attributes are
  // defined.
  vtkNew<vtkPolyDataMapper> cylinderMapper;
  cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

  vtkNew<vtkActor> cylinderActor;
  cylinderActor->SetMapper(cylinderMapper);
  cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
  cylinderActor->RotateX(30.0);
  cylinderActor->RotateY(-45.0);

  renderer->AddActor(cylinderActor);

  // Reset Camera(probably needs to go in its own funcion that is called
  // whenever model is changed
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Azimuth(30);
  renderer->GetActiveCamera()->Elevation(30);
  // renderer->GetActiveCamera()->ResetCameraClippingRange();
}

MainWindow::~MainWindow() {
  delete ui;
  if (renderThread != nullptr) {
    if (renderThread->isRunning()) {
      renderThread->exit();
      while (renderThread->isRunning()) {
      }
    }
    delete renderThread;
  }
}

void MainWindow::handleButton() {
  QMessageBox msgBox;
  msgBox.setText("Add button was clicked");
  msgBox.exec();
  emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

void MainWindow::handleButton_2() {
  if (GetSelectedPart() == nullptr) {
    emit statusUpdateMessage(QString("Part hasn't been selected yet"), 0);
    return;
  }
  OptionDialog dialog(this, GetSelectedPart());

  if (dialog.exec() == QDialog::Accepted) {
    dialog.SetValue();
    emit statusUpdateMessage(
        QString("Dialog accepted ") + GetSelectedPart()->data(0).toString(), 0);
    ReRender();
  } else {
    emit statusUpdateMessage(QString("Dialog rejected"), 0);
  }
}

void MainWindow::on_actionItem_Options_triggered() { handleButton_2(); }

void MainWindow::handleTreeClicked() {

  // Get the index of the selected item
  QModelIndex index = ui->treeView->currentIndex();

  // Get a pointer to the item from the index
  ModelPart *selectedPart = static_cast<ModelPart *>(index.internalPointer());

  // In this case, we will retrieve the name string from the internal QVariant
  // data array
  QString text = selectedPart->data(0).toString();

  emit statusUpdateMessage(QString("The selected item is: ") + text, 0);

  ui->Slider_R->setValue(selectedPart->getColourR());
  ui->Slider_G->setValue(selectedPart->getColourG());
  ui->Slider_B->setValue(selectedPart->getColourB());
}

ModelPart *MainWindow::GetSelectedPart() {

  // Get the index of the selected item
  QModelIndex index = ui->treeView->currentIndex();

  // Get a pointer to the item from the index
  ModelPart *selectedPart = static_cast<ModelPart *>(index.internalPointer());

  return selectedPart;
}

void MainWindow::updateRender() {
  renderer->RemoveAllViewProps();
  for (int i = 0; i < partList->rowCount(QModelIndex()); i++) {
    updateRenderFromTree(partList->index(i, 0, QModelIndex()));
  }
  ReRender();
  ui->treeView->expandAll();
  ui->treeView->update();
}

void MainWindow::on_actionOpen_File_triggered() {
  emit statusUpdateMessage(QString("Open File action triggered"), 0);
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));
  emit statusUpdateMessage(QString("File name: ") + fileName, 0);
  if (fileName == "") {
    return;
  }
  QString visible("true");
  auto part = new ModelPart({fileName, visible});
  part->loadSTL(fileName);
  if (GetSelectedPart() == nullptr) {
    partList->getRootItem()->appendChild(part);
  } else {
    GetSelectedPart()->appendChild(part);
  }
  updateRender();
}

void MainWindow::updateRenderFromTree(const QModelIndex &index) {
  if (index.isValid()) {
    ModelPart *selectedPart = static_cast<ModelPart *>(index.internalPointer());
    if (selectedPart->getActor() != nullptr && selectedPart->visible()) {
      renderer->AddActor(selectedPart->getActor());
    }
  }
  // Check to see if part has children
  if (!partList->hasChildren(index) ||
      (index.flags() & Qt::ItemNeverHasChildren)) {
    return;
  }
  int rows = partList->rowCount(index);
  for (int i = 0; i < rows; i++) {
    updateRenderFromTree(partList->index(i, 0, index));
  }
}

void MainWindow::scaleToFit(vtkRenderer *renderer) { renderer->ResetCamera(); }

void MainWindow::on_actionOpen_VR_triggered() {
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkCamera> camera;
  vtkSmartPointer<vtkRenderWindow> window;
  vtkSmartPointer<vtkRenderWindowInteractor> interactor;
  if (vr::VR_IsHmdPresent()) {
    renderer = vtkOpenVRRenderer::New();
    camera = vtkOpenVRCamera::New();
    vtkSmartPointer<vtkOpenVRRenderWindow> localWindow =
        vtkOpenVRRenderWindow::New();
    localWindow->Initialize();
    window = localWindow;
    interactor = vtkOpenVRRenderWindowInteractor::New();
  } else {
    renderer = vtkRenderer::New();
    camera = vtkCamera::New();
    window = vtkRenderWindow::New();
    interactor = vtkRenderWindowInteractor::New();
  }
  renderThread = new RenderThread(this, renderer, window, interactor, camera);
  ModelPart *root = this->partList->getRootItem();
  loadToRenderThread(root);
  renderThread->start();
}

QString MainWindow::splitFileName(const QString &fileName) {
  auto splittedFileName = fileName.split(QString('/'), Qt::KeepEmptyParts);
  QString displayFileName;
  if (splittedFileName.length() != 1) {
    displayFileName = splittedFileName[splittedFileName.length() - 1];
  } else {
    displayFileName = fileName;
  }
  return displayFileName;
}

void MainWindow::loadToRenderThread(ModelPart *part) {
  std::cout << "Child count: " << part->childCount() << std::endl;
  if (part->childCount() > 0) {
    for (int i = 0; i < part->childCount(); i++) {
      loadToRenderThread(part->child(i));
    }
  }
  vtkActor *actor = part->getNewActor();
  if (actor != nullptr) {
    renderThread->addActorOffline(actor);
  }
}

void MainWindow::on_actionOpenDir_triggered() {
  emit statusUpdateMessage(QString("Open Directory action triggered"), 0);
  QString directoryName =
      QFileDialog::getExistingDirectory(this, tr("Open Directory"), "C:\\");
  emit statusUpdateMessage(QString("Directory name: ") + directoryName, 0);
  if (directoryName.isEmpty()) {
    return;
  }
  QDir directory(directoryName);
  directory.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot |
                      QDir::NoSymLinks);
  // Filters specifically for stl files
  QStringList filters;
  filters << "*.stl";
  QFileInfoList selectedDir = directory.entryInfoList();
  ModelPart *root = partList->getRootItem();

  recursiveDirSearch(selectedDir, root);
  updateRender();
}

void MainWindow::recursiveDirSearch(QFileInfoList dir, ModelPart *root) {
  QString visible("true");
  for (const auto &file : dir) {
    QString absFilePath = file.absoluteFilePath();
    QString name = splitFileName(absFilePath);

    ModelPart *part = new ModelPart({name, visible});

    root->appendChild(part);

    if (file.isDir()) {
      QDir recurseDir = QDir(absFilePath);
      recurseDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot |
                           QDir::NoSymLinks);
      recursiveDirSearch(recurseDir.entryInfoList(), part);
    } else if (file.completeSuffix() == "stl") {
      part->loadSTL(absFilePath);
    } else {
      qDebug() << "File type is not stl";
    }
  }
}

void MainWindow::ReRender() {
  renderer->Render();
  scaleToFit(renderer);
  ui->vtkWidget->renderWindow()->Render();
  ui->vtkWidget->update();
}

void MainWindow::updateColour() {
  GetSelectedPart()->setColour(ui->Slider_R->value(), ui->Slider_G->value(),
                               ui->Slider_B->value());
  ReRender();
}

void MainWindow::on_Slider_R_sliderMoved(int position) { updateColour(); }

void MainWindow::on_Slider_G_sliderMoved(int position) { updateColour(); }

void MainWindow::on_Slider_B_sliderMoved(int position) { updateColour(); }

void MainWindow::on_actiondelete_triggered() {
  QModelIndex ind = ui->treeView->currentIndex();
  this->partList->removeItem(ind);
  updateRender();
}

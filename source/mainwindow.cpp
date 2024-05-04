// VTK includes
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkHDRReader.h>
#include <vtkLight.h>
#include <vtkLightActor.h>
#include <vtkNamedColors.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSkybox.h>
#include <vtkSphereSource.h>
#include <vtkTexture.h>

// OpenVR includes
#include <openvr.h>

// Commands and Utils includes
#include "RenderThread/Commands/AddActorCommand.h"
#include "RenderThread/Commands/EndRenderCommand.h"
#include "RenderThread/Commands/RefreshRenderCommand.h"
#include "RenderThread/Commands/RemoveActorCommand.h"
#include "RenderThread/Commands/UpdateColourCommand.h"
#include "RenderThread/Commands/UpdateFilterListCommand.h"
#include "RenderThread/Commands/UpdatePropertyCommand.h"
#include "RenderThread/Commands/UpdateVisibilityCommand.h"
#include "Utils.h"

#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "optiondialog.h"
#include "optiondialogwithlist.h"
#include <QFile>
#include <QMessageBox>

using namespace Commands;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), renderThread(nullptr) {

  ui->setupUi(this);
  connect(ui->pushButton, &QPushButton::released, this,
          &MainWindow::handleButton);
  connect(ui->ModifyPart, &QPushButton::released, this,
          &MainWindow::handleModifyPartButton);
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
  renderer = vtkSmartPointer<vtkOpenGLRenderer>::New();
  renderWindow->AddRenderer(renderer);

  hdr_fileName =
      (std::filesystem::current_path() / "skybox" / "rural_asphalt_road_4k.hdr")
          .string();
  loadPBR(hdr_fileName);

  vtkNew<vtkFrustumCoverageCuller> culler;
  renderer->AddCuller(culler);

  // Reset Camera(probably needs to go in its own funcion that is called
  // whenever model is changed
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Azimuth(30);
  renderer->GetActiveCamera()->Elevation(30);
  // renderer->GetActiveCamera()->ResetCameraClippingRange();
}

MainWindow::~MainWindow() {
  if (renderThread != nullptr) {
    std::shared_ptr<EndRenderCommand> endCommand =
        std::make_shared<EndRenderCommand>();
    renderThread->addCommand(endCommand);
    if (!renderThread->wait(3000)) {
      qDebug() << "Waited for 3 seconds, will terminate the render "
                  "thread";
      renderThread->terminate();
      renderThread->wait();
    }
  }
  delete ui;
}

void MainWindow::handleButton() {
  auto part = GetSelectedPart();
  if (part == nullptr) {
    emit statusUpdateMessage(QString("Part hasn't been selected yet"), 0);
    return;
  }
  OptionDialogWithList dialog(this, part);
  dialog.exec();
  if (renderThread != nullptr) {
    // TODO: Update property
    Utils::recursiveAddCommand<UpdatePropertyCommand>(renderThread, part);
    Utils::recursiveAddCommand<UpdateFilterListCommand>(renderThread, part);
  }
  ReRender();
  emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

void MainWindow::handleModifyPartButton() {
  if (GetSelectedPart() == nullptr) {
    emit statusUpdateMessage(QString("Part hasn't been selected yet"), 0);
    return;
  }
  OptionDialogWithList dialog(this, GetSelectedPart());

  if (dialog.exec() == QDialog::Accepted) {
    emit statusUpdateMessage(
        QString("Dialog accepted ") + GetSelectedPart()->data(0).toString(), 0);
    ReRender();
    if (renderThread != nullptr) {
      Utils::recursiveAddCommand<UpdatePropertyCommand>(renderThread,
                                                        GetSelectedPart());
      Utils::recursiveAddCommand<UpdateFilterListCommand>(renderThread,
                                                          GetSelectedPart());
    }
    ui->Slider_R->setValue(GetSelectedPart()->getColourR());
    ui->Slider_G->setValue(GetSelectedPart()->getColourG());
    ui->Slider_B->setValue(GetSelectedPart()->getColourB());
  } else {
    emit statusUpdateMessage(QString("Dialog rejected"), 0);
  }
}

void MainWindow::on_actionItem_Options_triggered() { handleModifyPartButton(); }

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

  // Add skybox back
  renderer->AddActor(skybox);

  for (int i = 0; i < partList->rowCount(QModelIndex()); i++) {
    updateRenderFromTree(partList->index(i, 0, QModelIndex()));
  }
  ReRender();
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
  QString splitted = splitFileName(fileName);
  auto part = new ModelPart({splitted, visible});
  part->loadSTL(fileName);
  if (GetSelectedPart() == nullptr) {
    partList->getRootItem()->appendChild(part);
  } else {
    GetSelectedPart()->appendChild(part);
  }

  if (renderThread != nullptr) {
    qDebug() << "RenderThread detected, adding actor to RenderThread";
    auto command = std::make_shared<AddActorCommand>(part->getNewActor());
    renderThread->addCommand(command);
  }

  updateRender();
  ui->treeView->expandAll();
  ui->treeView->update();
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
  if (renderThread != nullptr) {
    emit statusUpdateMessage("VR rendering already running, please stop "
                             "currently running rendering first.",
                             0);
    return;
  }
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
  renderThread = new RenderThread(this, renderer, window, interactor, camera,
                                  hdr_fileName);
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
  qDebug() << "Child count: " << part->childCount();
  if (part->childCount() > 0) {
    for (int i = 0; i < part->childCount(); i++) {
      loadToRenderThread(part->child(i));
    }
  }
  vtkSmartPointer<vtkActor> actor = part->getNewActor();
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
  ui->treeView->expandAll();
  ui->treeView->update();
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

      if (renderThread != nullptr) {
        auto command = std::make_shared<AddActorCommand>(part->getNewActor());
        renderThread->addCommand(command);
      }
    } else {
      qDebug() << "File type is not stl";
    }
  }
}

void MainWindow::ReRender() {
  scaleToFit(renderer);
  ui->vtkWidget->renderWindow()->Render();
  ui->vtkWidget->update();
  // if (renderThread != nullptr) {
  //   std::shared_ptr<RefreshRenderCommand> command =
  //       std::make_shared<RefreshRenderCommand>();
  //   renderThread->addCommand(command);
  // }
}

void MainWindow::updateColour() {
  auto currentPart = GetSelectedPart();
  if (currentPart == nullptr) {
    return;
  }
  currentPart->setColour(ui->Slider_R->value(), ui->Slider_G->value(),
                         ui->Slider_B->value());
  ReRender();

  // Send command to renderThread
  if (renderThread != nullptr) {
    Utils::recursiveAddCommand<UpdateColourCommand>(renderThread, currentPart);
    // auto refresh = std::make_shared<RefreshRenderCommand>();
    // renderThread->addCommand(refresh);
  }
}

void MainWindow::on_Slider_R_sliderMoved(int position) { updateColour(); }

void MainWindow::on_Slider_G_sliderMoved(int position) { updateColour(); }

void MainWindow::on_Slider_B_sliderMoved(int position) { updateColour(); }

void MainWindow::on_actiondelete_triggered() {
  QModelIndex ind = ui->treeView->currentIndex();
  if (renderThread != nullptr) {
    Utils::recursiveAddCommand<RemoveActorCommand>(
        renderThread, static_cast<ModelPart *>(ind.internalPointer()));
    auto command = std::make_shared<RefreshRenderCommand>();
    renderThread->addCommand(command);
  }
  this->partList->removeItem(ind);
  updateRender();
}

// Quit OpenVR (if OpenVR is open)
void MainWindow::on_actionstopbutton_triggered() {
  if (renderThread != nullptr) {
    std::shared_ptr<EndRenderCommand> endCommand =
        std::make_shared<EndRenderCommand>();
    renderThread->addCommand(endCommand);
    if (!renderThread->wait(3000)) {
      qDebug() << "Waited for 3 seconds, will terminate the render "
                  "thread";
      renderThread->terminate();
      renderThread->wait();
    }
    delete renderThread;
    renderThread = nullptr;
  }
}

void MainWindow::loadPBR(std::string const &hdr_fileName) {
  skybox = vtkSmartPointer<vtkSkybox>::New();
  vtkNew<vtkTexture> envTexture;
  reader = vtkSmartPointer<vtkHDRReader>::New();

  // Assume the file is an HDR file.
  if (reader->CanReadFile(hdr_fileName.c_str())) {
    reader->SetFileName(hdr_fileName.c_str());
    envTexture->SetInputConnection(reader->GetOutputPort());
    envTexture->SetColorModeToDirectScalars();
  } else {
    qDebug() << "Cannot read file:" << hdr_fileName;
  }

  envTexture->MipmapOn();
  envTexture->InterpolateOn();

  skybox->SetFloorRight(0, 0, 1);
  skybox->SetProjection(vtkSkybox::Sphere);
  skybox->SetTexture(envTexture);
  skybox->GammaCorrectOn();

  // Adding lighting to renderer
  renderer->UseImageBasedLightingOn();
  renderer->UseSphericalHarmonicsOn();
  renderer->SetEnvironmentTexture(envTexture, false);
  renderer->AddActor(skybox);
}

void MainWindow::on_XRotSpeed_sliderMoved(int position) {
  if (renderThread != nullptr) {
    renderThread->updateSpeedX(position);
    ui->XRotDisp->display(position);
  }
  return;
}

void MainWindow::on_YRotSpeed_sliderMoved(int position) {
  if (renderThread != nullptr) {
    renderThread->updateSpeedY(position);
    ui->YRotDisp->display(position);
  }
  return;
}

void MainWindow::on_ZRotSpeed_sliderMoved(int position) {
  if (renderThread != nullptr) {
    renderThread->updateSpeedZ(position);
    ui->ZRotDisp->display(position);
  }
  return;
}

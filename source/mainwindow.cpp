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
#include "Utils.h"

#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "optiondialogwithlist.h"
#include <QFile>
#include <QMessageBox>
#include "RenderThread/Commands/UpdateRotationSpeedCommand.h"

using namespace Commands;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), renderThread(nullptr), XSpeed(0.), YSpeed(0.), ZSpeed(0.) {

  ui->setupUi(this);

  // Connect the UI to handler.
  connect(ui->pushButton, &QPushButton::released, this,
          &MainWindow::handleButton);
  connect(ui->ModifyPart, &QPushButton::released, this,
          &MainWindow::handleModifyPartButton);
  connect(ui->treeView, &QTreeView::clicked, this,
          &MainWindow::handleTreeClicked);
  connect(this, &MainWindow::statusUpdateMessage, ui->statusbar,
          &QStatusBar::showMessage);

  // Create and bind part list
  this->partList = new ModelPartList("PartsList");
  ui->treeView->setModel(this->partList);

  // Bind action to treeView
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

  // Set skybox filename.
  hdr_fileName =
      (std::filesystem::current_path() / "skybox" / "rural_asphalt_road_4k.hdr")
          .string();
  loadPBR(hdr_fileName);

  // Add culler (I don't know if this is automatically added when creating
  // renderer, but this is supposedly will improve performance)
  vtkNew<vtkFrustumCoverageCuller> culler;
  renderer->AddCuller(culler);

  // Reset Camera (probably needs to go in its own funcion that is called
  // whenever model is changed)
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Azimuth(30);
  renderer->GetActiveCamera()->Elevation(30);
}

MainWindow::~MainWindow() {

  // Stop render thread before destructing
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
  handleModifyPartButton();
  emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

void MainWindow::handleModifyPartButton() {

  // Check if part is null or not
  if (GetSelectedPart() == nullptr) {
    emit statusUpdateMessage(QString("Part hasn't been selected yet"), 0);
    return;
  }

  // Create the dialog
  OptionDialogWithList dialog(this, GetSelectedPart());

  if (dialog.exec() == QDialog::Accepted) {
    emit statusUpdateMessage(
        QString("Dialog accepted ") + GetSelectedPart()->data(0).toString(), 0);

    // Rerender to update the actor on screen without clicking on the render
    // widget.
    ReRender();

    // Add command to renderThread to update all property and filter list inside
    // renderThread.
    if (renderThread != nullptr) {
      Utils::recursiveAddCommand<UpdatePropertyCommand>(renderThread,
                                                        GetSelectedPart());
      Utils::recursiveAddCommand<UpdateFilterListCommand>(renderThread,
                                                          GetSelectedPart());
    }

    // Update slider to new value
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
  // Remove everything from renderer
  renderer->RemoveAllViewProps();

  // Add skybox back
  renderer->AddActor(skybox);

  // Load actor from tree.
  for (int i = 0; i < partList->rowCount(QModelIndex()); i++) {
    updateRenderFromTree(partList->index(i, 0, QModelIndex()));
  }
  ReRender();
}

void MainWindow::on_actionOpen_File_triggered() {
  emit statusUpdateMessage(QString("Open File action triggered"), 0);

  // Get file name from the user.
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));
  emit statusUpdateMessage(QString("File name: ") + fileName, 0);

  // Check if file name is empty.
  if (fileName == "") {
    return;
  }

  // Default visibility.
  QString visible("true");

  // Since fileName is actually path, it need to be splitted to get only the
  // file name.
  QString splitted = splitFileName(fileName);

  // Create new ModelPart.
  auto part = new ModelPart({splitted, visible});
  part->loadSTL(fileName); // Load the file in.

  // Add the part as child based on currently selected part.
  if (GetSelectedPart() == nullptr) {
    partList->getRootItem()->appendChild(part);
  } else {
    GetSelectedPart()->appendChild(part);
  }

  // Send command to add new actor if renderThread exist.
  if (renderThread != nullptr) {
    auto command = std::make_shared<AddActorCommand>(part->getNewActor());
    renderThread->addCommand(command);
  }

  // Update renderer to include new actor.
  updateRender();

  // Expand the treeView to expose the new part that is being loaded.
  ui->treeView->expandAll();
  ui->treeView->update();
}

void MainWindow::updateRenderFromTree(const QModelIndex &index) {
  // If index contains ModelPart pointer, then the actor of that part will be
  // loaded into renderer.
  if (index.isValid()) {
    ModelPart *selectedPart = static_cast<ModelPart *>(index.internalPointer());
    if (selectedPart->getActor() != nullptr && selectedPart->visible()) {
      renderer->AddActor(selectedPart->getActor());
    }
  }

  // Check to see if part doesn't have children
  if (!partList->hasChildren(index) ||
      (index.flags() & Qt::ItemNeverHasChildren)) {
    return;
  }

  // Search child part for more part to load.
  int rows = partList->rowCount(index);
  for (int i = 0; i < rows; i++) {
    updateRenderFromTree(partList->index(i, 0, index));
  }
}

void MainWindow::scaleToFit(vtkRenderer *renderer) { renderer->ResetCamera(); }

void MainWindow::on_actionOpen_VR_triggered() {

  // Check if renderThread exist already.
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

  // Create different version of VTK component if HMD is detected.
  if (vr::VR_IsHmdPresent()) {
    renderer = vtkOpenVRRenderer::New();
    camera = vtkOpenVRCamera::New();

    vtkSmartPointer<vtkOpenVRRenderWindow> localWindow =
        vtkOpenVRRenderWindow::New();
    localWindow->Initialize(); // Need to call initialize here, otherwise it'll
                               // need to be cast to the derived class when
                               // everything is being set up.

    window = localWindow;
    interactor = vtkOpenVRRenderWindowInteractor::New();
  } else {
    renderer = vtkRenderer::New();
    camera = vtkCamera::New();
    window = vtkRenderWindow::New();
    interactor = vtkRenderWindowInteractor::New();
  }

  // Creaete new RenderThread.
  renderThread = new RenderThread(this, renderer, window, interactor, camera,
                                  hdr_fileName);

  // Load actor to RenderThread.
  ModelPart *root = this->partList->getRootItem();
  loadToRenderThread(root);
  renderThread->start();
}

QString MainWindow::splitFileName(const QString &fileName) {
  // Split file name on slash and return either original string or the last
  // value of the splitted list.
  auto splittedFileName = fileName.split(QString('/'), Qt::KeepEmptyParts);
  QString displayFileName;

  if (splittedFileName.length() > 1) {
    displayFileName = splittedFileName[splittedFileName.length() - 1];
  } else {
    displayFileName = fileName;
  }
  return displayFileName;
}

void MainWindow::loadToRenderThread(ModelPart *part) {
  // Load actor to RenderThread recursively.
  qDebug() << "Child count: " << part->childCount();
  for (int i = 0; i < part->childCount(); i++) {
    loadToRenderThread(part->child(i));
  }
  vtkSmartPointer<vtkActor> actor = part->getNewActor();
  if (actor != nullptr) {
    renderThread->addActorOffline(actor);
  }
}

void MainWindow::on_actionOpenDir_triggered() {
  emit statusUpdateMessage(QString("Open Directory action triggered"), 0);

  // Get directory name
  QString directoryName =
      QFileDialog::getExistingDirectory(this, tr("Open Directory"), "C:\\");
  emit statusUpdateMessage(QString("Directory name: ") + directoryName, 0);

  if (directoryName.isEmpty()) {
    return;
  }

  // Create QDir with the directory name.
  QDir directory(directoryName);

  // Set filters so that only directory and files are only accessible.
  directory.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot |
                      QDir::NoSymLinks);

  QFileInfoList selectedDir = directory.entryInfoList();
  ModelPart *root = partList->getRootItem();

  // Search directory recursively for files.
  recursiveDirSearch(selectedDir, root);
  updateRender();
  ui->treeView->expandAll();
  ui->treeView->update();
}

void MainWindow::recursiveDirSearch(QFileInfoList dir, ModelPart *parent) {
  QString visible("true");

  for (const auto &file : dir) {
    QString absFilePath = file.absoluteFilePath(); // Get absolute file path
    QString name = splitFileName(absFilePath); // Split path to get file namea

    // Create new part
    ModelPart *part = new ModelPart({name, visible});

    // Append to current parent
    parent->appendChild(part);

    // Continue search if the current file is a directory, load it if it's an
    // .stl file, and print some error if it's not stl file.
    if (file.isDir()) {
      QDir recurseDir = QDir(absFilePath);
      recurseDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot |
                           QDir::NoSymLinks);
      recursiveDirSearch(recurseDir.entryInfoList(), part);
    } else if (file.completeSuffix() == "stl") {
      part->loadSTL(absFilePath);

      // Also send command to RenderThread if it exist.
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
  // Update renderer so that new change to rendering apply immediately.
  scaleToFit(renderer);
  ui->vtkWidget->renderWindow()->Render();
  ui->vtkWidget->update();
}

void MainWindow::updateColour() {
  auto currentPart = GetSelectedPart();
  if (currentPart == nullptr) {
    return;
  }
  // Set colour to current part
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
  // Get current index
  QModelIndex ind = ui->treeView->currentIndex();
  if (renderThread != nullptr) {
    // Send remove actor command to RenderThread recursively and send refresh
    // command to reset the renderer on RenderThread.
    Utils::recursiveAddCommand<RemoveActorCommand>(
        renderThread, static_cast<ModelPart *>(ind.internalPointer()));
    auto command = std::make_shared<RefreshRenderCommand>();
    renderThread->addCommand(command);
  }
  this->partList->removeItem(ind);
  updateRender();
}

// Quit VR (if VR is open)
void MainWindow::on_actionstopbutton_triggered() {
  if (renderThread != nullptr) {
    auto endCommand = std::make_shared<EndRenderCommand>();
    renderThread->addCommand(endCommand);
    if (!renderThread->wait(3000)) {
      qDebug() << "Waited for 3 seconds, will terminate the render "
                  "thread";
      renderThread->terminate();
      renderThread->wait();
    }
    delete renderThread;
    renderThread = nullptr; // Delete doesn't set renderThread to nullptr, need
                            // to set it by yourself.
  }
}

void MainWindow::loadPBR(std::string const &hdr_fileName) {
  // Create new skybox and texture.
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

  // Set texture to the skybox
  skybox->SetFloorRight(0, 0, 1);
  skybox->SetProjection(vtkSkybox::Sphere);
  skybox->SetTexture(envTexture);
  skybox->GammaCorrectOn();

  // Adding image based lighting to renderer
  renderer->UseImageBasedLightingOn();
  renderer->UseSphericalHarmonicsOn();
  renderer->SetEnvironmentTexture(envTexture, false);
  renderer->AddActor(skybox);
}

void MainWindow::on_XRotSpeed_sliderMoved(int position) {
  // Update RenderThread with new speed
  XSpeed = position;
  if (renderThread != nullptr) {
    auto command = std::make_shared<UpdateRotationSpeedCommand>(XSpeed,YSpeed,ZSpeed);
    renderThread->addCommand(command);
    ui->XRotDisp->display(position);
  }
  return;
}

void MainWindow::on_YRotSpeed_sliderMoved(int position) {
  // Update RenderThread with new speed
  YSpeed = position;
  if (renderThread != nullptr) {
    auto command = std::make_shared<UpdateRotationSpeedCommand>(XSpeed,YSpeed,ZSpeed);
    renderThread->addCommand(command);
    ui->YRotDisp->display(position);
  }
  return;
}

void MainWindow::on_ZRotSpeed_sliderMoved(int position) {
  ZSpeed = position;
  if (renderThread != nullptr) {
    auto command = std::make_shared<UpdateRotationSpeedCommand>(XSpeed,YSpeed,ZSpeed);
    renderThread->addCommand(command);
    ui->ZRotDisp->display(position);
  }
  return;
}

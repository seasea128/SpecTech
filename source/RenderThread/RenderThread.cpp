#include "RenderThread.h"
#include "Callback.h"
#include <vtkActorCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>

#include <type_traits>

RenderThread::RenderThread(
    QObject *parent, vtkSmartPointer<vtkRenderer> renderer,
    vtkSmartPointer<vtkRenderWindow> window,
    vtkSmartPointer<vtkRenderWindowInteractor> interactor,
    vtkSmartPointer<vtkCamera> camera)
    : endRender(false) {
  /* Initialise actor list */
  actors = vtkActorCollection::New();

  /* Initialise command variables */
  rotateX = 0.;
  rotateY = 0.;
  rotateZ = 0.;

  callback = RenderThreadCallback::New(this);

  this->renderer = renderer;
  this->window = window;
  this->interactor = interactor;
  this->camera = camera;
}

RenderThread::~RenderThread() {
  callback->Delete();
  actors->InitTraversal();
  vtkActor *a;
  while ((a = (vtkActor *)actors->GetNextActor())) {
    a->Delete();
  }
  actors->Delete();
}

void RenderThread::run() {
  /* You might want to edit the 3D model once VR has started, however VTK is not
   * "thread safe". This means if you try to edit the VR model from the GUI
   * thread while the VR thread is running, the program could become corrupted
   * and crash. The solution is to get the VR thread to edit the model. Any
   * decision to change the VR model will come fromthe user via the GUI thread,
   * so there needs to be a mechanism to pass data from the GUI thread to the VR
   * thread.
   */

  vtkNew<vtkNamedColors> colors;

  // Set the background color.
  std::array<unsigned char, 4> bkg{{26, 51, 102, 255}};
  colors->SetColor("BkgColor", bkg.data());

  renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

  /* Loop through list of actors provided and add to scene */
  // vtkSmartPointer<vtkCompositePolyDataMapper> bigMapper =
  // vtkCompositePolyDataMapper::New();
  vtkActor *a;
  actors->InitTraversal();
  while ((a = (vtkActor *)actors->GetNextActor())) {
    renderer->AddActor(a);
  }

  // window->Initialize();
  window->AddRenderer(renderer);

  renderer->SetActiveCamera(camera);

  interactor->SetRenderWindow(window);
  interactor->Initialize();

  interactor->AddObserver(vtkCommand::TimerEvent, callback);
  int timerId = interactor->CreateRepeatingTimer(20);

  interactor->Start();
  window->Render();
}

void RenderThread::addActorOffline(vtkActor *actor) {
  if (!this->isRunning()) {
    double *ac = actor->GetOrigin();

    /* I have found that these initial transforms will position the FS
     * car model in a sensible position but you can experiment
     */
    actor->RotateX(-90);
    actor->AddPosition(-ac[0] + 0, -ac[1] - 100, -ac[2] - 200);

    actors->AddItem(actor);
  }
}

void RenderThread::issueCommand(int cmd, double value) {
  /* Update class variables according to command */
  switch (cmd) {
  /* These are just a few basic examples */
  case END_RENDER:
    this->endRender = true;
    break;

  case ROTATE_X:
    this->rotateX = value;
    break;

  case ROTATE_Y:
    this->rotateY = value;
    break;

  case ROTATE_Z:
    this->rotateZ = value;
    break;

  case RE_RENDER:
    // Cannot call Render() on renderWindow directly here, since it will be
    // executed on main thread. It need to be called from renderThread itself.
    this->reRender = true;
    break;
  }
}

void RenderThread::stopRender() const {
  interactor->GetRenderWindow()->Finalize();
  interactor->TerminateApp();
}

void RenderThread::refreshRender() const { this->window->Render(); }

void RenderThread::updateColour(vtkActor *actorToUpdate,
                                vtkColor3<unsigned char> &updateColour) {
  actorToUpdate->GetProperty()->SetColor(double(updateColour.GetRed()) / 255,
                                         double(updateColour.GetGreen()) / 255,
                                         double(updateColour.GetBlue()) / 255);
}

void RenderThread::updateVisibility(vtkActor *actorToUpdate, bool visible) {
  if (actorToUpdate != nullptr) {
    actorToUpdate->GetProperty()->SetOpacity(visible ? 1.0 : 0.0);
  }
}

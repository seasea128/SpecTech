#include "RenderThread.h"
#include "Callback.h"
#include <qdebug.h>
#include <vtkActorCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>

#include <vtkWeakPointer.h>

using namespace Commands;

RenderThread::RenderThread(
    QObject *parent, vtkSmartPointer<vtkRenderer> renderer,
    vtkSmartPointer<vtkRenderWindow> window,
    vtkSmartPointer<vtkRenderWindowInteractor> interactor,
    vtkSmartPointer<vtkCamera> camera)
    : rotateX(0.), rotateY(0.), rotateZ(0.) {
  /* Initialise actor list */
  actors = vtkActorCollection::New();

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

  vtkNew<vtkFrustumCoverageCuller> culler;
  renderer->AddCuller(culler);
  renderer->SetActiveCamera(camera);

  interactor->SetRenderWindow(window);
  interactor->Initialize();
  interactor->AddObserver(vtkCommand::RenderEvent, callback);

  interactor->SetDesiredUpdateRate(60);
  interactor->SetStillUpdateRate(60);

  // interactor->AddObserver(vtkCommand::TimerEvent, callback);
  // int timerId = interactor->CreateRepeatingTimer(20);

  if (dynamic_cast<vtkOpenVRRenderWindowInteractor *>(
          interactor.GetPointer()) == nullptr) {
    interactor->SetDone(false);
    while (!interactor->GetDone()) {
      interactor->ProcessEvents();
      interactor->Render();
    }
  } else {
    interactor->Start();
  }
}

void RenderThread::addCommand(const std::shared_ptr<BaseCommand> &command) {
  // Lock the mutex to prevent the case where there is only one command and it
  // is being added and removed from the queue at the same time.
  this->mutex.lock();
  this->queue.enqueue(command); // The ownership of command should be passed
                                // through to the queue now, so command should
                                // still exist after this scope end.
  this->mutex.unlock();
}

void RenderThread::addActorOffline(vtkSmartPointer<vtkActor> actor) {
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

void RenderThread::stopRender() const {
  interactor->GetRenderWindow()->Finalize();
  interactor->TerminateApp();
}

void RenderThread::refreshRender() const { this->window->Render(); }

void RenderThread::updateColour(vtkWeakPointer<vtkActor> actorToUpdate,
                                vtkColor3<unsigned char> &updateColour) {
  if (actorToUpdate) {
    actorToUpdate->GetProperty()->SetColor(
        double(updateColour.GetRed()) / 255,
        double(updateColour.GetGreen()) / 255,
        double(updateColour.GetBlue()) / 255);
  }
}

void RenderThread::updateVisibility(vtkWeakPointer<vtkActor> actorToUpdate,
                                    bool visible) {
  if (actorToUpdate != nullptr) {
    actorToUpdate->GetProperty()->SetOpacity(visible ? 1.0 : 0.0);
  }
}

void RenderThread::removeActor(vtkWeakPointer<vtkActor> actorToRemove) {
  if (actorToRemove) {
    actors->RemoveItem(actorToRemove);

    renderer->RemoveAllViewProps();
    vtkActor *a;
    actors->InitTraversal();
    while ((a = (vtkActor *)actors->GetNextActor())) {
      renderer->AddActor(a);
    }
  }
}

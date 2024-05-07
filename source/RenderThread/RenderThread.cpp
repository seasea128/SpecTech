#include "RenderThread.h"
#include "Callback.h"
#include <qdebug.h>
#include <vtkActorCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkHDRReader.h>
#include <vtkNamedColors.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkTexture.h>

#include <vtkWeakPointer.h>

using namespace Commands;

RenderThread::RenderThread(
    QObject *parent, vtkSmartPointer<vtkRenderer> renderer,
    vtkSmartPointer<vtkRenderWindow> window,
    vtkSmartPointer<vtkRenderWindowInteractor> interactor,
    vtkSmartPointer<vtkCamera> camera, std::string_view hdr_fileName)
    : rotateX(0.), rotateY(0.), rotateZ(0.) {
  /* Initialise actor list */
  actors = vtkActorCollection::New();

  // Create callback and assign everything to member variable
  callback = RenderThreadCallback::New(this);

  this->renderer = renderer;
  this->window = window;
  this->interactor = interactor;
  this->camera = camera;
  this->hdr_fileName = hdr_fileName;
}

RenderThread::~RenderThread() {
  // Delete the callback since it's a normal pointer
  callback->Delete();
  this->renderer->Delete();
  this->window->Delete();
  this->interactor->Delete();
  this->camera->Delete();
  this->reader->Delete();
}

void RenderThread::run() {

  // Load up another copy of the skybox, can't use same one as UI because it'll
  // complains with warning
  reader = vtkHDRReader::New();
  reader->SetFileName(std::string(hdr_fileName).c_str());
  reader->Update();

  vtkNew<vtkTexture> envTexture;
  envTexture->SetInputConnection(reader->GetOutputPort());
  envTexture->SetColorModeToDirectScalars();
  envTexture->MipmapOn();
  envTexture->InterpolateOn();

  skybox = vtkSmartPointer<vtkSkybox>::New();
  skybox->SetFloorRight(0, 0, 1);
  skybox->SetProjection(vtkSkybox::Sphere);
  skybox->SetTexture(envTexture);
  skybox->GammaCorrectOn();

  // Set skybox and enable PBR and image based lightning
  renderer->UseImageBasedLightingOn();

  // Need to cast it to vtkOpenGLRenderer because UseSphericalHarmonicsOn only
  // exist on vtkOpenGLRenderer
  vtkSmartPointer<vtkOpenGLRenderer> localRenderer =
      vtkOpenGLRenderer::SafeDownCast(renderer);
  localRenderer->UseSphericalHarmonicsOn();
  renderer->SetEnvironmentTexture(skybox->GetTexture(), false);
  renderer->AddActor(skybox);

  /* Loop through list of actors provided and add to scene */
  vtkActor *a;
  actors->InitTraversal();
  while ((a = (vtkActor *)actors->GetNextActor())) {
    renderer->AddActor(a);
  }

  window->AddRenderer(renderer);

  vtkNew<vtkFrustumCoverageCuller> culler;
  renderer->AddCuller(culler);
  renderer->SetActiveCamera(camera);

  interactor->SetRenderWindow(window);
  interactor->Initialize();
  interactor->AddObserver(vtkCommand::RenderEvent,
                          callback); // The callback will be executed every time
                                     // renderer renders something.

  // Set the target update rate, should be 90 for 90fps with Vive Pro
  interactor->SetDesiredUpdateRate(90);
  interactor->SetStillUpdateRate(90);

  // This check needs to be done to simulate the VR, which would render
  // continuosly without stopping.
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
  // Stopping the RenderThread
  interactor->GetRenderWindow()->Finalize();
  interactor->TerminateApp();
}

void RenderThread::refreshRender() const {
  // Remove all actor and readd from list
  renderer->RemoveAllViewProps();
  vtkActor *a;
  actors->InitTraversal();
  while ((a = (vtkActor *)actors->GetNextActor())) {
    renderer->AddActor(a);
  }
  renderer->AddActor(skybox);
}

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
  }
}

void RenderThread::addActor(vtkSmartPointer<vtkActor> actorToAdd) {
  if (actorToAdd) {
    double *ac = actorToAdd->GetOrigin();

    /* I have found that these initial transforms will position the FS
     * car model in a sensible position but you can experiment
     */
    actorToAdd->RotateX(-90);
    actorToAdd->AddPosition(-ac[0] + 0, -ac[1] - 100, -ac[2] - 200);
    actors->AddItem(actorToAdd);
    renderer->AddActor(actorToAdd);
  }
}

void RenderThread::updateSpeedX(unsigned int Xval) { rotateX = Xval; }
void RenderThread::updateSpeedY(unsigned int Yval) { rotateY = Yval; }
void RenderThread::updateSpeedZ(unsigned int Zval) { rotateZ = Zval; }

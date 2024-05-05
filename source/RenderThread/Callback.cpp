#include "Callback.h"

#include <memory>

void RenderThreadCallback::Execute(vtkObject *caller, unsigned long eventId,
                                   void *vtkNotUsed(callData)) {
  if (vtkCommand::RenderEvent == eventId) {
    renderThread->mutex.lock();

    auto beforeCommand = std::chrono::system_clock::now();
    while (!renderThread->queue.isEmpty()) {
      auto currentTime = std::chrono::system_clock::now();
      // Execute as much command as the CPU can until 2ms passed
      if (currentTime - beforeCommand > std::chrono::milliseconds(2)) {
        break;
      }
      std::shared_ptr<Commands::BaseCommand> command =
          renderThread->queue.dequeue();
      command->Execute(*renderThread);
    }
    renderThread->mutex.unlock();

    auto iren = dynamic_cast<vtkRenderWindowInteractor *>(caller);

    vtkActorCollection *actorList = iren->GetRenderWindow()
                                        ->GetRenderers()
                                        ->GetFirstRenderer()
                                        ->GetActors();
    vtkActor *a;

    /* X Rotation */
    actorList->InitTraversal();
    while ((a = (vtkActor *)actorList->GetNextActor())) {
      a->RotateX(renderThread->rotateX);
    }

    /* Y Rotation */
    actorList->InitTraversal();
    while ((a = (vtkActor *)actorList->GetNextActor())) {
      a->RotateY(renderThread->rotateY);
    }

    /* Z Rotation */
    actorList->InitTraversal();
    while ((a = (vtkActor *)actorList->GetNextActor())) {
      a->RotateZ(renderThread->rotateZ);
    }
  }
}

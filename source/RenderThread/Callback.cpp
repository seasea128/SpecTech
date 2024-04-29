#include "Callback.h"

#include <memory>

void RenderThreadCallback::Execute(vtkObject *caller, unsigned long eventId,
                                   void *vtkNotUsed(callData)) {
  if (vtkCommand::RenderEvent == eventId) {
    renderThread->mutex.lock();
    auto lastExecuted = std::chrono::system_clock::now();
    while (!renderThread->queue.isEmpty()) {
      auto currentTime = std::chrono::system_clock::now();
      // Execute as much command as the CPU can until 15ms passed (Should be
      // lower than this)
      if (currentTime - lastExecuted > std::chrono::milliseconds(15)) {
        break;
      }
      std::shared_ptr<Commands::BaseCommand> command =
          renderThread->queue.dequeue();
      command->Execute(*renderThread);
    }
    renderThread->mutex.unlock();
    renderThread->refreshRender();

    auto iren = dynamic_cast<vtkRenderWindowInteractor *>(caller);

    // This is where the logic for updating actors and thing should be
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

    // Rerender the window whenever it's updated
    // if (renderThread->reRender) {
    //  renderThread->window->Render();
    //  renderThread->reRender = false;
    //}

    // if (renderThread->endRender) {
    //   iren->GetRenderWindow()->Finalize();
    //   iren->TerminateApp();
    // }
  }
}

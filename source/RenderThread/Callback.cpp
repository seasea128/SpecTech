#include "Callback.h"
#include "Commands/UpdateColourCommand.h"

#include <memory>

void RenderThreadCallback::Execute(vtkObject *caller, unsigned long eventId,
                                   void *vtkNotUsed(callData)) {
  if (vtkCommand::TimerEvent == eventId) {

    renderThread->mutex.lock();
    // if (!renderThread->queue.isEmpty()) {
    //   std::shared_ptr<BaseCommand> command = renderThread->queue.dequeue();
    //   command->Execute(*(this->renderThread));
    // }

    int counter = 0;
    while (!renderThread->queue.isEmpty()) {
      if (counter >= 10) {
        break;
      }
      std::shared_ptr<BaseCommand> command = renderThread->queue.dequeue();
      if (!renderThread->queue.isEmpty()) {
        std::shared_ptr<BaseCommand> nextCommand = renderThread->queue.head();
        if (command == nextCommand) {
          counter++;
          continue;
        }
      }
      command->Execute(*renderThread);
      counter++;
    }
    renderThread->mutex.unlock();

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
    if (renderThread->reRender) {
      renderThread->window->Render();
      renderThread->reRender = false;
    }

    if (renderThread->endRender) {
      iren->GetRenderWindow()->Finalize();
      iren->TerminateApp();
    }
  }
}

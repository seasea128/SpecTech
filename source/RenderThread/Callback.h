#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "RenderThread.h"
#include <vtkActorCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>

class vtkTimerCallback : public vtkCallbackCommand {
public:
  vtkTimerCallback() = default;

  static vtkTimerCallback *New(RenderThread *thread) {
    vtkTimerCallback *cb = new vtkTimerCallback;
    cb->renderThread = thread;
    return cb;
  }

  virtual void Execute(vtkObject *caller, unsigned long eventId,
                       void *vtkNotUsed(callData)) {
    if (vtkCommand::TimerEvent == eventId) {
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
    }
  }

private:
  RenderThread *renderThread;
};
#endif // CALLBACK_H_

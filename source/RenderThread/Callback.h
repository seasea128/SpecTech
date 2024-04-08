#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "RenderThread.h"
#include <vtkActorCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>

class RenderThreadCallback : public vtkCallbackCommand {
public:
  RenderThreadCallback() = default;

  static RenderThreadCallback *New(RenderThread *thread) {
    RenderThreadCallback *cb = new RenderThreadCallback;
    cb->renderThread = thread;
    return cb;
  }

  virtual void Execute(vtkObject *caller, unsigned long eventId,
                       void *vtkNotUsed(callData));

private:
  RenderThread *renderThread;
};
#endif // CALLBACK_H_

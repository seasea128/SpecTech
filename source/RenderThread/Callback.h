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

  /**
   * Static function that is used by vtkSmartPointer to create new instance of
   * RenderThreadCallback.
   */
  static RenderThreadCallback *New(RenderThread *thread) {
    RenderThreadCallback *createdCallback = new RenderThreadCallback;
    createdCallback->renderThread = thread;
    // cb->lastExecuted = std::chrono::system_clock::now();
    return createdCallback;
  }

  /**
   * RenderThread will execute this function everytime it is rendering a frame.
   *
   * Currently, it process commands stored inside RenderThread::queue.
   */
  virtual void Execute(vtkObject *caller, unsigned long eventId,
                       void *vtkNotUsed(callData));

private:
  /**
   * Pointer to RenderThread. Needed to access current instance of RenderThread
   */
  RenderThread *renderThread;
  // std::chrono::system_clock::time_point lastExecuted;
};
#endif // CALLBACK_H_

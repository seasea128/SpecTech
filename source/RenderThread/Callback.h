#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "RenderThread.h"
#include <vtkActorCollection.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>

/**
 * Callback for RenderThread to process commands and doing various other things.
 *
 * Code that should be run on render loop should be here.
 */
class RenderThreadCallback : public vtkCallbackCommand {
public:
  RenderThreadCallback() = default;

  /**
   * Static function that is used by vtkSmartPointer to create new instance of
   * RenderThreadCallback.
   * @param thread is the parent RenderThread.
   * @return new copy of RenderThreadCallback.
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
   *
   * @param caller is the object that is calling this function.
   * @param eventId is the id of the event that call this function.
   * @param vtkNotUsed is not used here.
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

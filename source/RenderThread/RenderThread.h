#ifndef RENDERTHREAD_H_
#define RENDERTHREAD_H_

#include <QQueue>
#include <QThread>
#include <qmutex.h>
#include <qwaitcondition.h>

#include <vtkActorCollection.h>
#include <vtkCamera.h>
#include <vtkLODActor.h>
#include <vtkMapperCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

#include <memory>

#include "Commands/BaseCommand.h"

// RenderThreadCallback need to be forward declared since it has circular
// dependency with RenderThread.
class RenderThreadCallback;

class RenderThread : public QThread {
  Q_OBJECT

public:
  /** List of command names */
  enum { END_RENDER, ROTATE_X, ROTATE_Y, ROTATE_Z, RE_RENDER } Command;

  RenderThread(const RenderThread &) = default;
  RenderThread(RenderThread &&) = delete;
  RenderThread &operator=(const RenderThread &) = default;
  RenderThread &operator=(RenderThread &&) = delete;
  /**  Constructor
   */
  RenderThread(QObject *parent, vtkSmartPointer<vtkRenderer> renderer,
               vtkSmartPointer<vtkRenderWindow> window,
               vtkSmartPointer<vtkRenderWindowInteractor> interactor,
               vtkSmartPointer<vtkCamera> camera);

  /**  Destructor
   */
  virtual ~RenderThread();

  /** This allows actors to be added to the VR renderer BEFORE the VR
   * interactor has been started
   */
  virtual void addActorOffline(vtkActor *actor);

  /** This allows commands to be issued to the VR thread in a thread safe way.
   * Function will set variables within the class to indicate the type of
   * action / animation / etc to perform. The rendering thread will then
   * impelement this.
   */
  virtual void issueCommand(int cmd, double value);

  void addCommand(const std::shared_ptr<BaseCommand> &command) {
    mutex.lock();
    queue.enqueue(command);
    mutex.unlock();
  }

  void stopRender() const;

  void refreshRender() const;

protected:
  void run() override;

  // Custom callback for renderThread. Any modification of data inside this
  // class should probably go through this callback since
  // vtkRenderWindowInteractor doesn't have DoOneEvent method, so handling the
  // command in main loop would be pretty much impossible. - Chanon Yothavut
  vtkSmartPointer<RenderThreadCallback> callback;

  // VTK rendering related things - I think this should be reassignable to
  // vtkOpenVR alternative. - Chanon Yothavut
  vtkSmartPointer<vtkRenderWindow> window;
  vtkSmartPointer<vtkRenderWindowInteractor> interactor;
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkCamera> camera;

  /* Use to synchronise passing of data to VR thread */
  QMutex mutex;
  QWaitCondition condition;

  QQueue<std::shared_ptr<BaseCommand>> queue;

  /** List of actors that will need to be added to the VR scene */
  vtkSmartPointer<vtkActorCollection> actors;

  // TODO: Add a command FIFO queue to handle communication from main thread,
  // then process it in callback.
  // TODO: Might need another queue in main thread to notify if command is done.

  /** This will be set to false by the constructor, if it is set to true
   * by the GUI then the rendering will end
   */
  bool endRender; // Instead of setting this to true, might as well just stop
                  // the thread by calling renderThread->exit()
                  // Edit: This assumption doesn't work.

  // Need this to signal when the thread's renderWindow need to rerender. -
  // Chanon Yothavut
  bool reRender;

  /* Some variables to indicate animation actions to apply.
   *
   */
  double rotateX; /*< Degrees to rotate around X axis (per time-step) */
  double rotateY; /*< Degrees to rotate around Y axis (per time-step) */
  double rotateZ; /*< Degrees to rotate around Z axis (per time-step) */

  // RenderThreadCallback being a friend of RenderThread will make the code much
  // more simpler, since there isn't a need for getter/setter for protected
  // member variable.
  friend class RenderThreadCallback;
};

#endif // RENDERTHREAD_H_

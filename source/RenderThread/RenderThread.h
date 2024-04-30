#ifndef RENDERTHREAD_H_
#define RENDERTHREAD_H_

#include <QQueue>
#include <QThread>
#include <qmutex.h>
#include <qwaitcondition.h>

#include <vtkActorCollection.h>
#include <vtkCamera.h>
#include <vtkColor.h>
#include <vtkHDRReader.h>
#include <vtkLODActor.h>
#include <vtkMapperCollection.h>
#include <vtkOpenGLRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSkybox.h>
#include <vtkSmartPointer.h>

#include <memory>

#include "Commands/BaseCommand.h"

// RenderThreadCallback need to be forward declared since it has circular
// dependency with RenderThread.
class RenderThreadCallback;

/**
 * Implementation of renderer agnostic thread where both normal rendering
 * component or OpenVR rendering component can be used.
 */
class RenderThread : public QThread {
  Q_OBJECT

public:
  RenderThread(const RenderThread &) = delete;
  RenderThread(RenderThread &&) = delete;
  RenderThread &operator=(const RenderThread &) = delete;
  RenderThread &operator=(RenderThread &&) = delete;
  /**  Constructor
   */
  RenderThread(QObject *parent, vtkSmartPointer<vtkRenderer> renderer,
               vtkSmartPointer<vtkRenderWindow> window,
               vtkSmartPointer<vtkRenderWindowInteractor> interactor,
               vtkSmartPointer<vtkCamera> camera,
               vtkSmartPointer<vtkHDRReader> reader);

  /**  Destructor
   */
  virtual ~RenderThread();

  /** This allows actors to be added to the VR renderer BEFORE the VR
   * interactor has been started
   */
  virtual void addActorOffline(vtkSmartPointer<vtkActor> actor);

  /**
   * Add command to the queue inside this RenderThread. Should be thread-safe.
   * @param command is the command that is being passed through to RenderThread.
   */
  void addCommand(const std::shared_ptr<Commands::BaseCommand> &command);

  /**
   * Stop renderWindow when it is called.
   */
  void stopRender() const;

  /**
   * Re-render when called. Needed for testing since VTK doesn't continuosly
   * render on normal OpenGL renderwindow with no interaction.
   */
  void refreshRender() const;

  /**
   * Update colour of the given actor with provided vtkColor.
   */
  void updateColour(vtkWeakPointer<vtkActor> actorToUpdate,
                    vtkColor3<unsigned char> &updateColour);

  /**
   * Update visibility of the given actor.
   */
  void updateVisibility(vtkWeakPointer<vtkActor> actorToUpdate, bool visible);

  /**
   * Remove actor from collection.
   */
  void removeActor(vtkWeakPointer<vtkActor> actorToRemove);

  /**
   * Add actor to collection.
   */
  void addActor(vtkSmartPointer<vtkActor> actorToAdd);

protected:
  void run() override;

  /** Custom callback for renderThread. Any modification of data inside
   * RenderThread class should probably go through this callback since
   * vtkRenderWindowInteractor doesn't have DoOneEvent method, so handling the
   * command in main loop would be pretty much impossible.
   */
  vtkSmartPointer<RenderThreadCallback> callback;

  // VTK rendering related things
  vtkSmartPointer<vtkRenderWindow> window;
  vtkSmartPointer<vtkRenderWindowInteractor> interactor;
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkCamera> camera;
  vtkSmartPointer<vtkSkybox> skybox;
  vtkSmartPointer<vtkHDRReader> reader;

  /** Use to synchronise passing of data to VR thread */
  QMutex mutex;
  QWaitCondition condition;

  /**
   * Queue of commands waiting to be executed by RenderThreadCallback.
   */
  QQueue<std::shared_ptr<Commands::BaseCommand>> queue;

  /** List of actors that will need to be added to the VR scene */
  vtkSmartPointer<vtkActorCollection> actors;

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

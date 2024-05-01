#ifndef UPDATEOPACITYCOMMAND_H_
#define UPDATEOPACITYCOMMAND_H_

#include "../../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkActor.h>

namespace Commands {

/**
 * Command that will update visibility of given actor on RenderThread.
 */
class UpdateVisibilityCommand : public BaseCommand {

public:
  /**
   * Constructor that create an instance with given ModelPart.
   * @param part is the ModelPart that need to update it's visibility.
   */
  UpdateVisibilityCommand(const ModelPart *part);

  /**
   * Copy constructor of UpdateVisibilityCommand.
   * @param command is the command that is being copied.
   */
  UpdateVisibilityCommand(const UpdateVisibilityCommand &command);

  /**
   * Assignment operator of UpdateVisibilityCommand.
   * @param command is the command that is being copied.
   * @return assigned UpdateVisibilityCommand.
   */
  UpdateVisibilityCommand &operator=(const UpdateVisibilityCommand &command);

  /**
   * Destructor of UpdateVisibilityCommand.
   */
  virtual ~UpdateVisibilityCommand();

  /**
   * Function that will be executed in the RenderThread.
   * @param renderThread is used to access RenderThread from AddActorCommand.
   */
  virtual void Execute(RenderThread &renderThread) override;

private:
  /** Visibility of the given actor */
  bool visible;

  /**
   * Actor that need to update visibility.
   */
  vtkWeakPointer<vtkActor> actorToUpdate;
};
} // namespace Commands
#endif // UPDATEOPACITYCOMMAND_H_

#ifndef UPDATEPROPERTYCOMMAND_H_
#define UPDATEPROPERTYCOMMAND_H_

#include "../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkActor.h>
#include <vtkColor.h>

namespace Commands {
/**
 * Command that will update colour of a given actor in RenderThread.
 */
class UpdatePropertyCommand : public BaseCommand {

public:
  /**
   * Constructor of UpdatePropertyCommand with part that need to update it's
   * colour.
   * @param part is the part that need to update it's colour.
   */
  UpdatePropertyCommand(ModelPart *part);

  /**
   * Copy constructor of UpdatePropertyCommand
   * @param command is the command to be copied.
   */
  UpdatePropertyCommand(const UpdatePropertyCommand &command);

  /**
   * Assignment operator override for RemoveActorCommand.
   * @param command is the command to be copied.
   * @return assigned UpdatePropertyCommand.
   */
  UpdatePropertyCommand &operator=(const UpdatePropertyCommand &command);
  virtual ~UpdatePropertyCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  vtkSmartPointer<vtkProperty> newProperty; /**< New colour of the actor.*/
  vtkWeakPointer<vtkActor> actorToUpdate;   /**< Actor that is being updated.*/
};
} // namespace Commands

#endif // UPDATEPROPERTYCOMMAND_H_

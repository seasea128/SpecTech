#ifndef UPDATECOLOURCOMMAND_H_
#define UPDATECOLOURCOMMAND_H_
#include "../../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkActor.h>
#include <vtkColor.h>

namespace Commands {
/**
 * Command that will update colour of a given actor in RenderThread.
 */
class UpdateColourCommand : public BaseCommand {

public:
  /**
   * Constructor of UpdateColourCommand with part that need to update it's
   * colour.
   * @param part is the part that need to update it's colour.
   */
  UpdateColourCommand(const ModelPart *part);

  /**
   * Copy constructor of UpdateColourCommand
   * @param command is the command to be copied.
   */
  UpdateColourCommand(const UpdateColourCommand &command);

  /**
   * Assignment operator override for RemoveActorCommand.
   * @param command is the command to be copied.
   * @return assigned UpdateColourCommand.
   */
  UpdateColourCommand &operator=(const UpdateColourCommand &command);
  virtual ~UpdateColourCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  vtkColor3<unsigned char> updateColour;  /**< New colour of the actor.*/
  vtkWeakPointer<vtkActor> actorToUpdate; /**< Actor that is being updated.*/
};
} // namespace Commands
#endif // UPDATECOLOURCOMMAND_H_

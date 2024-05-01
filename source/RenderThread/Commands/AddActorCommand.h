#ifndef ADDACTORCOMMAND_H_
#define ADDACTORCOMMAND_H_
#include "../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkSmartPointer.h>
namespace Commands {

/**
 * Commands used to add actor to RenderThread.
 */

class AddActorCommand : public BaseCommand {

public:
  /**
   * Constructor for creating AddActorCommand with ModelPart.
   * @param part is the part needed to add to RenderThread.
   */
  AddActorCommand(const ModelPart *part);

  /**
   * Copy constructor for this class.
   * @param command is the command to copy.
   */
  AddActorCommand(const AddActorCommand &command);

  /**
   * Constructor for creating AddActorCommand with vtkActor.
   * @param actor is the actor that need to be added.
   */
  AddActorCommand(const vtkSmartPointer<vtkActor> actor);

  /**
   * Assignment operator override for AddActorCommand.
   * @param command is the command to copy over to the new AddActorCommand.
   * @return assigned AddActorCommand.
   */
  AddActorCommand &operator=(const AddActorCommand &command);

  /**
   * Destructor for AddActorCommand.
   */
  virtual ~AddActorCommand();

  /**
   * Function that will be executed in the RenderThread.
   * @param renderThread is used to access RenderThread from AddActorCommand.
   */
  virtual void Execute(RenderThread &renderThread) override;

private:
  /**
   * Smart pointer to actor that is being added to RenderThread.
   */
  vtkSmartPointer<vtkActor> actorToAdd;
};
} // namespace Commands
#endif // ADDACTORCOMMAND_H_

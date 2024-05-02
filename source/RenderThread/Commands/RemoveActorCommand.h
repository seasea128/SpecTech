#ifndef REMOVEACTORCOMMAND_H_
#define REMOVEACTORCOMMAND_H_

#include "../../ModelPart.h"
#include "BaseCommand.h"
namespace Commands {

/**
 * Command that will remove actor from RenderThread.
 */
class RemoveActorCommand : public BaseCommand {

public:
  /**
   * Constructor for RemoveActorCommand with part
   * @param part is the part that actor need to be removed from the
   * RenderThread.
   */
  RemoveActorCommand(const ModelPart *part);

  /**
   * Copy constructor for RemoveActorCommand.
   * @param command is the command to be copied.
   */
  RemoveActorCommand(const RemoveActorCommand &command);

  /**
   * Assignment operator override for RemoveActorCommand.
   * @param command is the command that is being copied.
   * @return assigned RemoveActorCommand
   */
  RemoveActorCommand &operator=(const RemoveActorCommand &command);

  /**
   * Destructor for RemoveActorCommand.
   */
  virtual ~RemoveActorCommand();
  /**
   * Function that will be executed in the RenderThread.
   * @param renderThread is used to access RenderThread from AddActorCommand.
   */
  virtual void Execute(RenderThread &renderThread) override;

private:
  /**
   * Weak pointer to actor that need to be removed.
   */
  vtkWeakPointer<vtkActor> actorToRemove;
};
} // namespace Commands
#endif // REMOVEACTORCOMMAND_H_

#ifndef REMOVEACTORCOMMAND_H_
#define REMOVEACTORCOMMAND_H_

#include "../../ModelPart.h"
#include "BaseCommand.h"
namespace Commands {

class RemoveActorCommand : public BaseCommand {

public:
  RemoveActorCommand(const ModelPart *part);
  RemoveActorCommand(const RemoveActorCommand &command);
  RemoveActorCommand &operator=(const RemoveActorCommand &command);
  virtual ~RemoveActorCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  vtkWeakPointer<vtkActor> actorToRemove;
};
} // namespace Commands
#endif // REMOVEACTORCOMMAND_H_

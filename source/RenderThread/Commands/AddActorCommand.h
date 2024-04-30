#ifndef ADDACTORCOMMAND_H_
#define ADDACTORCOMMAND_H_
#include "../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkSmartPointer.h>
namespace Commands {

class AddActorCommand : public BaseCommand {

public:
  AddActorCommand(const ModelPart *part);
  AddActorCommand(const AddActorCommand &command);
  AddActorCommand(const vtkSmartPointer<vtkActor> actor);
  AddActorCommand &operator=(const AddActorCommand &command);
  virtual ~AddActorCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  vtkSmartPointer<vtkActor> actorToAdd;
};
} // namespace Commands
#endif // ADDACTORCOMMAND_H_

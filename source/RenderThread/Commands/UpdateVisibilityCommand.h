#ifndef UPDATEOPACITYCOMMAND_H_
#define UPDATEOPACITYCOMMAND_H_

#include "../../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkActor.h>

namespace Commands {

class UpdateVisibilityCommand : public BaseCommand {

public:
  UpdateVisibilityCommand(const ModelPart *part);
  UpdateVisibilityCommand(const UpdateVisibilityCommand &command);
  UpdateVisibilityCommand &operator=(const UpdateVisibilityCommand &command);
  virtual ~UpdateVisibilityCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  bool visible;
  vtkWeakPointer<vtkActor> actorToUpdate;
};
} // namespace Commands
#endif // UPDATEOPACITYCOMMAND_H_

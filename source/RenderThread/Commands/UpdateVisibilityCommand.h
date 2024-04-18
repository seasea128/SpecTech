#ifndef UPDATEOPACITYCOMMAND_H_
#define UPDATEOPACITYCOMMAND_H_

#include "../../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkActor.h>

class UpdateVisibilityCommand : public BaseCommand {

public:
  // UpdateVisibilityCommand(vtkActor *_actorToUpdate, bool _visible);
  UpdateVisibilityCommand(const ModelPart *part);
  UpdateVisibilityCommand(const UpdateVisibilityCommand &command);
  UpdateVisibilityCommand &operator=(const UpdateVisibilityCommand &command);
  virtual ~UpdateVisibilityCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  bool visible;
  vtkActor *actorToUpdate;
};

#endif // UPDATEOPACITYCOMMAND_H_

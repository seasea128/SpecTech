#ifndef UPDATECOLOURCOMMAND_H_
#define UPDATECOLOURCOMMAND_H_
#include "../../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkActor.h>
#include <vtkColor.h>

class UpdateColourCommand : public BaseCommand {

public:
  // UpdateColourCommand(vtkActor *_actorToUpdate,
  //                     vtkColor3<unsigned char> _updateColour);
  UpdateColourCommand(const ModelPart *part);
  UpdateColourCommand(const UpdateColourCommand &command);
  UpdateColourCommand &operator=(const UpdateColourCommand &command);
  virtual ~UpdateColourCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  vtkColor3<unsigned char> updateColour;
  vtkActor *actorToUpdate;
};

#endif // UPDATECOLOURCOMMAND_H_

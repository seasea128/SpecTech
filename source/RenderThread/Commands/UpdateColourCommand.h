#ifndef UPDATECOLOURCOMMAND_H_
#define UPDATECOLOURCOMMAND_H_
#include "BaseCommand.h"
#include <vtkActor.h>
#include <vtkColor.h>

class UpdateColourCommand : public BaseCommand {
  using BaseCommand::operator=;

public:
  UpdateColourCommand(vtkActor *_actorToUpdate,
                      vtkColor3<unsigned char> _updateColour);
  virtual ~UpdateColourCommand();
  virtual void Execute(RenderThread &renderThread) override;

private:
  vtkColor3<unsigned char> updateColour;
  vtkActor *actorToUpdate;
};

#endif // UPDATECOLOURCOMMAND_H_

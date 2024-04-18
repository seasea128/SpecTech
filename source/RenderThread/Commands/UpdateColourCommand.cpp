#include "UpdateColourCommand.h"

#include "../RenderThread.h"

// UpdateColourCommand::UpdateColourCommand(vtkActor *_actorToUpdate,
//                                          vtkColor3<unsigned char>
//                                          _updateColour)
//     : actorToUpdate(_actorToUpdate), updateColour(_updateColour) {}

UpdateColourCommand::UpdateColourCommand(const ModelPart *part)
    : actorToUpdate(part->getVRActor()), updateColour(part->getColour()) {}

UpdateColourCommand::~UpdateColourCommand() {}

UpdateColourCommand::UpdateColourCommand(const UpdateColourCommand &command)
    : actorToUpdate(command.actorToUpdate), updateColour(command.updateColour) {
}

void UpdateColourCommand::Execute(RenderThread &renderThread) {
  renderThread.updateColour(actorToUpdate, updateColour);
}

UpdateColourCommand &
UpdateColourCommand::operator=(const UpdateColourCommand &command) {
  if (this == &command) {
    return *this;
  }
  this->actorToUpdate = command.actorToUpdate;
  this->updateColour = command.updateColour;
  return *this;
}

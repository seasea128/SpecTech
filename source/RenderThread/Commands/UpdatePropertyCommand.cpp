#include "UpdatePropertyCommand.h"
#include <vtkProperty.h>

using Commands::UpdatePropertyCommand;

UpdatePropertyCommand::UpdatePropertyCommand(ModelPart *part)
    : actorToUpdate(part->getVRActor()),
      newProperty(part->getActor()->GetProperty()) {}

UpdatePropertyCommand::~UpdatePropertyCommand() {}

UpdatePropertyCommand::UpdatePropertyCommand(
    const UpdatePropertyCommand &command)
    : actorToUpdate(command.actorToUpdate), newProperty(command.newProperty) {}

void UpdatePropertyCommand::Execute(RenderThread &renderThread) {
  if (actorToUpdate) {
    // Copy the property over to the VR actor
    actorToUpdate->GetProperty()->DeepCopy(newProperty);
  }
}

UpdatePropertyCommand &
UpdatePropertyCommand::operator=(const UpdatePropertyCommand &command) {
  if (this == &command) {
    return *this;
  }
  this->actorToUpdate = command.actorToUpdate;
  this->newProperty = command.newProperty;
  return *this;
}

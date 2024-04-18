#include "UpdateVisibilityCommand.h"
#include "../RenderThread.h"
#include <vtkProperty.h>

// UpdateVisibilityCommand::UpdateVisibilityCommand(vtkActor *_actorToUpdate,
//                                                  bool _visible)
//     : actorToUpdate(_actorToUpdate), visible(_visible) {}

UpdateVisibilityCommand::UpdateVisibilityCommand(const ModelPart *part)
    : actorToUpdate(part->getVRActor()), visible(part->visible()) {}

UpdateVisibilityCommand::UpdateVisibilityCommand(
    const UpdateVisibilityCommand &command)
    : actorToUpdate(command.actorToUpdate), visible(command.visible) {}

UpdateVisibilityCommand &
UpdateVisibilityCommand::operator=(const UpdateVisibilityCommand &command) {
  if (this == &command) {
    return *this;
  }
  this->actorToUpdate = command.actorToUpdate;
  this->visible = command.visible;
  return *this;
}

void UpdateVisibilityCommand::Execute(RenderThread &renderThread) {
  renderThread.updateVisibility(actorToUpdate, visible);
}

UpdateVisibilityCommand::~UpdateVisibilityCommand() {}

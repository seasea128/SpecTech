#include "RemoveActorCommand.h"
#include "../RenderThread.h"

using Commands::RemoveActorCommand;

RemoveActorCommand::RemoveActorCommand(const ModelPart *part)
    : actorToRemove(part->getVRActor()) {}

RemoveActorCommand::RemoveActorCommand(const RemoveActorCommand &command)
    : actorToRemove(command.actorToRemove) {}

RemoveActorCommand &
RemoveActorCommand::operator=(const RemoveActorCommand &command) {
  if (this == &command) {
    return *this;
  }
  this->actorToRemove = command.actorToRemove;
  return *this;
}

void RemoveActorCommand::Execute(RenderThread &renderThread) {
  renderThread.removeActor(actorToRemove);
}

RemoveActorCommand::~RemoveActorCommand() {}

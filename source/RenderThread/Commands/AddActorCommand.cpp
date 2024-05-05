#include "AddActorCommand.h"
#include "../RenderThread.h"

using Commands::AddActorCommand;

AddActorCommand::AddActorCommand(const ModelPart *part)
    : actorToAdd(part->getVRActor()) {}

AddActorCommand::AddActorCommand(const AddActorCommand &command)
    : actorToAdd(command.actorToAdd) {}

AddActorCommand::AddActorCommand(const vtkSmartPointer<vtkActor> actor)
    : actorToAdd(actor) {}

AddActorCommand &AddActorCommand::operator=(const AddActorCommand &command) {
  if (this == &command) {
    return *this;
  }
  this->actorToAdd = command.actorToAdd;
  return *this;
}

void AddActorCommand::Execute(RenderThread &renderThread) {
  renderThread.addActor(actorToAdd);
}

AddActorCommand::~AddActorCommand() {}

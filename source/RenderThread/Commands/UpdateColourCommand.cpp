#include "UpdateColourCommand.h"

#include "../RenderThread.h"

UpdateColourCommand::UpdateColourCommand(vtkActor *_actorToUpdate,
                                         vtkColor3<unsigned char> _updateColour)
    : actorToUpdate(_actorToUpdate), updateColour(_updateColour) {}

UpdateColourCommand::~UpdateColourCommand() {}

void UpdateColourCommand::Execute(RenderThread &renderThread) {
  renderThread.updateColour(actorToUpdate, updateColour);
}

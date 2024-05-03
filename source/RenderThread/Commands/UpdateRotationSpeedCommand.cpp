#include "UpdateRotationSpeedCommand.h"
#include "../RenderThread.h"

using namespace Commands;

UpdateRotationSpeedCommand::UpdateRotationSpeedCommand(const float x,
                                                       const float y,
                                                       const float z)
    : x(x), y(y), z(z) {}

void UpdateRotationSpeedCommand::Execute(RenderThread &renderThread) {
  renderThread.updateSpeedX(x);
  renderThread.updateSpeedY(y);
  renderThread.updateSpeedZ(z);
}

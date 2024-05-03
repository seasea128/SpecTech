#ifndef UPDATEROTATIONSPEEDCOMMAND_H
#define UPDATEROTATIONSPEEDCOMMAND_H
#include "BaseCommand.h"
namespace Commands {
class UpdateRotationSpeedCommand : public BaseCommand {
public:
  UpdateRotationSpeedCommand(const float x, const float y, const float z);

  virtual void Execute(RenderThread &renderThread) override;

private:
  float x, y, z;
};
} // namespace Commands
#endif // UPDATEROTATIONSPEEDCOMMAND_H

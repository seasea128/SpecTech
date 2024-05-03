#ifndef UPDATEROTATIONSPEEDCOMMAND_H
#define UPDATEROTATIONSPEEDCOMMAND_H

#include "BaseCommand.h"

namespace Commands {
/**
 * Command that will update rotation speed in RenderThread.
 */
class UpdateRotationSpeedCommand : public BaseCommand {

public:
  /**
   * Constructor of UpdatePropertyCommand
   * @param x is the rotation speed in x axis.
   * @param y is the rotation speed in y axis.
   * @param z is the rotation speed in z axis.
   */
  UpdateRotationSpeedCommand(const float x, const float y, const float z);

  /**
   * Function that will be executed in the RenderThread.
   * @param renderThread is used to access RenderThread from AddActorCommand.
   */
  virtual void Execute(RenderThread &renderThread) override;

private:
  float x; /**< New value of x */
  float y; /**< New value of y */
  float z; /**< New value of z */
};
} // namespace Commands
#endif // UPDATEROTATIONSPEEDCOMMAND_H

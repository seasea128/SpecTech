#ifndef REFRESHRENDERCOMMAND_H_
#define REFRESHRENDERCOMMAND_H_

#include "BaseCommand.h"

namespace Commands {

/**
 * Command that will refresh rendering on RenderThread.
 */
class RefreshRenderCommand : public BaseCommand {
  using BaseCommand::BaseCommand;
  using BaseCommand::operator=;

public:
  /**
   * Function that will be executed in the RenderThread.
   * @param renderThread is used to access RenderThread from AddActorCommand.
   */
  virtual void Execute(RenderThread &renderThread) override;
};
} // namespace Commands
#endif // REFRESHRENDERCOMMAND_H_

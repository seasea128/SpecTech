#ifndef ENDRENDERCOMMAND_H_
#define ENDRENDERCOMMAND_H_

#include "BaseCommand.h"

namespace Commands {

/**
 * Command that will end rendering on RenderThread.
 */
class EndRenderCommand : public BaseCommand {
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
#endif // ENDRENDERCOMMAND_H_

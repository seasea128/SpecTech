#ifndef REFRESHRENDERCOMMAND_H_
#define REFRESHRENDERCOMMAND_H_

#include "BaseCommand.h"

namespace Commands {

class RefreshRenderCommand : public BaseCommand {
  using BaseCommand::BaseCommand;
  using BaseCommand::operator=;

public:
  virtual void Execute(RenderThread &renderThread) override;
};
} // namespace Commands
#endif // REFRESHRENDERCOMMAND_H_

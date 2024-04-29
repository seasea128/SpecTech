#ifndef ENDRENDERCOMMAND_H_
#define ENDRENDERCOMMAND_H_

#include "BaseCommand.h"

namespace Commands {

class EndRenderCommand : public BaseCommand {
  using BaseCommand::BaseCommand;
  using BaseCommand::operator=;

public:
  virtual void Execute(RenderThread &renderThread) override;
};
} // namespace Commands
#endif // ENDRENDERCOMMAND_H_

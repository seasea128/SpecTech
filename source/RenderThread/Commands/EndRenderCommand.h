#ifndef ENDRENDERCOMMAND_H_
#define ENDRENDERCOMMAND_H_

#include "BaseCommand.h"

class EndRenderCommand : public BaseCommand {
  using BaseCommand::BaseCommand;
  using BaseCommand::operator=;

public:
  virtual void Execute(RenderThread &renderThread) override;
};

#endif // ENDRENDERCOMMAND_H_

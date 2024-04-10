#ifndef REFRESHRENDERCOMMAND_H_
#define REFRESHRENDERCOMMAND_H_

#include "BaseCommand.h"

class RefreshRenderCommand : public BaseCommand {
  using BaseCommand::BaseCommand;
  using BaseCommand::operator=;

public:
  virtual void Execute(RenderThread &renderThread) override;
};

#endif // REFRESHRENDERCOMMAND_H_

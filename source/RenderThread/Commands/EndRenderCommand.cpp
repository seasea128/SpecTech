#include "EndRenderCommand.h"
#include "../RenderThread.h"

using Commands::EndRenderCommand;

void EndRenderCommand::Execute(RenderThread &renderThread) {
  renderThread.stopRender();
}

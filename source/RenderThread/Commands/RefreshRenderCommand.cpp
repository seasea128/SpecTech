#include "RefreshRenderCommand.h"
#include "../RenderThread.h"

using Commands::RefreshRenderCommand;

void RefreshRenderCommand::Execute(RenderThread &renderThread) {
  renderThread.refreshRender();
}

#include "RefreshRenderCommand.h"
#include "../RenderThread.h"

void RefreshRenderCommand::Execute(RenderThread &renderThread) {
  renderThread.refreshRender();
}

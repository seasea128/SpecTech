#include "EndRenderCommand.h"
#include "../RenderThread.h"

void EndRenderCommand::Execute(RenderThread &renderThread) {
  renderThread.stopRender();
}

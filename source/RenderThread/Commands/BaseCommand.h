#ifndef BASECOMMAND_H_
#define BASECOMMAND_H_

class RenderThread;

namespace Commands {

/**
 * Base class of the command used in sending data between MainWindow
 * and RenderThread.
 */
class BaseCommand {
public:
  BaseCommand();

  // Copy constructor
  BaseCommand(const BaseCommand &command) {}

  // Assignment operator
  BaseCommand &operator=(const BaseCommand &command);

  virtual ~BaseCommand();

  /**
   * This function is what RenderThreadCallback will execute. Every derived
   * class of BaseCommand need to override this function.
   */
  virtual void Execute(RenderThread &renderThread) = 0;
};

} // namespace Commands

#endif // BASECOMMAND_H_

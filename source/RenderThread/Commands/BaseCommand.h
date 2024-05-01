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
  /**
   * Default constructor
   */
  BaseCommand();

  /**
   * Copy constructor
   * @param &command that is being copied.
   */
  BaseCommand(const BaseCommand &command);

  /** Assignment operator
   * @param command that is being assigned.
   * @return assigned BaseCommand.
   */
  BaseCommand &operator=(const BaseCommand &command);

  /**
   * Destructor
   */
  virtual ~BaseCommand();

  /**
   * This function is what RenderThreadCallback will execute. Every derived
   * class of BaseCommand need to override this function.
   * @param renderThread is the RenderThread that the command is executed on.
   */
  virtual void Execute(RenderThread &renderThread) = 0;
};

} // namespace Commands

#endif // BASECOMMAND_H_

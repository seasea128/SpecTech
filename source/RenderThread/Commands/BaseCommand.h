#ifndef BASECOMMAND_H_
#define BASECOMMAND_H_

class RenderThread;

class BaseCommand {
public:
  BaseCommand();

  // Copy constructor
  BaseCommand(const BaseCommand &command) {}

  // Assignment operator
  BaseCommand &operator=(const BaseCommand &command);

  virtual ~BaseCommand();
  virtual void Execute(RenderThread &renderThread) = 0;
};

#endif // BASECOMMAND_H_

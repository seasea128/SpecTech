#include "BaseCommand.h"

using Commands::BaseCommand;

BaseCommand::BaseCommand() {}

BaseCommand::~BaseCommand() {}

BaseCommand &BaseCommand::operator=(const BaseCommand &command) {
  return *this;
}

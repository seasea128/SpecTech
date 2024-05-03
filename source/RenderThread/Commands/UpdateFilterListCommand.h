#ifndef UPDATEFILTERLISTCOMMAND_H_
#define UPDATEFILTERLISTCOMMAND_H_

#include "../../ModelPart.h"
#include "BaseCommand.h"
#include <vtkSmartPointer.h>
namespace Commands {

/**
 * Commands used to add actor to RenderThread.
 */

class UpdateFilterListCommand : public BaseCommand {

public:
  /**
   * Constructor for creating UpdateFilterListCommand with ModelPart.
   * @param part is the part needed to add to RenderThread.
   */
  UpdateFilterListCommand(ModelPart *_part);

  /**
   * Copy constructor for this class.
   * @param command is the command to copy.
   */
  UpdateFilterListCommand(const UpdateFilterListCommand &command) = default;

  /**
   * Assignment operator override for UpdateFilterListCommand.
   * @param command is the command to copy over to the new
   * UpdateFilterListCommand.
   * @return assigned UpdateFilterListCommand.
   */
  UpdateFilterListCommand &
  operator=(const UpdateFilterListCommand &command) = default;

  /**
   * Destructor for UpdateFilterListCommand.
   */
  virtual ~UpdateFilterListCommand();

  /**
   * Function that will be executed in the RenderThread.
   * @param renderThread is used to access RenderThread from
   * UpdateFilterListCommand.
   */
  virtual void Execute(RenderThread &renderThread) override;

private:
  ModelPart *part;
  vtkSmartPointer<vtkSTLReader> file;
  vtkSmartPointer<vtkMapper> mapper;
  std::vector<Filter::FilterData> newFilterList;
};
} // namespace Commands
#endif // UPDATEFILTERLISTCOMMAND_H_

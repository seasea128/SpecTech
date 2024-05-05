#include "UpdateFilterListCommand.h"

#include "../../Utils.h"
#include <vtkSmartPointer.h>

using namespace Commands;

UpdateFilterListCommand::UpdateFilterListCommand(ModelPart *_part)
    : part(_part), newFilterList(Utils::copyFilterList(_part->getFilterList())),
      file(_part->getFileReader()), mapper(_part->getVRMapper()) {}

UpdateFilterListCommand::~UpdateFilterListCommand() {}

void UpdateFilterListCommand::Execute(RenderThread &renderThread) {
  // Create new polyData since it seems like when using the original polyData,
  // it somehow got destructed already after loading the polyData.
  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
  polyData->DeepCopy(file->GetOutputDataObject(0));

  // Update the filter chain
  Utils::setFilterChainFromListWithPolyData(newFilterList, polyData, mapper);
  part->setVRFilterList(newFilterList);
  part->setVRPolyData(polyData);
}

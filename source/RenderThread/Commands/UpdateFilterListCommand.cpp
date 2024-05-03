#include "UpdateFilterListCommand.h"

#include "../../Utils.h"
#include <vtkSmartPointer.h>

using namespace Commands;

UpdateFilterListCommand::UpdateFilterListCommand(ModelPart *_part)
    : part(_part), newFilterList(Utils::copyFilterList(_part->getFilterList())),
      file(_part->getFile()), mapper(_part->getVRMapper()) {}

UpdateFilterListCommand::~UpdateFilterListCommand() {}

void UpdateFilterListCommand::Execute(RenderThread &renderThread) {
  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

  polyData->DeepCopy(file->GetOutputDataObject(0));
  Utils::setFilterFromListWithPolyData(newFilterList, polyData, mapper);
  part->setVRFilterList(newFilterList);
  part->setVRPolyData(polyData);
}

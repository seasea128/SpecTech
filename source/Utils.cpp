#include "Utils.h"
#include "Filters/FilterData.h"
#include <vtkClipDataSet.h>
#include <vtkPlane.h>
#include <vtkShrinkFilter.h>

void Utils::setFilterFromListWithFile(
    const std::vector<Filter::FilterData> &filterList,
    vtkSmartPointer<vtkSTLReader> file, vtkSmartPointer<vtkMapper> mapper) {
  if (filterList.size() == 0) {
    mapper->SetInputConnection(file->GetOutputPort());
    return;
  }

  // Set file to the filter
  filterList[0].filterPointer->SetInputConnection(file->GetOutputPort());
  filterList[0].filterPointer->Update();

  // Set mapper to the last filter in the list
  mapper->SetInputConnection(
      filterList[filterList.size() - 1].filterPointer->GetOutputPort());

  // Don't need to do the loop if there's only 1 filter.
  if (filterList.size() == 1) {
    return;
  }

  // Set the filter chain to each other
  for (int i = 0; i < filterList.size() - 1; i++) {
    filterList[i + 1].filterPointer->SetInputConnection(
        filterList[i].filterPointer->GetOutputPort());
    filterList[i + 1].filterPointer->Update();
  }
}

void Utils::setFilterFromListWithPolyData(
    const std::vector<Filter::FilterData> &filterList,
    vtkSmartPointer<vtkPolyData> polyData, vtkSmartPointer<vtkMapper> mapper) {
  if (filterList.size() == 0) {
    mapper->SetInputDataObject(polyData);
    return;
  }

  // Set file to the filter
  filterList[0].filterPointer->SetInputDataObject(polyData);
  filterList[0].filterPointer->Update();

  // Set mapper to the last filter in the list
  mapper->SetInputConnection(
      filterList[filterList.size() - 1].filterPointer->GetOutputPort());

  // Don't need to do the loop if there's only 1 filter.
  if (filterList.size() == 1) {
    return;
  }

  // Set the filter chain to each other
  for (int i = 0; i < filterList.size() - 1; i++) {
    filterList[i + 1].filterPointer->SetInputConnection(
        filterList[i].filterPointer->GetOutputPort());
    filterList[i + 1].filterPointer->Update();
  }
}

std::vector<Filter::FilterData> Utils::copyFilterList(
    const std::vector<Filter::FilterData> &originalFilterList) {
  std::vector<Filter::FilterData> newFilterList = {};
  for (const auto &filter : originalFilterList) {
    switch (filter.filterType) {
    case Filter::ClipFilter: {
      vtkSmartPointer<vtkPlane> newPlane = vtkSmartPointer<vtkPlane>::New();
      vtkSmartPointer<vtkClipDataSet> clipFilter =
          vtkClipDataSet::SafeDownCast(filter.filterPointer);
      vtkSmartPointer<vtkPlane> originalPlane =
          vtkPlane::SafeDownCast(clipFilter->GetClipFunction());

      double origin[3];
      double normal[3];
      originalPlane->GetOrigin(origin);
      originalPlane->GetNormal(normal);

      newPlane->SetOrigin(origin[0], origin[1], origin[2]);
      newPlane->SetNormal(normal[0], normal[1], normal[2]);

      vtkSmartPointer<vtkClipDataSet> newClipFilter =
          vtkSmartPointer<vtkClipDataSet>::New();

      newClipFilter->SetClipFunction(newPlane);
      newFilterList.push_back({Filter::ClipFilter, newClipFilter});
      break;
    }
    case Filter::ShrinkFilter: {
      auto newShrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
      vtkSmartPointer<vtkShrinkFilter> shrinkFilter =
          vtkShrinkFilter::SafeDownCast(filter.filterPointer);
      newShrinkFilter->SetShrinkFactor(shrinkFilter->GetShrinkFactor());
      newFilterList.push_back({Filter::ShrinkFilter, newShrinkFilter});
      break;
    }
    }
  }
  return newFilterList;
}

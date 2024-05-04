#ifndef UTILS_H_
#define UTILS_H_

#include "ModelPart.h"
#include "RenderThread/RenderThread.h"

/**
 * Static class that contains utility functions.
 */
class Utils {
public:
  /**
   * Recursively adds command of type T to RenderThread.
   * @param renderThread is the RenderThread to add command to.
   * @param currentPart is the current part that is being recursively added to.
   */
  template <typename T>
  static void recursiveAddCommand(RenderThread *renderThread,
                                  ModelPart *currentPart) {
    static_assert(std::is_base_of<Commands::BaseCommand, T>::value,
                  "Given T is not derived from BaseCommand");
    if (currentPart->getVRActor() != nullptr) {
      auto command = std::make_shared<T>(currentPart);
      renderThread->addCommand(command);
    }
    if (currentPart->childCount() > 0) {
      for (int i = 0; i < currentPart->childCount(); i++) {
        recursiveAddCommand<T>(renderThread, currentPart->child(i));
      }
    }
  }

  /**
   * Function that set the connections of filter inside the list.
   * @param filterList is the list to apply.
   * @param file is the file reader.
   * @param mapper is the mapper that is connected to the file.
   */
  static void
  setFilterFromListWithFile(const std::vector<Filter::FilterData> &filterList,
                            vtkSmartPointer<vtkSTLReader> file,
                            vtkSmartPointer<vtkMapper> mapper);

  /**
   * Function that set the connections of filter inside the list.
   * @param filterList is the list to apply.
   * @param polyData is the input vtkPolyData.
   * @param mapper is the mapper that is connected to the file.
   */
  static void setFilterFromListWithPolyData(
      const std::vector<Filter::FilterData> &filterList,
      vtkSmartPointer<vtkPolyData> polyData, vtkSmartPointer<vtkMapper> mapper);

  /**
   * Function that copy the filter list given.
   *
   * This also create new copy of actual vtkAlgorithm inside Filter::FilterData.
   *
   * @param originalFilterList is the vector to copy.
   * @return New copy of originalFilterList.
   */
  static std::vector<Filter::FilterData>
  copyFilterList(const std::vector<Filter::FilterData> &originalFilterList);
};

#endif // UTILS_H_

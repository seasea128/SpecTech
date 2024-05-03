#ifndef FILTERDATA_H_
#define FILTERDATA_H_

#include "FilterType.h"
#include <vtkAlgorithm.h>
#include <vtkSmartPointer.h>

namespace Filter {
typedef struct {
  FilterType filterType;
  vtkSmartPointer<vtkAlgorithm> filterPointer;
} FilterData;
} // namespace Filter

#endif // FILTERDATA_H_

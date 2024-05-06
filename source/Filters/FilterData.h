#ifndef FILTERDATA_H_
#define FILTERDATA_H_

#include "FilterType.h"
#include <vtkAlgorithm.h>
#include <vtkSmartPointer.h>

namespace Filter {

/**
 * Struct for filter list.
 */
typedef struct {
  FilterType filterType;                       /**< Type of filter */
  vtkSmartPointer<vtkAlgorithm> filterPointer; /**< Pointer to the filter */
} FilterData;
} // namespace Filter

#endif // FILTERDATA_H_

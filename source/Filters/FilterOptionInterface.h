#ifndef FILTEROPTIONINTERFACE_H_
#define FILTEROPTIONINTERFACE_H_

namespace Filter {

/**
 * Interface for all widget that can modify filter option.
 *
 * This is done so that all of the widget that can edit filter have a common
 * class that it can be cast to when setting the value of the filter.
 */
class FilterOptionInterface {
public:
  /**
   * Function that will set the value of filter.
   */
  virtual void SetValue() = 0;
};

} // namespace Filter

#endif // FILTEROPTIONINTERFACE_H_

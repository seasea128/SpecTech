#ifndef FILTEROPTIONINTERFACE_H_
#define FILTEROPTIONINTERFACE_H_

namespace Filter {

class FilterOptionInterface {
public:
  virtual ~FilterOptionInterface() {}
  virtual void SetValue() = 0;
};

} // namespace Filter

#endif // FILTEROPTIONINTERFACE_H_

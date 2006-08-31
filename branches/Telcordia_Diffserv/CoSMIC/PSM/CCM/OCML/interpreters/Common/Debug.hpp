#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <fstream>

class Debug {
public:
  Debug()
    : of_("debug.txt")
  {
  }

  template <typename T>
  Debug& operator<<(T t)
  {
    of_ << t << std::endl;
    return *this;
  }

private:
  std::ofstream of_;
};

extern Debug debug;

#endif // DEBUG_HPP


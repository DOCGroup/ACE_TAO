#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <string>
#include <iostream>

namespace common
{

  class Exception
  {
  public:
    Exception(const char* function, const char* description)
      : function_(function), description_(description)
    {
    }

    const char* what() const
    {
      return description_.c_str();
    }

    const char* function() const
    {
      return function_.c_str();
    }

    void print() const
    {
      std::cerr << "Exception occured: " << std::endl
                << description_ << std::endl
                << "in function: " << function_ << std::endl;
    }

  public:
    std::string function_;
    std::string description_;
  };

} // namespace common

#endif // _EXCEPTION_HPP_

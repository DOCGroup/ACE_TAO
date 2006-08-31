#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <string>

class Parse_Error
{
public:
  Parse_Error(const char* msg): msg_(msg) { }

  const char* what() const { return msg_.c_str(); }

private:
  std::string msg_;
};

#endif // _EXCEPTION_HPP_

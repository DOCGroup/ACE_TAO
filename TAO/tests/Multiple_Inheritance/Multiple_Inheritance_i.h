#include "Multiple_InheritanceS.h"

class Multiple_Inheritance_i : public POA_D
{
public:
  Multiple_Inheritance_i () = default;

  Multiple_Inheritance_i (Multiple_Inheritance_i const &) = delete;
  Multiple_Inheritance_i (Multiple_Inheritance_i &&) = delete;
  Multiple_Inheritance_i &operator= (Multiple_Inheritance_i const &) = delete;
  Multiple_Inheritance_i &operator= (Multiple_Inheritance_i &&) = delete;

  char* method1 () override
    {
      return CORBA::string_dup ("method1");
    }
  char* method2 () override
    {
      return CORBA::string_dup ("method2");
    }
  char* method3 () override
    {
      return CORBA::string_dup ("method3");
    }
  char* method4 () override
    {
      return CORBA::string_dup ("method4");
    }
};

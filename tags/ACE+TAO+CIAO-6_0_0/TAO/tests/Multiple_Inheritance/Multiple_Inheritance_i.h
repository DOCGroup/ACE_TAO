// $Id$

#include "Multiple_InheritanceS.h"

class Multiple_Inheritance_i : public POA_D
{
public:
  Multiple_Inheritance_i (void);

  virtual char* method1 (void)
    {
      return CORBA::string_dup ("method1");
    }
  virtual char* method2 (void)
    {
      return CORBA::string_dup ("method2");
    }
  virtual char* method3 (void)
    {
      return CORBA::string_dup ("method3");
    }
  virtual char* method4 (void)
    {
      return CORBA::string_dup ("method4");
    }
};

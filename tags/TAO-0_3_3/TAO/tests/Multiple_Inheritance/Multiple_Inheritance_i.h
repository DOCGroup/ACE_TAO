// $Id$

#include "Multiple_InheritanceS.h"

ACE_RCSID(Multiple_Inheritance, Multiple_Inheritance_i, "$Id$")

class Multiple_Inheritance_i : public POA_D
{
public:
  virtual CORBA::String method1 (CORBA::Environment &)
    {
      return CORBA::string_dup ("method1");
    }
  virtual CORBA::String method2 (CORBA::Environment &)
    {
      return CORBA::string_dup ("method2");
    }
  virtual CORBA::String method3 (CORBA::Environment &)
    {
      return CORBA::string_dup ("method3");
    }
  virtual CORBA::String method4 (CORBA::Environment &)
    {
      return CORBA::string_dup ("method4");
    }
};

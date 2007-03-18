//
// $Id$
//

#include "TestS.h"

class MyInterfaceImpl
  : public virtual POA_MyInterface
{
  public:
  MyInterfaceImpl (CORBA::ORB_ptr orb);

  static const char* my_string;

  // = The skeleton methods
  virtual CORBA::Boolean myMethod (const char* stringParam);

private:
  CORBA::ORB_var orb_;
};

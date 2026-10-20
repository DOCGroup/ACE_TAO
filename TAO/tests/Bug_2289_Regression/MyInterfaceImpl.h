
#include "TestS.h"

class MyInterfaceImpl
  : public virtual POA_MyInterface
{
  public:
  MyInterfaceImpl (CORBA::ORB_ptr orb);
  MyInterfaceImpl (MyInterfaceImpl const &) = delete;
  MyInterfaceImpl &operator= (MyInterfaceImpl const &) = delete;
  MyInterfaceImpl (MyInterfaceImpl &&) = delete;
  MyInterfaceImpl &operator= (MyInterfaceImpl &&) = delete;

  static char const *my_string;

  // = The skeleton methods
  CORBA::Boolean myMethod (char const *stringParam) override;

private:
  CORBA::ORB_var orb_;
};

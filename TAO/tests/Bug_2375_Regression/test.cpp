#include "testC.h"

class MyTestValuetypeFactory : public TestValuetype_init
{
public:
  MyTestValuetypeFactory () = default;
  MyTestValuetypeFactory (MyTestValuetypeFactory const &) = delete;
  MyTestValuetypeFactory &operator= (MyTestValuetypeFactory const &) = delete;
  MyTestValuetypeFactory (MyTestValuetypeFactory &&) = delete;
  MyTestValuetypeFactory &operator= (MyTestValuetypeFactory &&) = delete;

  TestValuetype *create (::CORBA::Boolean, ::CORBA::Short) override { return nullptr; }
  TAO_OBV_CREATE_RETURN_TYPE (TestValuetype) create_for_unmarshal () override { return nullptr; }
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  MyTestValuetypeFactory myFactory;
  return 0;
}

// $Id$
#include "testC.h"
class  MyTestValuetypeFactory : public TestValuetype_init {
public:
  virtual TestValuetype* create (::CORBA::Boolean, ::CORBA::Short) {return 0;};
  virtual TAO_OBV_CREATE_RETURN_TYPE (TestValuetype) create_for_unmarshal (void) {return 0;};
};
int ACE_TMAIN (int, ACE_TCHAR *[]) {
  MyTestValuetypeFactory myFactory;
  return 0;
}

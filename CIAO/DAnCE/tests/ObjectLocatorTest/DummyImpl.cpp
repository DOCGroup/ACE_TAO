#include "DummyImpl.h"

namespace ObjectLocatorTest
  {

    char * DummyImpl::sayHello (
    )
    {
      return CORBA::string_dup (this->helloSentence);
    }

  } // namespace ObjectLocatorTest


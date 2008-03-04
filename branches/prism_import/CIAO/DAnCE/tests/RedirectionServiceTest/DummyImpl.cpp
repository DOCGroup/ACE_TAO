#include "DummyImpl.h"

namespace Test
  {

    char * DummyImpl::sayHello (
    )
    {
      return CORBA::string_dup (this->helloSentence);
    }

  } // namespace ObjectLocatorTest


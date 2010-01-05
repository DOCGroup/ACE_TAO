// $Id$
#include "DummyImpl.h"

namespace ObjectLocatorTest
  {

    char * DummyImpl::sayHello (void)
    {
      return CORBA::string_dup (this->helloSentence);
    }

  } // namespace ObjectLocatorTest


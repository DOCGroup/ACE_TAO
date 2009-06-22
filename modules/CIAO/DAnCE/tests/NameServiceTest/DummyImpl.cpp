// $Id$
#include "DummyImpl.h"

namespace NameServiceTest
  {

    char * DummyImpl::sayHello (
    )
    {
      return CORBA::string_dup (this->helloSentence);
    }

  } // namespace ObjectLocatorTest


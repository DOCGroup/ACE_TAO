#include "DummyImpl.h"

namespace ObjectLocatorTest
  {

    char * DummyImpl::sayHello (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ( (
                       ::CORBA::SystemException
                     ))
    {
      return CORBA::string_dup (this->helloSentence);
    }

  } // namespace ObjectLocatorTest


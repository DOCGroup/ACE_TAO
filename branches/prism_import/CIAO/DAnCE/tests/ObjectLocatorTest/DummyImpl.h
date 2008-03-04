#ifndef DUMMYIMPL_H_
#define DUMMYIMPL_H_

#include "DummyS.h"

namespace ObjectLocatorTest
  {

    class DummyImpl
          : public virtual POA_ObjectLocatorTest::Dummy
      {
      public:
        DummyImpl() : helloSentence ("A dummy hello sentence.") {};

        virtual char * sayHello (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
        )
        ACE_THROW_SPEC ( (
                           ::CORBA::SystemException
                         ));

      private:
        const char * helloSentence;
      };

  } // namespace POA_ObjectLocatorTest

#endif /*DUMMYIMPL_H_*/

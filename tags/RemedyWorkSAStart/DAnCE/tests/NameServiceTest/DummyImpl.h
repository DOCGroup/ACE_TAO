// $Id$
#ifndef DUMMYIMPL_H_
#define DUMMYIMPL_H_

#include "DummyS.h"

namespace NameServiceTest
  {

    class DummyImpl
          : public virtual POA_NameServiceTest::Dummy
      {
      public:
        DummyImpl() : helloSentence ("A dummy hello sentence.") {};

        virtual char * sayHello (
        );

      private:
        const char * helloSentence;
      };

  } // namespace POA_ObjectLocatorTest

#endif /*DUMMYIMPL_H_*/

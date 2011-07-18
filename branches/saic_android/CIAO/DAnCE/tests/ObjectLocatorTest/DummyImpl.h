// $Id$
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

        virtual char * sayHello (void);

      private:
        const char * helloSentence;
      };
  } // namespace POA_ObjectLocatorTest

#endif /*DUMMYIMPL_H_*/

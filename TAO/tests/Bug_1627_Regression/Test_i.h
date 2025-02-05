// -*- C++ -*-

#if !defined BUG_1330_REGRESSION_TEST_I_H
 #define BUG_1330_REGRESSION_TEST_I_H

#include "TestS.h"

class Test_i: public POA_Test
{
public:
   /// Constructor
   Test_i ();

   /// Destructor
   ~Test_i ();

   /// A method
   void test_method();
};

#endif /* ! BUG_1330_REGRESSION_TEST_I_H */

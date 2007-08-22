//
// $Id$
//
// -*- C++ -*-

#if !defined BUG_1330_REGRESSION_TEST_I_H
 #define BUG_1330_REGRESSION_TEST_I_H

#include "TestS.h"

class Test_i: public POA_Test
{
public:
   /// Constructor
   Test_i (void);

   /// Destructor
   ~Test_i (void);

   /// A method
   void test_method(void);
};

#endif /* ! BUG_1330_REGRESSION_TEST_I_H */

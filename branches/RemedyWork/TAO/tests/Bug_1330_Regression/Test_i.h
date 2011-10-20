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
  Test_i (CORBA::ORB_ptr orb);

  /// Destructor
  ~Test_i (void);

  /// A method
  virtual void test_method(void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif /* ! BUG_1330_REGRESSION_TEST_I_H */

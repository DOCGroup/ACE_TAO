// $Id$
// This file is kept as simple as possible, with hopes that it will
// be maintained as features are added to the NS.
#ifndef TAO_NOTIFY_UTEST_TESTQOS_H
#define TAO_NOTIFY_UTEST_TESTQOS_H
#include "UnitTester.h"

// This particular unit test case is for testing out the basic
// features available to a corba client application. (ie Utils, Suppliers, Consumers)
namespace TestQoS {

  // Test that all NS objects have an ID which can be used in various ways
  void testQoSChannel(ACE_ENV_SINGLE_ARG_DECL);

  inline void testAll(ACE_ENV_SINGLE_ARG_DECL) {
    testQoSChannel(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

#endif

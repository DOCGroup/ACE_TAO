// $Id$
// This file is kept as simple as possible, with hopes that it will
// be maintained as features are added to the NS.
#ifndef TAO_NOTIFY_UTEST_TESTBASIC_H
#define TAO_NOTIFY_UTEST_TESTBASIC_H
#include "UnitTester.h"

// This particular unit test case is for testing out the basic
// features available to a corba client application. (ie Utils, Suppliers, Consumers)
namespace TestBasic {

  // Test that all NS objects have an ID which can be used in various ways
  void testChannelCreate(ACE_ENV_SINGLE_ARG_DECL);
  void testAdminCreate(ACE_ENV_SINGLE_ARG_DECL);

  inline void testAll(ACE_ENV_SINGLE_ARG_DECL) {
    testChannelCreate(ACE_ENV_SINGLE_ARG_PARAMETER);
    testAdminCreate(ACE_ENV_SINGLE_ARG_PARAMETER);
  }
}

#endif

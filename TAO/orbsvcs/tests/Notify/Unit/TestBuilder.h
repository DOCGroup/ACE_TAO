// $Id$
#ifndef TAO_NOTIFY_UTEST_TESTBUILDER_H
#define TAO_NOTIFY_UTEST_TESTBUILDER_H
#include "UnitTester.h"

namespace TestBuilder {

  void testBuildECF(ACE_ENV_SINGLE_ARG_DECL);
  void testBuildEC(ACE_ENV_SINGLE_ARG_DECL);
  void testTopologyPersist(ACE_ENV_SINGLE_ARG_DECL);
  void testSubscriptionTopology(ACE_ENV_SINGLE_ARG_DECL);

  inline void testAll(ACE_ENV_SINGLE_ARG_DECL) {
    testBuildECF(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    testBuildEC(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    testTopologyPersist(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    testSubscriptionTopology(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

#endif

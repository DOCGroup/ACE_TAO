/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = FILENAME
//   TestConfig
//
// = AUTHOR
//   Bryan Thrall (thrall@cse.wustl.edu)
//
// ============================================================================

#ifndef TESTCONFIG_H
#define TESTCONFIG_H

#include "ace/Array.h"
#include "ace/Bound_Ptr.h"
#include "ace/Synch.h" //for ACE_Null_Mutex

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TestConfig {

struct test_config_t;
typedef ACE_Array<test_config_t*> Test_Config_Set;
typedef ACE_Weak_Bound_Ptr<Test_Config_Set,ACE_Null_Mutex> TCFG_SET_WPTR;

// Entity_Type_t is used to distinguish different types of entities
// (such as Event Channel events and Distributable Threads). Not exactly
// an enumeration of those types, but it should take on reasonably
// distinct values for each.
typedef unsigned long Entity_Type_t;

typedef long Period_t;

enum Criticality_t {
// Defines the criticality of the entity.
  VERY_LOW_CRITICALITY,
  LOW_CRITICALITY,
  MEDIUM_CRITICALITY,
  HIGH_CRITICALITY,
  VERY_HIGH_CRITICALITY
};

enum Importance_t {
// Defines the importance of the entity,
// which can be used as a "tie-breaker" when
// other scheduling parameters are equal.
  VERY_LOW_IMPORTANCE,
  LOW_IMPORTANCE,
  MEDIUM_IMPORTANCE,
  HIGH_IMPORTANCE,
  VERY_HIGH_IMPORTANCE
};


struct test_config_t
// = TITLE
//   Test configuration information for the back-end.
//
// = DESCRIPTION
//   The QoS and number of tasks for each
//   test "entity" described by the following
//   information.
{
  // The entity type should uniquely identify the
  // set of entities configured by this struct.
  Entity_Type_t type;

  // This expresses the rate at which entities are
  // pushed.
  Period_t period;

  // Entity Criticality (user assigned significance).
  Criticality_t criticality;

  // Entity importance, used to "break ties".
  Importance_t importance;

  // Number of entities to push through the back-end. This is
  // effectively a termination condition for the test, since it will
  // terminate once all num_entities entities have been pushed for
  // each test_config_t used to configure the back-end.
  long num_entities;
};

class Test_Config {
  // = TITLE
  //   Interface for configuring the test back-end.
  //
  // = DESCRIPTION
  //   This class provides an interface for configuring the back-end of the test.
  //   For example, the Event Channel might be the back-end, so an adapter
  //   implementing this interface would be used to configure the EC in that case.
  //
public:
  Test_Config (void) {}

  virtual ~Test_Config (void);

  virtual int configure (TCFG_SET_WPTR configs) = 0;
  // Configures the back-end. Each test_config_t in the set specifies
  // the configuration of a separate type of entity. Returns 0 when
  // the configuration is successful, non-zero otherwise.  An
  // ACE_Weak_Bound_Ptr is used because the TestConfig might want to
  // keep a pointer to the Test_Config_Set but should not take
  // possession of the set (that is, control when the set is deleted).

  virtual int run (void) = 0;
  // Runs the configured back-end. Returns 0 if the run encountered
  // no errors, non-zero otherwise. Entities are pushed periodically
  // according to their respective test_config_t's until num_entities
  // are pushed of each test_config_t.

  //NOTE: It might be useful to distinguish between a run which
  //returns after the last entity was pushed and one which returns
  //after the last entity was received at its destination.
};

} /* namespace TestConfig */

#endif /* TESTCONFIG_H */

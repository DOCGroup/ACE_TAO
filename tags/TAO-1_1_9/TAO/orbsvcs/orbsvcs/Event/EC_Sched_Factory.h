/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Sched_Factory
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_SCHED_FACTORY_H
#define TAO_EC_SCHED_FACTORY_H
#include "ace/pre.h"

#include "EC_Default_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "sched_event_export.h"

class TAO_RTSchedEvent_Export TAO_EC_Sched_Factory : public TAO_EC_Default_Factory
{
  // = TITLE
  //   Extend the default factory to support scheduling
  //
  // = DESCRIPTION
  //
  // = MEMORY MANAGMENT
  //
public:
  TAO_EC_Sched_Factory (void);
  // Constructor

  virtual ~TAO_EC_Sched_Factory (void);
  // destructor...

  // = The Service_Object entry points
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);

  // = The EC_Factory methods
  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel*);
  virtual TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel*);
  virtual TAO_EC_Timeout_Generator*
      create_timeout_generator (TAO_EC_Event_Channel*);
  virtual TAO_EC_Scheduling_Strategy*
      create_scheduling_strategy (TAO_EC_Event_Channel*);

private:
};

#if defined (__ACE_INLINE__)
#include "EC_Sched_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_EC_Sched_Factory)
ACE_FACTORY_DECLARE (TAO_RTSchedEvent, TAO_EC_Sched_Factory)

#include "ace/post.h"
#endif /* TAO_EC_SCHED_FACTORY_H */

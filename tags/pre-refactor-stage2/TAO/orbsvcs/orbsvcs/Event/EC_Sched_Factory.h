/* -*- C++ -*- */
/**
 *  @file   EC_Sched_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_SCHED_FACTORY_H
#define TAO_EC_SCHED_FACTORY_H
#include "ace/pre.h"

#include "EC_Default_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "sched_event_export.h"

/**
 * @class TAO_EC_Sched_Factory
 *
 * @brief Extend the default factory to support scheduling
 *
 */
class TAO_RTSchedEvent_Export TAO_EC_Sched_Factory : public TAO_EC_Default_Factory
{
public:
  /// Constructor
  TAO_EC_Sched_Factory (void);

  /// destructor...
  virtual ~TAO_EC_Sched_Factory (void);

  // = The Service_Object entry points
  virtual int init (int argc, ACE_TCHAR* argv[]);
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

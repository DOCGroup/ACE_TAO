// -*- C++ -*-

//=============================================================================
/**
 *  @file RT_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_RT_ORB_INITIALIZER_H
#define TAO_RT_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// RTCORBA ORB initializer.
class TAO_RT_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:
  /// Priority mapping types
  enum
    {
      TAO_PRIORITY_MAPPING_CONTINUOUS,
      TAO_PRIORITY_MAPPING_LINEAR,
      TAO_PRIORITY_MAPPING_DIRECT
    };

  enum
    {
      TAO_NETWORK_PRIORITY_MAPPING_LINEAR
    };

  /**
   * Lifespan of the dynamic threads
   * TAO_RTCORBA_DT_INFINITIVE When the Dynamic Thread is created it will run
   * forever
   * TAO_RTCORBA_DT_IDLE When the Dynamic Thread is created it will run until
   * it has been idle for the specified amount of time
   * TAO_RTCORBA_DT_FIXED When the Dynamic Thread is created it will run for
   * the specified fix amount of time
   */
  enum TAO_RTCORBA_DT_LifeSpan
  {
    TAO_RTCORBA_DT_INFINITIVE,
    TAO_RTCORBA_DT_IDLE,
    TAO_RTCORBA_DT_FIXED
  };

  TAO_RT_ORBInitializer (int priority_mapping_type,
                         int network_priority_mapping_type,
                         int ace_sched_policy,
                         long sched_policy,
                         long scope_policy,
                         TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan,
                         ACE_Time_Value const &dynamic_thread_time);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:

  /// Register RTCORBA policy factories.
  void register_policy_factories (PortableInterceptor::ORBInitInfo_ptr info);

private:
  /// Instance of the RTCorba policy factory.
  /**
   * The RTCorba policy factory is stateless and reentrant, so share
   * a single instance between all ORBs.
   */
  PortableInterceptor::PolicyFactory_var policy_factory_;

  /// Priority mapping type.
  int const priority_mapping_type_;

  /// Network Priority mapping type.
  int const network_priority_mapping_type_;

  /// Scheduling policy.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by
   * functions like ACE_OS::thr_setprio() and
   * ACE_Sched_Params::priority_min(). Legal values are ACE_SCHED_RR,
   * ACE_SCHED_FIFO, and ACE_SCHED_OTHER.
   */
  int const ace_sched_policy_;

  /// Scheduling policy flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCHED_RR,
   * THR_SCHED_FIFO, and THR_SCHED_DEFAULT.
   */
  long const sched_policy_;

  /// Scheduling scope flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBScopePolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCOPE_SYSTEM and
   * THR_SCOPE_PROCESS.
   */
  long const scope_policy_;

  /// Dynamic thread lifespan policy
  TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan_;

  /// Dynamic thread time
  /**
   * When using thread pool a certain number of dynamic threads can be created.
   * By default these threads are created when needed but never end. Optionally
   * a time can be specified
   */
  ACE_Time_Value const dynamic_thread_time_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_ORB_INITIALIZER_H */

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

#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "RT_PolicyFactory.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// RTCORBA ORB initializer.
class TAO_RTCORBA_Export TAO_RT_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
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

  TAO_RT_ORBInitializer (int priority_mapping_type,
                         int network_priority_mapping_type,
                         int ace_sched_policy,
                         long sched_policy,
                         long scope_policy);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Register RTCORBA policy factories.
  void register_policy_factories (
                                  PortableInterceptor::ORBInitInfo_ptr info
                                  ACE_ENV_ARG_DECL);

private:

  /// Instance of the RTCORBA policy factory.
  /// The RTCORBA policy factory is stateless and reentrant, so share
  /// a single instance between all ORBs.
  TAO_RT_PolicyFactory policy_factory_;

  /// Priority mapping type.
  int priority_mapping_type_;

  /// Network Priority mapping type.
  int network_priority_mapping_type_;

  /// Scheduling policy.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by
   * functions like ACE_OS::thr_setprio() and
   * ACE_Sched_Params::priority_min(). Legal values are ACE_SCHED_RR,
   * ACE_SCHED_FIFO, and ACE_SCHED_OTHER.
   */
  int ace_sched_policy_;

  /// Scheduling policy flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCHED_RR,
   * THR_SCHED_FIFO, and THR_SCHED_DEFAULT.
   */
  long sched_policy_;

  /// Scheduling scope flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBScopePolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCOPE_SYSTEM and
   * THR_SCOPE_PROCESS.
   */
  long scope_policy_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_RT_ORB_INITIALIZER_H */

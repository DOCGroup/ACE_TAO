// -*- C++ -*-

//=============================================================================
/**
 *  @file   Connection_Timeout_Policy_i.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan (bala@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_CONNECTION_TIMEOUT_POLICY_I_H
#define TAO_CONNECTION_TIMEOUT_POLICY_I_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Messaging/TAO_ExtC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*
 * @class TAO_ConnectionTimeoutPolicy
 *
 * @brief TAO::ConnectionTimeoutPolicy implementation
 *
 *  This policy controls the connection timeout time while
 *  establishing connections. This policy is proprietary to TAO.
 */
class TAO_ConnectionTimeoutPolicy
  : public TAO::ConnectionTimeoutPolicy,
    public ::CORBA::LocalObject
{

public:
  /// Constructor.
  TAO_ConnectionTimeoutPolicy (const TimeBase::TimeT& relative_expiry);

  /// Copy constructor.
  TAO_ConnectionTimeoutPolicy (const TAO_ConnectionTimeoutPolicy &rhs);

  /// Implement the timeout hook, this is set in the ORB_Core at
  /// initialization time.
  static void hook (TAO_ORB_Core *orb_core,
                    TAO_Stub *stub,
                    bool &has_timeout,
                    ACE_Time_Value &time_value);

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any& val);

  /// Returns a copy of <this>.
  virtual TAO_ConnectionTimeoutPolicy *clone (void) const;

  // = The TAO::ConnectionTinoutPolicy methods
  virtual TimeBase::TimeT relative_expiry (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  /// Change the CORBA representation to the ACE representation.
  void set_time_value (ACE_Time_Value &time_value);

  /// Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:
  /// The attribute
  TimeBase::TimeT relative_expiry_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_CONNECTION_TIMEOUT_POLICY_I_H */

/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   FT_Policy_i.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_POLICY_I_H
#define TAO_FT_POLICY_I_H
#include "ace/pre.h"

#include "orbsvcs/FT_CORBA_ORBC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// @@ This class inherits from TAO_Local_RefCounted_Object as the
// functionality outlined in the spec seems to indicate that. I could
// very well be wrong - Bala

/**
 * @class TAO_FT_Request_Duration_Policy
 *
 * @brief FT::RequestDurationPolicy implementation
 *
 * This policy controls the request duration in the ORB. The
 * semantics are like this. On the server side, the server should
 * retain the details of the request from the client (and the
 * reply sent to it too) atleast for the time period specified by
 * this policy value. On the client side, the client uses this
 * value to calculate the expiration_id in the RequestService
 * context. The expiration_id is a sort of timeout for the client
 * to keep trying to connect to server object groups under certain
 * conditions.
 */
class TAO_FT_Export TAO_FT_Request_Duration_Policy:
  public FT::RequestDurationPolicy,
  public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_FT_Request_Duration_Policy (const TimeBase::TimeT& relative_expiry);

  /// Copy constructor.
  TAO_FT_Request_Duration_Policy (const TAO_FT_Request_Duration_Policy &rhs);

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Returns a copy of <this>.
  virtual TAO_FT_Request_Duration_Policy *clone (void) const;

  // = The FT::RequestDurationPolicy methods
  virtual TimeBase::TimeT request_duration_value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Change the CORBA representation to the ACE representation.
  void set_time_value (ACE_Time_Value &time_value);

private:
  /// The attribute
  TimeBase::TimeT request_duration_;
};


/**
 * @class TAO_FT_Heart_Beat_Policy
 *
 * @brief FT::HeartBeatPolicy
 *
 * If this  policy is set, it enables the client ORB to send
 * heartbeats to the server ORB over the open connections.
 */
class TAO_FT_Export TAO_FT_Heart_Beat_Policy:
  public FT::HeartbeatPolicy,
  public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_FT_Heart_Beat_Policy (const CORBA::Boolean boolean,
                            const TimeBase::TimeT &interval,
                            const TimeBase::TimeT &timeout);

  /// Copy constructor.
  TAO_FT_Heart_Beat_Policy (const TAO_FT_Heart_Beat_Policy &rhs);

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Returns a copy of <this>.
  virtual TAO_FT_Heart_Beat_Policy *clone (void) const;

  // = The FT::HeartBeatPolicy methods
  virtual FT::HeartbeatPolicyValue heartbeat_policy_value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Change the CORBA representation to the ACE representation.
  void set_time_value (ACE_Time_Value &time_value,
                       const TimeBase::TimeT &timebase);

private:

  // The attributes
  /// <heartbeat_> indicates whether heartbeating is enabled or not.
  CORBA::Boolean heartbeat_;

  /// Intervals in which heartbeat messages need to be sent
  TimeBase::TimeT heartbeat_interval_;

  /// The timeouts for the heartbeats
  TimeBase::TimeT heartbeat_timeout_;
};

/**
 * @class TAO_FT_Heart_Beat_Enabled_Policy
 *
 * @brief FT::HeartBeatEnabledPolicy
 *
 * If this  policy is set, it enables the server ORB to set
 * the TAG_FT_HEARTBEAT_ENABLED component in the IOP profile of
 * the IOR that it exposes
 */
class TAO_FT_Export TAO_FT_Heart_Beat_Enabled_Policy:
  public FT::HeartbeatEnabledPolicy,
  public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_FT_Heart_Beat_Enabled_Policy (const CORBA::Boolean boolean);

  /// Copy constructor.
  TAO_FT_Heart_Beat_Enabled_Policy (
      const TAO_FT_Heart_Beat_Enabled_Policy &rhs);

  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  /// Returns a copy of <this>.
  virtual TAO_FT_Heart_Beat_Enabled_Policy *clone (void) const;

  // = The FT::HeartBeatPolicy methods
  virtual CORBA::Boolean heartbeat_enabled_policy_value (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// indicates whether heartbeating is enabled or not.
  CORBA::Boolean heartbeat_enabled_value_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "FT_Policy_i.i"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* TAO_FT_POLICY_I_H */

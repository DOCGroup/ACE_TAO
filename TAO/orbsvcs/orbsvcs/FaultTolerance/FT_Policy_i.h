/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   FaultTolerance
//
// = DESCRIPTION
//   Implementation classes for the FT related policies
//
// = FILENAME
//   FT_Policy_i.h
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

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

class TAO_FT_Export TAO_FT_Request_Duration_Policy:
  public FT::RequestDurationPolicy,
  public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   FT::RequestDurationPolicy implementation
  //
  // = DESCRIPTION
  //   This policy controls the request duration in the ORB. The
  //   semantics are like this. On the server side, the server should
  //   retain the details of the request from the client (and the
  //   reply sent to it too) atleast for the time period specified by
  //   this policy value. On the client side, the client uses this
  //   value to calculate the expiration_id in the RequestService
  //   context. The expiration_id is a sort of timeout for the client
  //   to keep trying to connect to server object groups under certain
  //   conditions
  //
public:

  TAO_FT_Request_Duration_Policy (const TimeBase::TimeT& relative_expiry);
  // Constructor.

  TAO_FT_Request_Duration_Policy (const TAO_FT_Request_Duration_Policy &rhs);
  // Copy constructor.

  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  virtual TAO_FT_Request_Duration_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The FT::RequestDurationPolicy methods
  virtual TimeBase::TimeT request_duration_value (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void set_time_value (ACE_Time_Value &time_value);
  // Change the CORBA representation to the ACE representation.

private:
  TimeBase::TimeT request_duration_;
  // The attribute
};


class TAO_FT_Export TAO_FT_Heart_Beat_Policy:
  public FT::HeartbeatPolicy,
  public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   FT::HeartBeatPolicy
  //
  // = DESCRIPTION
  //   If this  policy is set, it enables the client ORB to send
  //   heartbeats to the server ORB over the open connections.
  //
public:

  TAO_FT_Heart_Beat_Policy (const CORBA::Boolean boolean,
                            const TimeBase::TimeT &interval,
                            const TimeBase::TimeT &timeout);
  // Constructor.

  TAO_FT_Heart_Beat_Policy (const TAO_FT_Heart_Beat_Policy &rhs);
  // Copy constructor.

  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  virtual TAO_FT_Heart_Beat_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The FT::HeartBeatPolicy methods
  virtual FT::HeartbeatPolicyValue heartbeat_policy_value (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void set_time_value (ACE_Time_Value &time_value,
                       const TimeBase::TimeT &timebase);
  // Change the CORBA representation to the ACE representation.

private:

  // The attributes
  CORBA::Boolean heartbeat_;
  // <heartbeat_> indicates whether heartbeating is enabled or not.

  TimeBase::TimeT heartbeat_interval_;
  // Intervals in which heartbeat messages need to be sent

  TimeBase::TimeT heartbeat_timeout_;
  // The timeouts for the heartbeats
};


class TAO_FT_Export TAO_FT_Heart_Beat_Enabled_Policy:
  public FT::HeartbeatEnabledPolicy,
  public TAO_Local_RefCounted_Object
{
  // = TITLE
  //   FT::HeartBeatEnabledPolicy
  //
  // = DESCRIPTION
  //   If this  policy is set, it enables the server ORB to set
  //   the TAG_FT_HEARTBEAT_ENABLED component in the IOP profile of
  //   the IOR that it exposes
  //
public:

  TAO_FT_Heart_Beat_Enabled_Policy (const CORBA::Boolean boolean);
  // Constructor.

  TAO_FT_Heart_Beat_Enabled_Policy (
      const TAO_FT_Heart_Beat_Enabled_Policy &rhs);
  // Copy constructor.

  static CORBA::Policy_ptr create (const CORBA::Any& val
                                   TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  virtual TAO_FT_Heart_Beat_Enabled_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The FT::HeartBeatPolicy methods
  virtual CORBA::Boolean heartbeat_enabled_policy_value (
      TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::Boolean heartbeat_enabled_value_;
  // indicates whether heartbeating is enabled or not.
};

#if defined (__ACE_INLINE__)
#include "FT_Policy_i.i"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* TAO_FT_POLICY_I_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_ClientPolicy_i.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_CLIENTPOLICY_I_H
#define TAO_FT_CLIENTPOLICY_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/FT_CORBA_ORBC.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "tao/LocalObject.h"
#include "orbsvcs/FaultTolerance/FT_ClientORB_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_FT_Request_Duration_Policy
  : public FT::RequestDurationPolicy,
    public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_FT_Request_Duration_Policy (const TimeBase::TimeT& relative_expiry);

  /// Copy constructor.
  TAO_FT_Request_Duration_Policy (const TAO_FT_Request_Duration_Policy &rhs);

  /// Helper method for the implementation of
  /// CORBA::ORB::create_policy.
  static CORBA::Policy_ptr create (const CORBA::Any& val);

  /// Returns a copy of <this>.
  virtual TAO_FT_Request_Duration_Policy *clone (void) const;

  // = The FT::RequestDurationPolicy methods
  virtual TimeBase::TimeT request_duration_policy_value (void);

  virtual CORBA::PolicyType policy_type ();

  virtual CORBA::Policy_ptr copy ();

  virtual void destroy ();

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
class TAO_FT_Heart_Beat_Policy
  : public FT::HeartbeatPolicy,
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
  static CORBA::Policy_ptr create (const CORBA::Any& val);

  /// Returns a copy of <this>.
  virtual TAO_FT_Heart_Beat_Policy *clone (void) const;

  // = The FT::HeartBeatPolicy methods
  virtual FT::HeartbeatPolicyValue heartbeat_policy_value (void);

  virtual CORBA::PolicyType policy_type ();

  virtual CORBA::Policy_ptr copy ();

  virtual void destroy ();

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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "orbsvcs/FaultTolerance/FT_ClientPolicy_i.inl"
#endif /* __ACE_INLINE__ */


#include /**/ "ace/post.h"
#endif /* TAO_FT_CLIENTPOLICY_I_H */

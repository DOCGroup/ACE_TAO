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

#include "orbsvcs/FT_CORBAC.h"

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

  static CORBA::Policy_ptr create (const CORBA::Any& val,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  virtual TAO_FT_Request_Duration_Policy *clone (void) const;
  // Returns a copy of <this>.

  // = The FT::RequestDurationPolicy methods
  virtual TimeBase::TimeT request_duration_value (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void set_time_value (ACE_Time_Value &time_value);
  // Change the CORBA representation to the ACE representation.

private:
  TimeBase::TimeT request_duration_;
  // The attribute
};


#if defined (__ACE_INLINE__)
#include "FT_Policy_i.i"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* TAO_FT_POLICY_I_H */

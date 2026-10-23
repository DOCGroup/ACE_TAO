// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_ServerPolicy_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_SERVERPOLICY_I_H
#define TAO_FT_SERVERPOLICY_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/FT_CORBA_ORBC.h"
#include "orbsvcs/FaultTolerance/FT_ServerORB_export.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_FT_Heart_Beat_Enabled_Policy
 *
 * @brief FT::HeartBeatEnabledPolicy
 *
 * If this  policy is set, it enables the server ORB to set
 * the TAG_FT_HEARTBEAT_ENABLED component in the IOP profile of
 * the IOR that it exposes
 */
class TAO_FT_Heart_Beat_Enabled_Policy
  : public FT::HeartbeatEnabledPolicy
  , public ::CORBA::LocalObject
{
public:
  /// Constructor.
  explicit TAO_FT_Heart_Beat_Enabled_Policy (CORBA::Boolean const boolean);

  /// Copy constructor.
  TAO_FT_Heart_Beat_Enabled_Policy (
      TAO_FT_Heart_Beat_Enabled_Policy const &rhs);

  static CORBA::Policy_ptr create (CORBA::Any const &val);

  // Helper method for the implementation of
  // CORBA::ORB::create_policy.

  /// Returns a copy of <this>.
  TAO_FT_Heart_Beat_Enabled_Policy *clone () const;

  // = The FT::HeartBeatPolicy methods
  CORBA::Boolean heartbeat_enabled_policy_value () override;

  CORBA::PolicyType policy_type () override;

  CORBA::Policy_ptr copy () override;

  void destroy () override;

private:
  /// indicates whether heartbeating is enabled or not.
  CORBA::Boolean heartbeat_enabled_value_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/FaultTolerance/FT_ServerPolicy_i.inl"
#endif /* __ACE_INLINE__ */


#include /**/ "ace/post.h"
#endif /* TAO_FT_SERVERPOLICY_I_H */

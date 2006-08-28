// -*- C++ -*-

//=============================================================================
/**
 *  @file   Network_Priority_Policy.h
 *
 *  $Id$
 *
 *  
 */
//=============================================================================


#ifndef TAO_NETWORK_PRIORITY_POLICY_H
#define TAO_NETWORK_PRIORITY_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Network_Priority_PolicyC.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/SString.h"
#include "tao/Basic_Types.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Network_Priority_Policy
 *
 * @brief Implementation class for TAO-specific Network Priority Policy.
 *
 * This policy is used to control the network priority assigned to the
 * requests and replies, processed by the ORB.
 */
class TAO_Export TAO_Network_Priority_Policy
  : public TAO::NetworkPriorityPolicy,
    public TAO_Local_RefCounted_Object
{
public:

  /// default constructor.
  TAO_Network_Priority_Policy (void);

  /// Constructor.
  TAO_Network_Priority_Policy (
     const TAO::DiffservCodepoint &request_diffserv_codepoint,
     const TAO::DiffservCodepoint &reply_diffserv_codepoint,
     TAO::NetworkPriorityModel &network_priority_model);

  /// Copy constructor.
  TAO_Network_Priority_Policy (
     const TAO_Network_Priority_Policy &rhs);

  /// Returns a copy of <this>.
  TAO_Network_Priority_Policy *clone (void) const;

  // = The TAO::Network_Priority_Policy methods
  //

  void set_network_priority_model (::TAO::NetworkPriorityModel npm
                    ACE_ENV_ARG_DECL)
                    ACE_THROW_SPEC ((::CORBA::SystemException));

  void set_request_diffserv_codepoint (::TAO::DiffservCodepoint dscp
                    ACE_ENV_ARG_DECL)
                    ACE_THROW_SPEC ((::CORBA::SystemException));

  void set_reply_diffserv_codepoint (::TAO::DiffservCodepoint dscp
                ACE_ENV_ARG_DECL)
                ACE_THROW_SPEC ((::CORBA::SystemException));

  ::TAO::DiffservCodepoint get_request_diffserv_codepoint (
           ACE_ENV_SINGLE_ARG_DECL)
           ACE_THROW_SPEC ((::CORBA::SystemException));

  ::TAO::DiffservCodepoint get_reply_diffserv_codepoint (
           ACE_ENV_SINGLE_ARG_DECL)
           ACE_THROW_SPEC ((::CORBA::SystemException));

  TAO::DiffservCodepoint request_diffserv_codepoint (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  TAO::DiffservCodepoint reply_diffserv_codepoint (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  TAO::NetworkPriorityModel network_priority_model (
    ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  TAO::NetworkPriorityModel priority_model (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// This method writes a CDR representation of the object state.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  virtual ~TAO_Network_Priority_Policy (void);

private:
  /// The attributes
  TAO::DiffservCodepoint request_diffserv_codepoint_;
  TAO::DiffservCodepoint reply_diffserv_codepoint_;
  TAO::NetworkPriorityModel network_priority_model_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_NETWORK_PRIORITY_POLICY_H */

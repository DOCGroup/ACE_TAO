// -*- C++ -*-

//=============================================================================
/**
 *  @file   Server_Network_Priority_Policy.h
 *
 *  $Id$
 *
 *
 */
//=============================================================================


#ifndef TAO_SERVER_NETWORK_PRIORITY_POLICY_H
#define TAO_SERVER_NETWORK_PRIORITY_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/DiffServPolicy/DiffServPolicy_Export.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/Basic_Types.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Server_Network_Priority_Policy
 *
 * @brief Implementation class for TAO-specific Network Priority Policy.
 *
 * This policy is defined by a server application to specify the
 * network priority model that it would like to follow, as well as
 * the request DiffServ codepoint and reply DiffServ codepoint
 * corresponding to the network priority model.
 */
class TAO_DiffServPolicy_Export TAO_Server_Network_Priority_Policy
  : public TAO::NetworkPriorityPolicy,
    public TAO_Local_RefCounted_Object
{
public:

  /// default constructor.
  TAO_Server_Network_Priority_Policy (void);

  /// Constructor.
  TAO_Server_Network_Priority_Policy (
     const TAO::DiffservCodepoint &request_diffserv_codepoint,
     const TAO::DiffservCodepoint &reply_diffserv_codepoint,
     TAO::NetworkPriorityModel &network_priority_model);

  /// Copy constructor.
  TAO_Server_Network_Priority_Policy (
     const TAO_Server_Network_Priority_Policy &rhs);

  /// Returns a copy of <this>.
  TAO_Server_Network_Priority_Policy *clone (void) const;

  TAO::DiffservCodepoint request_diffserv_codepoint (void);

  void request_diffserv_codepoint (TAO::DiffservCodepoint req_dscp);

  TAO::DiffservCodepoint reply_diffserv_codepoint (void);

  void reply_diffserv_codepoint (TAO::DiffservCodepoint reply_dscp);

  TAO::NetworkPriorityModel network_priority_model (void);

  void network_priority_model (TAO::NetworkPriorityModel npm);

  static CORBA::Policy_ptr create (const CORBA::Any &val);

  CORBA::PolicyType policy_type (void);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

  // Return the cached policy type for this policy.
  TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// This method writes a CDR representation of the object state.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

protected:

  virtual ~TAO_Server_Network_Priority_Policy (void);

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
#endif /* TAO_SERVER_NETWORK_PRIORITY_POLICY_H */

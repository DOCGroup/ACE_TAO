// -*- C++ -*-

// ===================================================================
/**
 *  @file   DiffServ_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_DIFFSERV_PROTOCOLS_HOOKS_H
#define TAO_DIFFSERVT_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#include "tao/Network_Priority_Protocols_Hooks.h"
#include "ace/Service_Config.h"
#include "tao/DiffServPolicy/DiffServPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DiffServPolicy_Export TAO_DS_Network_Priority_Protocols_Hooks
      : public TAO_Network_Priority_Protocols_Hooks
{
public:

  /// Constructor
  TAO_DS_Network_Priority_Protocols_Hooks (void);

  /// Destructor
  virtual ~TAO_DS_Network_Priority_Protocols_Hooks (void);

  /// Initialize the network priority protocols hooks instance.
  void init_hooks (TAO_ORB_Core *orb_core);

  /// This function is used by the client side ORB to figure out
  /// the DiffServ codepoint that needs to be added to the request
  /// to be sent to the server.
  ///
  CORBA::Long get_dscp_codepoint (TAO_Stub *stub, CORBA::Object *object);

  /// This function is used by the server side to figure out the
  /// DiffServ codepoint that is attached and sent as part of the 
  /// service context of the request from the client side.
  /// Specifically, when CLIENT_PROPAGATED networ priority model
  /// is followed, the clients sent the DiffServ codepoint, they
  /// want the server to use in the reply, as a service context
  /// entry in the request.
  ///
  CORBA::Long get_dscp_codepoint (TAO_Service_Context &sc);

  /// This function is called from the ORB_Core to add
  /// network priority policy specific information on the request
  /// service context. Specifically, when CLIENT_PROPAGATED network
  /// priority model is used, the DiffServ codepoint that needs to be used
  /// by the server side in the reply, is specified as a service context
  /// entry, just like how RT-CORBA's CLIENT_PROPAGATED priority model works.
  ///
  void np_service_context (TAO_Stub *stub,
                           TAO_Service_Context &service_context,
                           CORBA::Boolean restart);

  /// Helper function that is used by the np_service_context () method.
  ///
  void add_rep_np_service_context_hook (
    TAO_Service_Context &service_context,
    CORBA::Long &dscp_codepoint);

protected:

  TAO_ORB_Core *orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_DiffServPolicy,
                               TAO_DS_Network_Priority_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO_DiffServPolicy,
                     TAO_DS_Network_Priority_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_DIFFSERV_PROTOCOLS_HOOKS_H */

// -*- C++ -*-

// ===================================================================
/**
 *  @file   Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
// ===================================================================

#ifndef TAO_PROTOCOLS_HOOKS_H
#define TAO_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/IOP_IORC.h"
#include "ace/SString.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Policy;
  class Environment;
}

class TAO_ORB_Core;
class TAO_Service_Context;
class TAO_Connection_Handler;
class TAO_Stub;

class TAO_Export TAO_IIOP_Protocol_Properties
{
public:

  TAO_IIOP_Protocol_Properties (void);

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  int keep_alive_;
  int dont_route_;
  int no_delay_;
  CORBA::Boolean enable_network_priority_;
};

class TAO_Export TAO_UIOP_Protocol_Properties
{
public:

  TAO_UIOP_Protocol_Properties (void);

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
};

class TAO_Export TAO_SHMIOP_Protocol_Properties
{
public:

  TAO_SHMIOP_Protocol_Properties (void);

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  int keep_alive_;
  int dont_route_;
  int no_delay_;
  CORBA::Long preallocate_buffer_size_;
  ACE_CString mmap_filename_;
  ACE_CString mmap_lockname_;
};

class TAO_Export TAO_DIOP_Protocol_Properties
{
public:

  TAO_DIOP_Protocol_Properties (void);

  CORBA::Boolean enable_network_priority_;
};

class TAO_Export TAO_SCIOP_Protocol_Properties
{
public:

  TAO_SCIOP_Protocol_Properties (void);

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  int keep_alive_;
  int dont_route_;
  int no_delay_;
  CORBA::Boolean enable_network_priority_;
};

class TAO_Export TAO_Protocols_Hooks : public ACE_Service_Object
{
public:
  /// destructor
  virtual ~TAO_Protocols_Hooks (void);

  /*
  //  Used by both the Default_Protocols_Hooks as well as 
  //  RT_Protocols_Hooks
  //
  /// Initialize the protocols hooks instance.
  virtual void init_hooks (TAO_ORB_Core *orb_core
                           ACE_ENV_ARG_DECL) = 0;

  */

  /*
  //
  // Called by Remote_Invocation.cpp to set the client network priority
  // when a request is being sent to the server.
  // Implemented by RT_Protocols_Hooks, and hence only used for
  // RTCORBA way of providing DiffServ network priority.
  // Another thing that could be done, is to existence of RTCORBA
  // client network protocol policy to set this client network priority.
  //
  virtual CORBA::Boolean set_client_network_priority (IOP::ProfileId protocol_tag,
                                                      TAO_Stub *stub
                                                      ACE_ENV_ARG_DECL) = 0;

  */

  /*
  //
  // Called by RT_Servant_Dispatcher when preparing the reply to be 
  // sent back to the client.
  // RT_Protocols_Hooks implements this.
  //
  virtual CORBA::Boolean set_server_network_priority (IOP::ProfileId protocol_tag,
                                                      CORBA::Policy *policy
                                                      ACE_ENV_ARG_DECL) = 0;
  
  */

  /*
  //
  // Used by the Connection Handlers when they are initiaized.
  // only RT_Protocols_Hooks implements these methods.
  // So these can be used by the Connection Handlers only in conjunction
  // with RTCORBA.
  // Protocols_Properties_Protocols_Hooks needs to implement these methods.
  //
  virtual void server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;

  virtual void client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                                        ACE_ENV_ARG_DECL) = 0;
  
  */

  /*
  //
  // Used by ORB_Core to set the DiffServ specific service
  // context entry to carry the reply DiffServ codepoint to be
  // used by the server in the case of the CLIENT_PROPAGATED
  // network priority model.
  // Only the DiffServ_Protocols_Hooks needs to implement this.
  //
  virtual CORBA::Long get_dscp_codepoint (void) = 0;

  virtual void set_dscp_codepoint (CORBA::Long &dscp_codepoint
                                          ACE_ENV_ARG_DECL) = 0;

  virtual void np_service_context (TAO_Stub *stub,
                                   TAO_Service_Context &service_context,
                                   CORBA::Boolean restart
                                   ACE_ENV_ARG_DECL) = 0;

  virtual void add_rep_np_service_context_hook (
    TAO_Service_Context &service_context,
    CORBA::Long &dscp_codepoint
    ACE_ENV_ARG_DECL) = 0;

  */

  /*
  //
  // Used by ORB_Core to add RTCORBA specific service context
  // for the CLIENT_PROPAGATED priority model policy.
  // Only RT_Protocols_Hooks needs to implement this.
  //
  virtual void rt_service_context (TAO_Stub *stub,
                                   TAO_Service_Context &service_context,
                                   CORBA::Boolean restart
                                   ACE_ENV_ARG_DECL) = 0;

  virtual void add_rt_service_context_hook (
    TAO_Service_Context &service_context,
    CORBA::Policy *model_policy,
    CORBA::Short &client_priority
    ACE_ENV_ARG_DECL) = 0;

  virtual void get_selector_hook (CORBA::Policy *model_policy,
                                  CORBA::Boolean
                                  &is_client_propagated,
                                  CORBA::Short &server_priority) = 0;

  virtual void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                               CORBA::Short priority,
                                               CORBA::Short &min_priority,
                                               CORBA::Short &max_priority,
                                               int &in_range) = 0;

  */

  /**
   * @name Accessor and modifier to the current thread priority, used to
   * implement the RTCORBA::Current interface, but it is faster for
   * some critical components.  If the RTCORBA library isn't used,
   * these operations are no-ops.
   */
  //@{

  /*
  //
  // Used by the RTCORBA and RTPortableServer libraries
  // to set the appropriate thread and network priority
  // for the requests/replies.
  // Thread_Protocols_Hooks needs to implement this.
  //
  virtual int get_thread_CORBA_priority (CORBA::Short &
                                         ACE_ENV_ARG_DECL) = 0;

  virtual int get_thread_native_priority (CORBA::Short &
                                          ACE_ENV_ARG_DECL) = 0;

  virtual int get_thread_CORBA_and_native_priority (
    CORBA::Short &,
    CORBA::Short &
    ACE_ENV_ARG_DECL) = 0;

  virtual int set_thread_CORBA_priority (CORBA::Short
                                         ACE_ENV_ARG_DECL) = 0;

  virtual int set_thread_native_priority (CORBA::Short
                                          ACE_ENV_ARG_DECL) = 0;

  */

  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PROTOCOLS_HOOKS_H */

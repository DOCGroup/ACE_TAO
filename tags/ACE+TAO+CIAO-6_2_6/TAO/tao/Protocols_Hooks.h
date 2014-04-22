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

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/IOPC.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Policy;
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
  CORBA::Long hop_limit_;
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

  CORBA::Long send_buffer_size_;
  CORBA::Long recv_buffer_size_;
  CORBA::Boolean enable_network_priority_;
  CORBA::Long hop_limit_;
  CORBA::Boolean enable_multicast_loop_;
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
  CORBA::Long hop_limit_;
};

class TAO_Export TAO_Protocols_Hooks : public ACE_Service_Object
{
public:
  /// Destructor
  virtual ~TAO_Protocols_Hooks (void);

  /// Initialize the protocols hooks instance.
  virtual void init_hooks (TAO_ORB_Core *orb_core) = 0;

  virtual CORBA::Boolean set_client_network_priority (
    IOP::ProfileId protocol_tag,
    TAO_Stub *stub) = 0;

  virtual CORBA::Boolean set_server_network_priority (
    IOP::ProfileId protocol_tag,
    CORBA::Policy *policy) = 0;

  virtual void server_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void client_protocol_properties_at_orb_level (
    TAO_IIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void server_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void client_protocol_properties_at_orb_level (
    TAO_UIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void server_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void client_protocol_properties_at_orb_level (
    TAO_SHMIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void server_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void client_protocol_properties_at_orb_level (
    TAO_DIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void server_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &protocol_properties) = 0;

  virtual void client_protocol_properties_at_orb_level (
    TAO_SCIOP_Protocol_Properties &protocol_properties) = 0;

  virtual CORBA::Long get_dscp_codepoint (void) = 0;

  virtual void get_selector_hook (CORBA::Policy *model_policy,
                                  CORBA::Boolean &is_client_propagated,
                                  CORBA::Short &server_priority) = 0;

  virtual void get_selector_bands_policy_hook (CORBA::Policy *bands_policy,
                                               CORBA::Short priority,
                                               CORBA::Short &min_priority,
                                               CORBA::Short &max_priority,
                                               bool &in_range) = 0;

  /**
   * @name Accessor and modifier to the current thread priority, used to
   * implement the RTCORBA::Current interface, but it is faster for
   * some critical components.  If the RTCORBA library isn't used,
   * these operations are no-ops.
   */
  //@{
  virtual int get_thread_CORBA_priority (CORBA::Short &) = 0;

  virtual int get_thread_native_priority (CORBA::Short &) = 0;

  virtual int get_thread_CORBA_and_native_priority (
    CORBA::Short &,
    CORBA::Short &) = 0;

  virtual int get_thread_implicit_CORBA_priority (CORBA::Short&) = 0;

  virtual int set_thread_CORBA_priority (CORBA::Short) = 0;

  virtual int restore_thread_CORBA_and_native_priority (CORBA::Short,
                                                        CORBA::Short) = 0;
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PROTOCOLS_HOOKS_H */

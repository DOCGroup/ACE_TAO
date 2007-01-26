// -*- C++ -*-

// ===================================================================
/**
 *  @file   Protocols_Properties_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_PROTOCOLS_PROPERTIES_PROTOCOLS_HOOKS_H
#define TAO_PROTOCOLS_PROPERTIES__ROTOCOLS_HOOKS_H

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
  class Environment;
}

class TAO_Connection_Handler;

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

class TAO_Export TAO_Protocols_Properties_Protocols_Hooks
      : public ACE_Service_Object
{
public:
  /// destructor
  virtual ~TAO_Protocols_Properties_Protocols_Hooks (void);

  virtual void init_hooks (TAO_ORB_Core *orb_core
                           ACE_ENV_ARG_DECL) = 0;

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

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PROTOCOLS_PROPERTIES_PROTOCOLS_HOOKS_H */

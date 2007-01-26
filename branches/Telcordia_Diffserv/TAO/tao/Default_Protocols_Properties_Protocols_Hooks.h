// -*- C++ -*-

// ===================================================================
/**
 *  @file   Default_Protocols_Properties_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_DEFAULT_PROTOCOLS_PROPERTIES_PROTOCOLS_HOOKS_H
#define TAO_DEFAULT_PROTOCOLS_PROPERTIES_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Protocols_Properties_Protocols_Hooks.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA;

class TAO_Export TAO_Default_Protocols_Properties_Protocols_Hooks 
      : public TAO_Protocols_Properties_Protocols_Hooks
{
public:

  /// destructor
  virtual ~TAO_Default_Protocols_Properties_Protocols_Hooks (void);

  void init_hooks (TAO_ORB_Core *orb_core
                   ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_IIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_UIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_SHMIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_DIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void server_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);

  void client_protocol_properties_at_orb_level (TAO_SCIOP_Protocol_Properties &protocol_properties
                                                ACE_ENV_ARG_DECL);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, 
                               TAO_Default_Protocols_Properties_Protocols_Hooks)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Protocols_Properties_Protocols_Hooks)

#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_PROTOCOLS_PROPERTIES_PROTOCOLS_HOOKS_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file    Remote_Object_Proxy_Broker.h
 *
 *  $Id$
 *
 *  This files contains the Remote Proxy Broker implementation
 *  for the CORBA Object.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Balachandran Natarajan (modified the implementation)
 */
//=============================================================================

#ifndef TAO_REMOTE_OBJECT_PROXY_BROKER_H
#define TAO_REMOTE_OBJECT_PROXY_BROKER_H

#include /**/ "ace/pre.h"

#include "tao/Object_Proxy_Broker.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Remote_Object_Proxy_Broker
   *
   * @brief Remote_Object_Proxy_Broker
   *
   * A broker for standard CORBA::Object calls that needs to be made
   * on remote objects.
   */

  class TAO_Export Remote_Object_Proxy_Broker : public Object_Proxy_Broker
  {
  public:
    /// Please see the documentation in Object_Proxy_Broker.h for
    /// details.

    virtual CORBA::Boolean _is_a (CORBA::Object_ptr target,
                                  const char *logical_type_id);

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (CORBA::Object_ptr target);

  virtual CORBA::InterfaceDef * _get_interface (CORBA::Object_ptr target);

  virtual CORBA::Object_ptr _get_component (CORBA::Object_ptr target);

  virtual char * _repository_id (CORBA::Object_ptr target);
#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

  };
}

// -----------------------------------------------------

TAO::Remote_Object_Proxy_Broker *the_tao_remote_object_proxy_broker (void);

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_REMOTE_OBJECT_PROXY_BROKER_H */

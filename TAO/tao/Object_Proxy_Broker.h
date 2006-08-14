// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_Proxy_Broker.h
 *
 *  $Id$
 *
 *  This files contains the abstract class for the CORBA Object
 *  proxy brokers.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  Modified by
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_PROXY_BROKER_H
#define TAO_OBJECT_PROXY_BROKER_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class InterfaceDef;

  class Object;
  typedef Object *Object_ptr;

  class Environment;
}

namespace TAO
{
  /**
   * @class Object_Proxy_Broker
   *
   * @brief Object_Proxy_Broker
   *
   */
  class TAO_Export Object_Proxy_Broker
  {
  public:

    /// Destructor
    virtual ~Object_Proxy_Broker (void);

    virtual CORBA::Boolean _is_a (CORBA::Object_ptr target,
                                  const char *logical_type_id
                                  ACE_ENV_ARG_DECL) = 0;

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (CORBA::Object_ptr target
                                        ACE_ENV_ARG_DECL) = 0;

  virtual CORBA::InterfaceDef *_get_interface (
      CORBA::Object_ptr target
      ACE_ENV_ARG_DECL
    ) = 0;

  virtual CORBA::Object_ptr _get_component (CORBA::Object_ptr target
                                            ACE_ENV_ARG_DECL) = 0;

  virtual char * _repository_id (CORBA::Object_ptr target
                                 ACE_ENV_ARG_DECL) = 0;

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_PROXY_BROKER_H_ */

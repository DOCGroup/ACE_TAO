// -*- C++ -*-

//=============================================================================
/**
 *  @file    Remote_Object_Proxy_Impl.h
 *
 *  $Id$
 *
 *  This files contains the definition of the remote proxy
 *  for the CORBA::Object class.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_REMOTE_OBJECT_PROXY_IMPL_H_
#define TAO_REMOTE_OBJECT_PROXY_IMPL_H_

#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#include "tao/Object_Proxy_Impl.h"

/**
 * @class TAO_Remote_Object_Proxy_Impl
 *
 * @brief TAO_Remote_Object_Proxy_Impl
 *
 * This class implements the remote proxy for the CORBA::Object class.
 */
class TAO_Export TAO_Remote_Object_Proxy_Impl
  : public virtual TAO_Object_Proxy_Impl
{
public:

  virtual CORBA::Boolean _is_a (const CORBA::Object_ptr target,
                                const char * logical_type_id
                                ACE_ENV_ARG_DECL);

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (const CORBA::Object_ptr target
                                        ACE_ENV_ARG_DECL);

  virtual CORBA::InterfaceDef_ptr _get_interface (
      const CORBA::Object_ptr target
      ACE_ENV_ARG_DECL
    );

  virtual CORBA::Object_ptr _get_component (const CORBA::Object_ptr target
                                            ACE_ENV_ARG_DECL);

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

};


#include /**/ "ace/post.h"

#endif /* TAO_REMOTE_OBJECT_PROXY_IMPL */

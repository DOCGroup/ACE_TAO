// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_Proxy_Impl.h
 *
 *  $Id$
 *
 *  This files contains the proxy definition of the interface that
 *  all the Object proxy have to imlements.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_PROXY_IMPL_H_
#define TAO_OBJECT_PROXY_IMPL_H_

#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"

namespace CORBA
{
  class InterfaceDef;
  typedef InterfaceDef *InterfaceDef_ptr;
};

/**
 * @class TAO_Object_Proxy_Impl
 *
 * @brief TAO_Object_Proxy_Impl
 *
 * This class defines the interface that the proxy for the class
 * CORBA::Object have to implement.
 */
class TAO_Export TAO_Object_Proxy_Impl
{
public:

  virtual ~TAO_Object_Proxy_Impl (void);

  virtual CORBA::Boolean _is_a (const CORBA::Object_ptr target,
                                const char *logical_type_id
                                ACE_ENV_ARG_DECL) = 0;

#if (TAO_HAS_MINIMUM_CORBA == 0)

  virtual CORBA::Boolean _non_existent (const CORBA::Object_ptr target
                                        ACE_ENV_ARG_DECL) = 0;

  virtual CORBA::InterfaceDef_ptr _get_interface (
      const CORBA::Object_ptr target
      ACE_ENV_ARG_DECL
    ) = 0;

  virtual CORBA::Object_ptr _get_component (const CORBA::Object_ptr target
                                            ACE_ENV_ARG_DECL) = 0;

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

};

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_PROXY_IMPL_H_ */

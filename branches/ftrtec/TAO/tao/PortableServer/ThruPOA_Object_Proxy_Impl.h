// -*- C++ -*-

//=============================================================================
/**
 *  @file    ThruPOA_Object_Proxy_Impl.h
 *
 *  $Id$
 *
 *  This files contains the definition of the ThruPOA proxy
 *  for the CORBA::Object class.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_THRUPOA_OBJECT_PROXY_IMPL_H_
#define TAO_THRUPOA_OBJECT_PROXY_IMPL_H_

// -- ACE Include --
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

// -- TAO Include --
#include "tao/Object_Proxy_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_ThruPOA_Object_Proxy_Impl
 *
 * @brief TAO_ThruPOA_Object_Proxy_Impl
 *
 * This class implements the ThruPOA proxy for the CORBA::Object class.
 */
class TAO_PortableServer_Export TAO_ThruPOA_Object_Proxy_Impl 
  : public virtual TAO_Object_Proxy_Impl
{
public:

  TAO_ThruPOA_Object_Proxy_Impl (void);

  virtual ~TAO_ThruPOA_Object_Proxy_Impl (void);

  virtual CORBA::Boolean _is_a (const CORBA::Object_ptr target,
                                const CORBA::Char *logical_type_id
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

#endif /* TAO_THRUPOA_OBJECT_PROXY_IMPL_H_ */

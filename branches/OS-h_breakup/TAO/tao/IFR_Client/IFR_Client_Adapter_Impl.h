// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IFR_Client_Adapter_Impl.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_CLIENT_ADAPTER_IMPL_H
#define TAO_IFR_CLIENT_ADAPTER_IMPL_H
#include "ace/pre.h"

#include "tao/IFR_Client_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ifr_client_export.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_IFR_Client_Adapter_Impl
 *
 * @brief TAO_IFR_Client_Adapter_Impl.
 *
 * Class that adapts various functions in the CORBA namespace
 * which use the Interface Repository. This is the derived class
 * that contains the actual implementations.
 */
class TAO_IFR_Client_Export TAO_IFR_Client_Adapter_Impl
  : public TAO_IFR_Client_Adapter
{
public:
  virtual ~TAO_IFR_Client_Adapter_Impl (void);

  virtual CORBA::Boolean interfacedef_cdr_insert (
      TAO_OutputCDR &cdr,
      CORBA_InterfaceDef_ptr object_type
    );

  virtual void interfacedef_any_insert (
      CORBA_Any &any,
      CORBA_InterfaceDef_ptr object_type
    );

  virtual void dispose (
      CORBA_InterfaceDef_ptr orphan
    );

  virtual CORBA_InterfaceDef_ptr get_interface (
      CORBA::ORB_ptr orb,
      const char *repo_id
      ACE_ENV_ARG_DECL
    );

  virtual CORBA_InterfaceDef_ptr get_interface_remote (
      const CORBA::Object_ptr target
      ACE_ENV_ARG_DECL
    );

  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_IFR_Client_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_IFR_Client, TAO_IFR_Client_Adapter_Impl)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_IFR_Client_Initializer =
  &TAO_IFR_Client_Adapter_Impl::Initializer;

#else

static int
TAO_Requires_IFR_Client_Initializer =
  TAO_IFR_Client_Adapter_Impl::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include "ace/post.h"
#endif /* TAO_IFR_CLIENT_ADAPTER_IMPL_H */

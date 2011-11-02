// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/IFR_Client/ifr_client_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IFR_Client_Adapter.h"

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
      CORBA::InterfaceDef_ptr object_type);

  virtual void interfacedef_any_insert (
      CORBA::Any *any,
      CORBA::InterfaceDef_ptr object_type);

  virtual void dispose (CORBA::InterfaceDef_ptr orphan);

  virtual CORBA::InterfaceDef_ptr get_interface (
      CORBA::ORB_ptr orb,
      const char *repo_id);

  virtual CORBA::InterfaceDef_ptr get_interface_remote (
      CORBA::Object_ptr target);

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  virtual void create_operation_list (
      CORBA::ORB_ptr orb,
      CORBA::OperationDef_ptr,
      CORBA::NVList_ptr&);
#endif /*TAO_HAS_MINIMUM_CORBA*/

  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_IFR_Client_Initializer =
  TAO_IFR_Client_Adapter_Impl::Initializer ();


ACE_STATIC_SVC_DECLARE (TAO_IFR_Client_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_IFR_Client, TAO_IFR_Client_Adapter_Impl)
TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_IFR_CLIENT_ADAPTER_IMPL_H */

// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IFR_Client_Adapter.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_CLIENT_ADAPTER_H
#define TAO_IFR_CLIENT_ADAPTER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

/**
 * @class TAO_IFR_Client_Adapter
 *
 * @brief TAO_IFR_Client_Adapter.
 *
 * Class that adapts various functions in the CORBA namespace
 * which use the Interface Repository. This is a base class for
 * the actual implementation in the TAO_IFR_Client library.
 */
class TAO_Export TAO_IFR_Client_Adapter : public ACE_Service_Object
{
public:
  virtual ~TAO_IFR_Client_Adapter (void);

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
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  virtual CORBA_InterfaceDef_ptr get_interface_remote (
      const CORBA::Object_ptr target
      ACE_ENV_ARG_DECL
    );
};

#include "ace/post.h"
#endif /* TAO_IFR_CLIENT_ADAPTER_H */

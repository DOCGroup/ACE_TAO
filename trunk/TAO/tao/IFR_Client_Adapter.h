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

class IR_InterfaceDef;

/**
 * @class TAO_IFR_Client_Adapter
 *
 * @brief TAO_IFR_Client_Adapter.
 *
 * Class that adapts various functions in the CORBA namespace
 * which use the Interface Repository. This is a base class for 
 * the actual implementation in the IFR_Basic library.
 */
class TAO_Export TAO_IFR_Client_Adapter : public ACE_Service_Object
{
public:
  virtual ~TAO_IFR_Client_Adapter (void);

  virtual CORBA::Boolean interfacedef_cdr_insert (
      TAO_OutputCDR &cdr,
      IR_InterfaceDef *object_type
    );

  virtual void interfacedef_any_insert (
      CORBA_Any &any,
      IR_InterfaceDef *object_type
    );

  virtual IR_InterfaceDef *get_interface (
      CORBA::ORB_ptr orb,
      const char *repo_id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );

  virtual CORBA_IRObject_ptr get_interface_def (
      CORBA::ORB_ptr orb,
      const char *repo_id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );
};

#include "ace/post.h"
#endif /* TAO_IFR_CLIENT_ADAPTER_H */

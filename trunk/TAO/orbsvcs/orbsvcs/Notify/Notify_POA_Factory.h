/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_POA_Factory.h
 *
 *  $Id$
 *
 * Factory interface for POA objects.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_POA_FACTORY_H
#define TAO_NOTIFY_POA_FACTORY_H

#include "ace/pre.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "notify_export.h"

 /**
  * @class TAO_Notify_POA_Factory
  *
  * @brief TAO_Notify_POA_Factory
  *
  * Interface for POA objects.
  */
class TAO_Notify_Export TAO_Notify_POA_Factory : public ACE_Service_Object
{
 public:
  // = POA creation methods.
  /// Create the POA to activate Event Channels in.
  virtual PortableServer::POA_ptr create_event_channel_POA (PortableServer::POA_ptr parent_poa ACE_ENV_ARG_DECL) = 0;

  /// Create the POA to activate SA's in.
  virtual PortableServer::POA_ptr create_supplier_admin_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL) = 0;

  /// Create the POA to activate CA's in.
  virtual PortableServer::POA_ptr create_consumer_admin_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL) = 0;

  /// Create the POA to activate proxy push consumers in.
  virtual PortableServer::POA_ptr create_proxy_pushconsumer_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL) = 0;

  /// Create the POA to activate proxy push suppliers in.
  virtual PortableServer::POA_ptr create_proxy_pushsupplier_POA (PortableServer::POA_ptr parent_poa, CORBA::Long new_poa_id ACE_ENV_ARG_DECL) = 0;

  // = Object activation and POA methods
  /// Converts <id> to an ObjectId and registers with the POA <poa>.
  virtual CORBA::Object_ptr activate_object_with_id (CORBA::Long id, PortableServer::POA_ptr poa,PortableServer::Servant servant ACE_ENV_ARG_DECL) = 0;

  /// Registers with the POA <poa>.
  virtual CORBA::Object_ptr activate_object (PortableServer::POA_ptr poa,PortableServer::Servant servant ACE_ENV_ARG_DECL) = 0;

  /// Registers with the POA <poa>.
  virtual CORBA::Object_ptr servant_to_reference (PortableServer::POA_ptr poa,PortableServer::Servant servant ACE_ENV_ARG_DECL) = 0;

  /// Look for the Object with id <id> in <poa>.
  virtual CORBA::Object_ptr id_to_reference (CORBA::Long id, PortableServer::POA_ptr poa ACE_ENV_ARG_DECL) = 0;

  /// Destroy the <poa>
  virtual void destroy_POA (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL) = 0;

  /// Deactivate the object.
  virtual void deactivate_object (CORBA::Object_ptr obj, PortableServer::POA_ptr poa ACE_ENV_ARG_DECL) = 0;

  /// Deactivate the object.
  virtual void deactivate_object (PortableServer::Servant servant, PortableServer::POA_ptr ACE_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_POA_FACTORY_H */

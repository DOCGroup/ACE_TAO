/* -*- C++ -*- */
/**
 *  @file LookupManager.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_LOOKUPMANAGER_H
#define TAO_Notify_Tests_LOOKUPMANAGER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Hash_Map_Manager.h"

class TAO_Notify_Tests_Periodic_Supplier;
class TAO_Notify_Tests_Periodic_Consumer;
class TAO_Notify_Tests_Driver_Base;
class TAO_Notify_Tests_Activation_Manager;
class TAO_Notify_Tests_Priority_Mapping;

/**
 * @class TAO_Notify_Tests_LookupManager
 *
 * @brief Utility to register and resolve object references.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_LookupManager
{
public:
  /// Constructor
  TAO_Notify_Tests_LookupManager (void);

  /// Destructor
  ~TAO_Notify_Tests_LookupManager ();

  /// Init
  void init (CORBA::ORB_ptr orb);

  ///= Register Objects

  /// Register the application starter object.
  void _register (TAO_Notify_Tests_Driver_Base* app);
  void _register (TAO_Notify_Tests_Activation_Manager* activation_manager);
  void _register (TAO_Notify_Tests_Priority_Mapping *priority_mapping);

  /// Register Objects with Naming Service
  void _register(CORBA::Object_ptr obj, const char* obj_name);

  ///= Resolve methods

  /// Resolve the application starter object.
  void resolve (TAO_Notify_Tests_Driver_Base*& app);
  void resolve (TAO_Notify_Tests_Activation_Manager*& activation_manager);
  void resolve (TAO_Notify_Tests_Priority_Mapping* &priority_mapping);

  /// Return the orb
  void resolve (CORBA::ORB_var& orb);

  /// Return the Root POA.
  void resolve (PortableServer::POA_var& poa);

  /// Return the default Naming context.
  void resolve (CosNaming::NamingContextExt_var& naming);

  void resolve (PortableServer::POA_var& poa, const char *poa_name);
  void resolve (CosNaming::NamingContextExt_var& naming, const char *naming_name);
  void resolve (CosNotifyChannelAdmin::EventChannelFactory_var& ecf, const char *factory_name);
  void resolve (CosNotifyChannelAdmin::EventChannel_var& ec, const char *channel_name);
  void resolve (CosNotifyChannelAdmin::SupplierAdmin_var& sa, const char *admin_name);
  void resolve (CosNotifyChannelAdmin::ConsumerAdmin_var& ca , const char *admin_name);
  void resolve (CosNotifyComm::StructuredPushSupplier_var& supplier, const char *supplier_name);
  void resolve (CosNotifyComm::StructuredPushConsumer_var& consumer, const char *consumer_name);
  void resolve (CosNotifyFilter::FilterFactory_var& ff, const char *filter_factory_name);
  void resolve (CosNotifyFilter::Filter_var& filter, const char *filter_name);
  void resolve (CosNotifyFilter::FilterAdmin_var& filter_admin, const char *filter_admin_name);

protected:
  /// Application Starter
  TAO_Notify_Tests_Driver_Base* app_;

  /// Activation Manager
  TAO_Notify_Tests_Activation_Manager* activation_manager_;

  /// Resolve to CORBA::Object
  CORBA::Object_ptr resolve_object (const char* obj_name);

  // The ORB that we use.
  CORBA::ORB_var orb_;

  // Reference to the root poa.
  PortableServer::POA_var root_poa_;

  // A naming context.
  CosNaming::NamingContextExt_var naming_;

  // Priority Mapping.
  TAO_Notify_Tests_Priority_Mapping *priority_mapping_;
};

TAO_NOTIFY_TEST_SINGLETON_DECLARE (ACE_Singleton, TAO_Notify_Tests_LookupManager, TAO_SYNCH_MUTEX)

typedef ACE_Singleton<TAO_Notify_Tests_LookupManager, TAO_SYNCH_MUTEX> _TAO_Notify_Tests_LookupManager;

#define LOOKUP_MANAGER  _TAO_Notify_Tests_LookupManager::instance()

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_LOOKUPMANAGER_H */

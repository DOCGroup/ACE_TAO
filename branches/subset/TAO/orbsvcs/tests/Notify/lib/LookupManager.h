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

#ifndef TAO_NS_LOOKUPMANAGER_H
#define TAO_NS_LOOKUPMANAGER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Hash_Map_Manager.h"

class TAO_NS_Periodic_Supplier;
class TAO_NS_Periodic_Consumer;
class TAO_NS_Driver_Base;
class TAO_NS_Activation_Manager;
class TAO_NS_Priority_Mapping;

/**
 * @class TAO_NS_LookupManager
 *
 * @brief Utility to register and resolve object references.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_LookupManager
{
public:
  /// Constuctor
  TAO_NS_LookupManager (void);

  /// Destructor
  ~TAO_NS_LookupManager ();

  /// Init
  void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  ///= Register Objects

  /// Register the application starter object.
  void _register (TAO_NS_Driver_Base* app);
  void _register (TAO_NS_Activation_Manager* activation_manager);
  void _register (TAO_NS_Priority_Mapping *priority_mapping);

  /// Register Objects with Naming Service
  void _register(CORBA::Object_ptr obj, const char* obj_name ACE_ENV_ARG_DECL);

  ///= Resolve methods

  /// Resolve the application starter object.
  void resolve (TAO_NS_Driver_Base*& app);
  void resolve (TAO_NS_Activation_Manager*& activation_manager);
  void resolve (TAO_NS_Priority_Mapping* &priority_mapping);

  /// Return the orb
  void resolve (CORBA::ORB_var& orb);

  /// Return the Root POA.
  void resolve (PortableServer::POA_var& poa);

  /// Return the default Naming context.
  void resolve (CosNaming::NamingContextExt_var& naming);

  void resolve (PortableServer::POA_var& poa, const char *poa_name ACE_ENV_ARG_DECL);
  void resolve (CosNaming::NamingContextExt_var& naming, const char *naming_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyChannelAdmin::EventChannelFactory_var& ecf, const char *factory_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyChannelAdmin::EventChannel_var& ec, const char *channel_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyChannelAdmin::SupplierAdmin_var& sa, const char *admin_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyChannelAdmin::ConsumerAdmin_var& ca , const char *admin_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyChannelAdmin::StructuredProxyPushSupplier_var& proxy, const char *proxy_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyChannelAdmin::StructuredProxyPushConsumer_var& proxy, const char *proxy_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyFilter::FilterFactory_var& ff, const char *filter_factory_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyFilter::Filter_var& filter, const char *filter_name ACE_ENV_ARG_DECL);
  void resolve (CosNotifyFilter::FilterAdmin_var& filter_admin, const char *filter_admin_name ACE_ENV_ARG_DECL);

protected:
  /// Application Starter
  TAO_NS_Driver_Base* app_;

  /// Activation Manager
  TAO_NS_Activation_Manager* activation_manager_;

  /// Resolve to CORBA::Object
  CORBA::Object_ptr resolve_object (const char* obj_name ACE_ENV_ARG_DECL);

  // The ORB that we use.
  CORBA::ORB_var orb_;

  // Reference to the root poa.
  PortableServer::POA_var root_poa_;

  // A naming context.
  CosNaming::NamingContextExt_var naming_;

  // Priority Mapping.
  TAO_NS_Priority_Mapping *priority_mapping_;
};

typedef ACE_Singleton<TAO_NS_LookupManager, TAO_SYNCH_MUTEX> _TAO_NS_LookupManager;

TAO_NOTIFY_TEST_SINGLETON_DECLARE (ACE_Singleton, TAO_NS_LookupManager, TAO_SYNCH_MUTEX);

#define LOOKUP_MANAGER  _TAO_NS_LookupManager::instance()

#include "ace/post.h"
#endif /* TAO_NS_LOOKUPMANAGER_H */

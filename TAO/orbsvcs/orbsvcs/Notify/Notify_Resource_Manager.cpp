// $Id$
#include "Notify_Resource_Manager.h"
#include "Notify_EventChannelFactory_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_FilterFactory_i.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_SequenceProxyPushSupplier_i.h"
#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_StructuredProxyPushConsumer_i.h"
#include "Notify_SequenceProxyPushConsumer_i.h"
#include "Notify_ProxyPushConsumer_i.h"

// @@ Pradeep: what happened to the RCSID macro?

TAO_Notify_Resource_Manager::TAO_Notify_Resource_Manager (PortableServer::POA_ptr default_POA)
  :default_POA_ (PortableServer::POA::_duplicate (default_POA))
{
  // @@ Pradeep: this is a perfectly useless comment, don't you think?
  // No-Op.
}

TAO_Notify_Resource_Manager::~TAO_Notify_Resource_Manager ()
{
  // @@ Pradeep: this is a perfectly useless comment, don't you think?
  // No-Op.
}

TAO_Notify_Resource_Manager*
TAO_Notify_Resource_Manager::create (PortableServer::POA_ptr default_POA, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Resource_Manager* mgr;
  ACE_NEW_THROW_EX (mgr,
                    TAO_Notify_Resource_Manager (default_POA),
                    CORBA::NO_MEMORY ());
  return mgr;
}

TAO_Notify_EventChannel_i*
TAO_Notify_Resource_Manager::create_event_channel (CosNotifyChannelAdmin::EventChannelFactory_ptr parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventChannel_i* channel;
  ACE_NEW_THROW_EX (channel,
                    TAO_Notify_EventChannel_i (parent, this),
                    CORBA::NO_MEMORY ());
  return channel;
}

TAO_Notify_ConsumerAdmin_i*
TAO_Notify_Resource_Manager::create_consumer_admin (TAO_Notify_EventChannel_i* event_channel,
                                               CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_ConsumerAdmin_i* consumer_admin_i;
  ACE_NEW_THROW_EX (consumer_admin_i,
                    TAO_Notify_ConsumerAdmin_i (event_channel, this),
                    CORBA::NO_MEMORY ());
  return consumer_admin_i;
}

TAO_Notify_SupplierAdmin_i*
TAO_Notify_Resource_Manager::create_supplier_admin (TAO_Notify_EventChannel_i* event_channel,
                                                   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_SupplierAdmin_i* supplier_admin_i;

  ACE_NEW_THROW_EX (supplier_admin_i,
                    TAO_Notify_SupplierAdmin_i (event_channel, this),
                    CORBA::NO_MEMORY ());
  return supplier_admin_i;
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::get_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

TAO_Notify_FilterFactory_i*
TAO_Notify_Resource_Manager::create_filter_factory (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_FilterFactory_i* filterfactory;
  ACE_NEW_THROW_EX (filterfactory,
                    TAO_Notify_FilterFactory_i (),
                    CORBA::NO_MEMORY ());
  return filterfactory;
}

TAO_Notify_Event_Manager*
TAO_Notify_Resource_Manager::create_event_manager (TAO_Notify_EventChannel_i* channel, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Event_Manager* event_manager;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (channel),
                    CORBA::NO_MEMORY ());
  return event_manager;
}

TAO_Notify_StructuredProxyPushSupplier_i*
TAO_Notify_Resource_Manager::create_struct_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_StructuredProxyPushSupplier_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_StructuredProxyPushSupplier_i (parent, this),
                    CORBA::NO_MEMORY ());
  return proxy;
}

TAO_Notify_SequenceProxyPushSupplier_i*
TAO_Notify_Resource_Manager::create_seq_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_SequenceProxyPushSupplier_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_SequenceProxyPushSupplier_i (parent, this),
                    CORBA::NO_MEMORY ());
  return proxy;
}

TAO_Notify_ProxyPushSupplier_i*
TAO_Notify_Resource_Manager::create_proxy_pushsupplier (TAO_Notify_ConsumerAdmin_i* parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_ProxyPushSupplier_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_ProxyPushSupplier_i (parent, this),
                    CORBA::NO_MEMORY ());
  return proxy;
}

TAO_Notify_StructuredProxyPushConsumer_i*
TAO_Notify_Resource_Manager::create_struct_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_StructuredProxyPushConsumer_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_StructuredProxyPushConsumer_i (parent, this),
                    CORBA::NO_MEMORY ());
  return proxy;
}

TAO_Notify_SequenceProxyPushConsumer_i*
TAO_Notify_Resource_Manager::create_seq_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_SequenceProxyPushConsumer_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_SequenceProxyPushConsumer_i (parent, this),
                    CORBA::NO_MEMORY ());
  return proxy;
}

TAO_Notify_ProxyPushConsumer_i*
TAO_Notify_Resource_Manager::create_proxy_pushconsumer (TAO_Notify_SupplierAdmin_i* parent, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_ProxyPushConsumer_i* proxy;
  ACE_NEW_THROW_EX (proxy,
                    TAO_Notify_ProxyPushConsumer_i (parent, this),
                    CORBA::NO_MEMORY ());
  return proxy;
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::create_event_channel_POA (PortableServer::POA_ptr parent_poa, CORBA::Environment &ACE_TRY_ENV)
{
  return this->create_generic_childPOA_i (parent_poa, ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::create_supplier_admin_POA (PortableServer::POA_ptr parent_poa, CORBA::Environment &ACE_TRY_ENV)
{
  return this->create_generic_childPOA_i (parent_poa, ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::create_consumer_admin_POA (PortableServer::POA_ptr parent_poa, CORBA::Environment &ACE_TRY_ENV)
{
  return this->create_generic_childPOA_i (parent_poa, ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::create_proxy_pushconsumer_POA (PortableServer::POA_ptr parent_poa, CORBA::Environment &ACE_TRY_ENV)
{
  return this->create_generic_childPOA_i (parent_poa, ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::create_proxy_pushsupplier_POA (PortableServer::POA_ptr parent_poa, CORBA::Environment &ACE_TRY_ENV)
{
  return this->create_generic_childPOA_i (parent_poa, ACE_TRY_ENV);
}

PortableServer::POA_ptr
TAO_Notify_Resource_Manager::create_generic_childPOA_i (PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Pradeep: if the Notification service is ever going to be
  // persistent or fault tolerant you may need to create this stuff
  // with the persistent policy too, probably you can handle that
  // using a different 'Resource_Manager' that overrides this
  // method. Just a thought...

  // Create a UNIQUE_ID and USER_ID policy because we want the POA
  // to detect duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil());

  PortableServer::IdAssignmentPolicy_var assignpolicy =
    poa->create_id_assignment_policy (PortableServer::USER_ID,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  PortableServer::POAManager_ptr manager =
    poa->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (2);
  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy.in ());
  policy_list [1] =
    PortableServer::IdAssignmentPolicy::_duplicate (assignpolicy.in ());

  // @@ Pradeep: is it possible to use a more meaningful name?
  char child_poa_name[BUFSIZ];

  CORBA::Long poa_id = this->poa_ids_.get ();

  ACE_OS::sprintf (child_poa_name, "%d", poa_id);

  // Create the child POA.
  PortableServer::POA_var poa_ret = poa->create_POA (child_poa_name,
                                                     manager,
                                                     policy_list,
                                                     ACE_TRY_ENV);
  // @@ Pradeep: these guys take an ACE_TRY_ENV argument, and can
  // raise exceptions.  WOuld you believe that?
  idpolicy->destroy ();
  assignpolicy->destroy ();

  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

#if 0
  ACE_DEBUG ((LM_DEBUG, "created child poa %s", child_poa_name));
#endif
  this->poa_ids_.next (); // commit this id to the active list

  return poa_ret._retn ();
}

PortableServer::ObjectId *
TAO_Notify_Resource_Manager::long_to_ObjectId (const CORBA::Long id)
{
  // Modified code from string_to_ObjectId ..
  //

  CORBA::ULong buffer_size = 4;

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

  // Copy the contents
  ACE_OS::memcpy (buffer, (char*)&id, buffer_size);

  // @@ Pradeep: TAO guarantees that Long is 4 bytes wide, but the
  // standard only guarantees that it is at least 4 bytes wide. You
  // may want to think about that....

  // Create and return a new ID
  PortableServer::ObjectId *obj_id = 0;
  ACE_NEW_RETURN (obj_id,
                  PortableServer::ObjectId (buffer_size,
                                            buffer_size,
                                            buffer,
                                            1),
                  0);

  return obj_id;
}

CORBA::Object_ptr
TAO_Notify_Resource_Manager::activate_object_with_id (CORBA::Long id, PortableServer::POA_ptr poa, PortableServer::Servant servant, CORBA::Environment &ACE_TRY_ENV)
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id);

  poa->activate_object_with_id (oid.in (),
                                servant,
                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa->id_to_reference (oid.in (),
                               ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_Notify_Resource_Manager::activate_object (PortableServer::POA_ptr poa, PortableServer::Servant servant, CORBA::Environment &ACE_TRY_ENV)
{
  poa->activate_object (servant, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ObjectId_var oid = poa->servant_to_id (servant, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa->id_to_reference (oid.in (), ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_Notify_Resource_Manager::servant_to_reference (PortableServer::POA_ptr poa, PortableServer::Servant servant, CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var oid = poa->servant_to_id (servant, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return poa->id_to_reference (oid.in (), ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_Notify_Resource_Manager::id_to_reference (CORBA::Long id, PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV)
{
  // Convert CORBA::Long to ObjectId
  PortableServer::ObjectId_var oid =
    this->long_to_ObjectId (id);

  return poa->id_to_reference (oid.in (),
                               ACE_TRY_ENV);
}

void
TAO_Notify_Resource_Manager::destroy_POA (PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV)
{
  if (!CORBA::is_nil (poa))
    poa->destroy (1,1,ACE_TRY_ENV);
}

void
TAO_Notify_Resource_Manager::deactivate_object (CORBA::Object_ptr obj, PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (poa) || CORBA::is_nil (obj))
    return;

  PortableServer::ObjectId_var id =
    poa->reference_to_id (obj, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_Notify_Resource_Manager::deactivate_object (PortableServer::Servant servant, PortableServer::POA_ptr poa, CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (poa) || servant == 0)
    return;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (servant, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::Boolean
TAO_Notify_Resource_Manager::default_subscription_enabled (void)
{
  return 1; // @@ true for now, make this a user option
}

// $Id$

#include "LookupManager.h"
#include "ace/OS_NS_string.h"



#include "Name.h"

TAO_Notify_Tests_LookupManager::TAO_Notify_Tests_LookupManager (void)
  : app_ (0)
    , activation_manager_ (0)
    , priority_mapping_ (0)
{
}

TAO_Notify_Tests_LookupManager::~TAO_Notify_Tests_LookupManager ()
{
}

void
TAO_Notify_Tests_LookupManager::init (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);

  this->resolve (this->root_poa_, TAO_Notify_Tests_Name::root_poa);

  this->resolve (this->naming_, TAO_Notify_Tests_Name::naming_service);
}

void
TAO_Notify_Tests_LookupManager::_register (CORBA::Object_ptr obj, const char* obj_name)
{
  CosNaming::Name_var name =
    this->naming_->to_name (obj_name);
  //@@ Warning: this is rebind..
  this->naming_->rebind (name.in (),
                         obj);

  ACE_DEBUG ((LM_DEBUG, "Registered %s with Naming Service\n", obj_name));
}

void
TAO_Notify_Tests_LookupManager::_register (TAO_Notify_Tests_Driver_Base* app)
{
  this->app_ = app;
}

void
TAO_Notify_Tests_LookupManager::resolve (TAO_Notify_Tests_Driver_Base*& app)
{
  app = this->app_;
}

void
TAO_Notify_Tests_LookupManager::_register (TAO_Notify_Tests_Activation_Manager* activation_manager)
{
  this->activation_manager_ = activation_manager;
}

void
TAO_Notify_Tests_LookupManager::resolve (TAO_Notify_Tests_Activation_Manager*& activation_manager)
{
  activation_manager = this->activation_manager_;
}

void
TAO_Notify_Tests_LookupManager::_register (TAO_Notify_Tests_Priority_Mapping* priority_mapping)
{
  this->priority_mapping_ = priority_mapping;
}

void
TAO_Notify_Tests_LookupManager::resolve (TAO_Notify_Tests_Priority_Mapping* &priority_mapping)
{
  priority_mapping = this->priority_mapping_;
}

void
TAO_Notify_Tests_LookupManager::resolve (CORBA::ORB_var& orb)
{
  orb = this->orb_;
}

void
TAO_Notify_Tests_LookupManager::resolve (PortableServer::POA_var& poa)
{
  poa = this->root_poa_;
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNaming::NamingContextExt_var& naming)
{
  naming = this->naming_;
}

void
TAO_Notify_Tests_LookupManager::resolve (PortableServer::POA_var& poa, const char *poa_name)
{
  if (ACE_OS::strcmp (poa_name, TAO_Notify_Tests_Name::root_poa) == 0)
    {
      CORBA::Object_ptr poa_object  =
                this->orb_->resolve_initial_references(TAO_Notify_Tests_Name::root_poa);

      if (CORBA::is_nil (poa_object))
        {
          ACE_ERROR ((LM_ERROR,
                      " (%P|%t) Unable to resolve the Root POA.\n"));
          return;
        }

      poa = PortableServer::POA::_narrow (poa_object);

          this->root_poa_ = poa;
    }
  else
    {
      poa = root_poa_->find_POA (poa_name, 0);
    }
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNaming::NamingContextExt_var& naming, const char *naming_name)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (naming_name);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_ = CosNaming::NamingContextExt::_narrow (naming_obj.in ());

  CosNaming::NamingContextExt::_duplicate (this->naming_.in ());

  naming = this->naming_;
}

CORBA::Object_ptr
TAO_Notify_Tests_LookupManager::resolve_object (const char* obj_name)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (obj_name);

  CORBA::Object_var obj =
    this->naming_->resolve (name);

  return obj._retn ();
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyChannelAdmin::EventChannelFactory_var& ecf, const char * factory_name)
{
  CORBA::Object_var object = this->resolve_object (factory_name);

  ecf = CosNotifyChannelAdmin::EventChannelFactory::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyChannelAdmin::EventChannel_var& ec, const char * channel_name)
{
  CORBA::Object_var object = this->resolve_object (channel_name);

  ec = CosNotifyChannelAdmin::EventChannel::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyChannelAdmin::SupplierAdmin_var& sa, const char * admin_name)
{
  CORBA::Object_var object = this->resolve_object (admin_name);

  sa = CosNotifyChannelAdmin::SupplierAdmin::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyChannelAdmin::ConsumerAdmin_var& ca , const char * admin_name)
{
  CORBA::Object_var object = this->resolve_object (admin_name);

  ca = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyComm::StructuredPushSupplier_var& supplier, const char *supplier_name)
{
  CORBA::Object_var object = this->resolve_object (supplier_name);

  supplier = CosNotifyComm::StructuredPushSupplier::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyComm::StructuredPushConsumer_var& consumer, const char * consumer_name)
{
  CORBA::Object_var object = this->resolve_object (consumer_name);

  consumer = CosNotifyComm::StructuredPushConsumer::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyFilter::FilterFactory_var& ff, const char *filter_factory_name)
{
  CORBA::Object_var object = this->resolve_object (filter_factory_name);

  ff = CosNotifyFilter::FilterFactory::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyFilter::Filter_var& filter, const char *filter_name)
{
  CORBA::Object_var object = this->resolve_object (filter_name);

  filter = CosNotifyFilter::Filter::_narrow (object.in());
}

void
TAO_Notify_Tests_LookupManager::resolve (CosNotifyFilter::FilterAdmin_var& filter_admin, const char *filter_admin_name)
{
  CORBA::Object_var object = this->resolve_object (filter_admin_name);

  filter_admin = CosNotifyFilter::FilterAdmin::_narrow (object.in());
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<TAO_Notify_Tests_LookupManager, TAO_SYNCH_MUTEX> *ACE_Singleton<TAO_Notify_Tests_LookupManager, TAO_SYNCH_MUTEX>::singleton_;
#endif /*ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

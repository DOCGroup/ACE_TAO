// $Id$

#include "Notify_EventChannel_i.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_FilterFactory_i.h"
#include "Notify_Resource_Manager.h"

TAO_Notify_EventChannel_i::TAO_Notify_EventChannel_i (CosNotifyChannelAdmin::EventChannelFactory_ptr my_factory, TAO_Notify_Resource_Manager* resource_manager)
  :my_factory_ (CosNotifyChannelAdmin::EventChannelFactory::_duplicate (my_factory)),
   resource_manager_(resource_manager),
   default_op_ (CosNotifyChannelAdmin::OR_OP),
   default_id_ (0),
   is_destroyed_ (0),
   max_queue_length_ (0),
   max_consumers_ (0),
   max_suppliers_ (0)
{
  // No-Op.
}

// Implementation skeleton destructor
TAO_Notify_EventChannel_i::~TAO_Notify_EventChannel_i (void)
{
  if (is_destroyed_ == 0)
    this->cleanup_i ();
}

void
TAO_Notify_EventChannel_i::init (const CosNotification::QoSProperties& initial_qos,
                                 const CosNotification::AdminProperties& initial_admin,
                                 PortableServer::POA_ptr my_POA,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->event_manager_ =
    this->resource_manager_->create_event_manager (this, ACE_TRY_ENV);
  ACE_CHECK;

  this->event_manager_->init (ACE_TRY_ENV);
  ACE_CHECK;

  my_POA_ = PortableServer::POA::_duplicate (my_POA);

  // try to set initial qos params
  this->set_qos (initial_qos, ACE_TRY_ENV);
  ACE_CHECK;

  // try to set initial admin params
  this->set_admin (initial_admin, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_TRY
    {
      // Create the POA for the CA's
      this->CA_POA_ =
        this->resource_manager_->create_consumer_admin_POA (this->my_POA_.in (),
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create the POA for the SA's
      this->SA_POA_ =
        this->resource_manager_->create_supplier_admin_POA (this->my_POA_.in (),
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create the default Consumer Admin. Because the ID_Pool is being used
      // the first time here, it will generate the id 0.

      CosNotifyChannelAdmin::AdminID id_unused;
      this->new_for_consumers (default_op_, id_unused, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->new_for_suppliers (default_op_, id_unused, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->default_filter_factory_ =
        this->create_default_filter_factory_i (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      this->cleanup_i (ACE_TRY_ENV);
      ACE_RETHROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_Notify_Event_Manager*
TAO_Notify_EventChannel_i::get_event_manager (void)
{
  return this->event_manager_;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannel_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return CosNotifyChannelAdmin::EventChannel
    ::_narrow (this->resource_manager_->
               servant_to_reference (this->my_POA_.in (), this, ACE_TRY_ENV));
}

void
TAO_Notify_EventChannel_i::cleanup_i (CORBA::Environment &ACE_TRY_ENV)
{
  // Cleanup all resources..
  this->resource_manager_->destroy_POA (this->CA_POA_.in (),
                                        ACE_TRY_ENV);
  this->resource_manager_->destroy_POA (this->SA_POA_.in (),
                                        ACE_TRY_ENV);
  PortableServer::POA_var def_POA =
    this->resource_manager_->get_default_POA ();

  this->resource_manager_->
    deactivate_object(this->default_filter_factory_.in (),
                      def_POA.in (),
                      ACE_TRY_ENV);

  this->CA_POA_ = PortableServer::POA::_nil ();
  this->SA_POA_ = PortableServer::POA::_nil ();
  this->my_POA_ =  PortableServer::POA::_nil ();
  this->my_factory_ = CosNotifyChannelAdmin::EventChannelFactory::_nil ();
  this->default_filter_factory_ = CosNotifyFilter::FilterFactory::_nil ();
}

void
TAO_Notify_EventChannel_i::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->is_destroyed_ = 1;

  // Deactivate ourselves.
  this->resource_manager_->
    deactivate_object (this,
                       this->my_POA_.in (),
                       ACE_TRY_ENV);
  this->cleanup_i (ACE_TRY_ENV);
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannel_i::create_default_filter_factory_i (CORBA::Environment& ACE_TRY_ENV)
{
  TAO_Notify_FilterFactory_i* filterfactory =
    this->resource_manager_->create_filter_factory (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  PortableServer::ServantBase_var filterfactory_var (filterfactory);

  // Init goes here.
  // ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  PortableServer::POA_var def_POA =
    this->resource_manager_->get_default_POA ();

  CORBA::Object_var obj = this->resource_manager_->
    activate_object (def_POA.in (),
                     filterfactory,
                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  return CosNotifyFilter::FilterFactory::_narrow (obj.in ());
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannel_i::MyFactory (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::EventChannelFactory
    ::_duplicate (this->my_factory_.in ());
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::default_consumer_admin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->get_consumeradmin (this->default_id_, ACE_TRY_ENV);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::default_supplier_admin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->get_supplieradmin (this->default_id_, ACE_TRY_ENV);
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannel_i::default_filter_factory (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyFilter::FilterFactory
    ::_duplicate (this->default_filter_factory_.in ());
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_ConsumerAdmin_i* consumeradmin =
    this->resource_manager_->create_consumer_admin (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  PortableServer::ServantBase_var consumeradmin_var (consumeradmin);

  id = this->consumer_admin_ids_.get ();

  consumeradmin->init (id, op, this->CA_POA_.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  CORBA::Object_var obj = this->resource_manager_->
    activate_object_with_id (id,
                             this->CA_POA_.in (),
                             consumeradmin,
                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  this->consumer_admin_ids_.next ();

  return CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op, CosNotifyChannelAdmin::AdminID_out id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_SupplierAdmin_i* supplieradmin =
    this->resource_manager_->create_supplier_admin (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  PortableServer::ServantBase_var supplieradmin_var (supplieradmin);

  id = this->supplier_admin_ids_.get ();

  supplieradmin->init (id, op, this->SA_POA_.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  CORBA::Object_var obj = this->resource_manager_->
    activate_object_with_id (id,
                             this->SA_POA_.in (),
                             supplieradmin,
                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  supplier_admin_ids_.next ();

  return CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::get_consumeradmin (CosNotifyChannelAdmin::AdminID id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  CORBA::Object_var obj =
    this->resource_manager_->id_to_reference (id, this->CA_POA_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  return CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::get_supplieradmin (CosNotifyChannelAdmin::AdminID id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  CORBA::Object_var obj =
    this->resource_manager_->id_to_reference (id, this->SA_POA_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  return CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in ());
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel_i::get_all_consumeradmins (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->consumer_admin_ids_.get_sequence (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel_i::get_all_supplieradmins (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->supplier_admin_ids_.get_sequence (ACE_TRY_ENV);
}

CosNotification::QoSProperties*
TAO_Notify_EventChannel_i::get_qos (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (ACE_TRY_ENV);
}

void
TAO_Notify_EventChannel_i::set_qos (const CosNotification::QoSProperties & qos,CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos, ACE_TRY_ENV);
}

void
TAO_Notify_EventChannel_i::validate_qos (const CosNotification::QoSProperties & required_qos, CosNotification::NamedPropertyRangeSeq_out available_qos, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos, ACE_TRY_ENV);
}

CosNotification::AdminProperties*
TAO_Notify_EventChannel_i::get_admin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CORBA::Long property_count = 3; //The spec has 3 properties, so far.

  CosNotification::AdminProperties_var admin;

  ACE_NEW_THROW_EX (admin,
                    CosNotification::AdminProperties (property_count),
                    CORBA::NO_MEMORY ());
  admin->length (property_count);

  (*admin)[0].name =
  CORBA::string_dup (CosNotification::MaxQueueLength);
  (*admin)[0].value <<= (CORBA::Long)max_queue_length_;

  (*admin)[1].name =
  CORBA::string_dup (CosNotification::MaxConsumers);
  (*admin)[1].value <<= (CORBA::Long)max_consumers_;

  (*admin)[2].name =
  CORBA::string_dup (CosNotification::MaxSuppliers);
  (*admin)[2].value <<= (CORBA::Long)max_suppliers_;

  return admin._retn ();
}

void
TAO_Notify_EventChannel_i::set_admin (const CosNotification::AdminProperties & admin, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedAdmin
                   ))
{
  for (CORBA::ULong i = 0; i < admin.length (); ++i)
    {
      if (ACE_OS::strcmp (admin[i].name,
                          CosNotification::MaxQueueLength) == 0)
        {
          admin[i].value >>= max_queue_length_;
        }
      else if (ACE_OS::strcmp (admin[i].name,
                               CosNotification::MaxSuppliers) == 0)
        {
          admin[i].value >>= max_suppliers_;
        }
      else if (ACE_OS::strcmp (admin[i].name,
                               CosNotification::MaxConsumers) == 0)
        {
          admin[i].value >>= max_consumers_;
        }
      else
        ACE_THROW (CosNotification::UnsupportedAdmin ());
    }
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::for_consumers (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // There is not no way to destroy CosEventChannelAdmin::ConsumerAdmins
  // so we just return the default Consumer Admin here.
  // TODO: find a way to disable the destroy method in the default Admin.

  CORBA::Object_var obj =
    this->get_consumeradmin (this->default_id_, ACE_TRY_ENV);

  return CosEventChannelAdmin::ConsumerAdmin::_narrow (obj.in (),
                                                       ACE_TRY_ENV);
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::for_suppliers (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CORBA::Object_var obj =
    this->get_supplieradmin (this->default_id_, ACE_TRY_ENV);

  return CosEventChannelAdmin::SupplierAdmin::_narrow (obj.in (),
                                                       ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::AdminID, CosNotifyChannelAdmin::AdminIDSeq>;
template class TAO_Notify_ID_Pool<CosNotifyChannelAdmin::AdminID>;

template class ACE_Unbounded_Set<CosNotifyChannelAdmin::AdminID>;
template class ACE_Unbounded_Set_Iterator<CosNotifyChannelAdmin::AdminID>;
template class ACE_Node<CosNotifyChannelAdmin::AdminID>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::AdminID, CosNotifyChannelAdmin::AdminIDSeq>;
#pragma instantiate TAO_Notify_ID_Pool<CosNotifyChannelAdmin::AdminID>

#pragma instantiate ACE_Unbounded_Set<CosNotifyChannelAdmin::AdminID>
#pragma instantiate ACE_Unbounded_Set_Iterator<CosNotifyChannelAdmin::AdminID>
#pragma instantiate ACE_Node<CosNotifyChannelAdmin::AdminID>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

/* -*- C++ -*- $Id$ */
#include "ace/Auto_Ptr.h"
#include "orbsvcs/orbsvcs/Notify/Notify_EventChannel_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_SupplierAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ConsumerAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_FilterFactory_i.h"

// Implementation skeleton constructor
TAO_Notify_EventChannel_i::TAO_Notify_EventChannel_i (TAO_Notify_EventChannelFactory_i& my_factory)
:my_factory_ (my_factory)
{
}

// Implementation skeleton destructor
TAO_Notify_EventChannel_i::~TAO_Notify_EventChannel_i (void)
{
}

void
TAO_Notify_EventChannel_i::init (CORBA::Environment &ACE_TRY_ENV)
{
  // TODO: init data members
  // ???? Pradeep: auto_ptr can't be used this way.
  // dispatcher_ =
  //  auto_ptr<TAO_Notify_Dispatcher>(TAO_Notify_Dispatcher::create (ACE_TRY_ENV));
  dispatcher_ = TAO_Notify_Dispatcher::create (ACE_TRY_ENV);
  ACE_CHECK;

  TAO_Notify_FilterFactory_i* filter_factory_i;
  ACE_NEW_THROW_EX (filter_factory_i,
                    TAO_Notify_FilterFactory_i (),
                    CORBA::NO_MEMORY ());

  this->filter_factory_ =
    filter_factory_i->get_ref (ACE_TRY_ENV);
  ACE_CHECK;
}

TAO_Notify_Dispatcher&
TAO_Notify_EventChannel_i::get_dispatcher (void)
{
  return *dispatcher_;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannel_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::EventChannel_var notify_ec;

  notify_ec = _this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  return notify_ec._retn ();
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannel_i::MyFactory (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return my_factory_._this (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::default_consumer_admin (CORBA::Environment & /*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException
                   ))
{
  //Add your implementation here
  return 0;
}

CosNotifyChannelAdmin::SupplierAdmin_ptr TAO_Notify_EventChannel_i::default_supplier_admin (CORBA::Environment & /*ACE_TRY_ENV*/ )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  //Add your implementation here
  return 0;
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannel_i::default_filter_factory (
                                                   CORBA::Environment & //ACE_TRY_ENV
                                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyFilter::FilterFactory::_duplicate (filter_factory_.in ());
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::new_for_consumers
(CosNotifyChannelAdmin::InterFilterGroupOperator /*op*/,
 CosNotifyChannelAdmin::AdminID_out id,
 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  //Add your implementation here

  CosNotifyChannelAdmin::ConsumerAdmin_var consumeradmin_ret;
  TAO_Notify_ConsumerAdmin_i* consumer_admin;

  ACE_NEW_THROW_EX (consumer_admin,
                    TAO_Notify_ConsumerAdmin_i (*this),
                    CORBA::NO_MEMORY ());

  auto_ptr <TAO_Notify_ConsumerAdmin_i> auto_consumeradmin (consumer_admin);

  consumer_admin->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  dispatcher_->add_dispatcher (consumer_admin->get_dispatcher ());
  // @@ check ret error!

  consumeradmin_ret = consumer_admin->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  // Add to the map
  id = consumer_admin_ids.get ();
  if (consumer_admin_map_.bind (id,
                                consumer_admin) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  auto_consumeradmin.release ();
  return consumeradmin_ret._retn ();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::new_for_suppliers (
                                              CosNotifyChannelAdmin::InterFilterGroupOperator /*op*/,
CosNotifyChannelAdmin::AdminID_out id,
CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  //Add your implementation here
  CosNotifyChannelAdmin::SupplierAdmin_var supplieradmin_ret;
  TAO_Notify_SupplierAdmin_i* supplier_admin;

  ACE_NEW_THROW_EX (supplier_admin,
                    TAO_Notify_SupplierAdmin_i (*this),
                    CORBA::NO_MEMORY ());

  auto_ptr <TAO_Notify_SupplierAdmin_i> auto_supplieradmin (supplier_admin);

  supplier_admin->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  supplieradmin_ret = supplier_admin->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

 // Add to the map
  id = supplier_admin_ids.get ();
  if (supplier_admin_map_.bind (id,
                                supplier_admin) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  auto_supplieradmin.release ();
  return supplieradmin_ret._retn ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::get_consumeradmin
(CosNotifyChannelAdmin::AdminID id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_Notify_ConsumerAdmin_i* admin;

  if (consumer_admin_map_.find (id,admin) == -1)
    ACE_THROW_RETURN (CosNotifyChannelAdmin::AdminNotFound (),
                      CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  return admin->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::get_supplieradmin
(CosNotifyChannelAdmin::AdminID id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  //Add your implementation here
  TAO_Notify_SupplierAdmin_i* admin;

  if (supplier_admin_map_.find (id,admin) == -1)
    ACE_THROW_RETURN (CosNotifyChannelAdmin::AdminNotFound (),
                      CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  return admin->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminIDSeq * TAO_Notify_EventChannel_i::get_all_consumeradmins (
                                                                                       CORBA::Environment & //ACE_TRY_ENV
                                                                                       )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

CosNotifyChannelAdmin::AdminIDSeq * TAO_Notify_EventChannel_i::get_all_supplieradmins (
                                                                                       CORBA::Environment & //ACE_TRY_ENV
                                                                                       )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

CosNotification::AdminProperties * TAO_Notify_EventChannel_i::get_admin (
                                                                         CORBA::Environment & //ACE_TRY_ENV
                                                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  //Add your implementation here
  return 0;
}

void TAO_Notify_EventChannel_i::set_admin (
                                           const CosNotification::AdminProperties & /*admin*/,
                                           CORBA::Environment & //ACE_TRY_ENV
                                           )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedAdmin
                   ))

{
  //Add your implementation here
}

CosEventChannelAdmin::ConsumerAdmin_ptr TAO_Notify_EventChannel_i::for_consumers (
                                                                                  CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
  CORBA::SystemException
  ))

  {
  //Add your implementation here
    return 0;
  }

  CosEventChannelAdmin::SupplierAdmin_ptr TAO_Notify_EventChannel_i::for_suppliers (
                                                                                    CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
  CORBA::SystemException
  ))

  {
  //Add your implementation here
    return 0;
  }

  void TAO_Notify_EventChannel_i::destroy (
                                           CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
  CORBA::SystemException
  ))

  {
  //Add your implementation here
  }


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *>;
template class ACE_Hash_Map_Manager<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;

template class ACE_Hash_Map_Entry<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *>;
template class ACE_Hash_Map_Manager<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>;

template class ID_Pool<CosNotifyChannelAdmin::AdminID>;
//template class ID_Pool<CosNotifyChannelAdmin::AdminID>;


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_ConsumerAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>

#pragma instantiate ACE_Hash_Map_Entry<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::AdminID, TAO_Notify_SupplierAdmin_i *,ACE_Hash<CosNotifyChannelAdmin::AdminID>, ACE_Equal_To<CosNotifyChannelAdmin::AdminID>,ACE_SYNCH_MUTEX>

#pragma instantiate ID_Pool<CosNotifyChannelAdmin::AdminID> 
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

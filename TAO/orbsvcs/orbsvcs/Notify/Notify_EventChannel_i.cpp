/* -*- C++ -*- $Id$ */

// @@ Not only the #include in the header file was uneeded, you repeat
// it here!
#include "ace/Auto_Ptr.h"

#include "orbsvcs/orbsvcs/Notify/Notify_EventChannel_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_SupplierAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ConsumerAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_FilterFactory_i.h"

// Implementation skeleton constructor
TAO_Notify_EventChannel_i::TAO_Notify_EventChannel_i (TAO_Notify_EventChannelFactory_i& my_factory)
  :my_factory_ (my_factory),
   dispatcher_ (0),
   default_op_ (CosNotifyChannelAdmin::OR_OP),
   default_id_ (0),
   max_queue_length_ (0),
   max_consumers_ (0),

   // @@ The following comment belongs in the header file!
   max_suppliers_ (0)  // O implies no limit
{
}

// Implementation skeleton destructor
TAO_Notify_EventChannel_i::~TAO_Notify_EventChannel_i (void)
{
  // @@ Who creates the dispatcher? It is an strategy, what is the
  // abstract factory for it?
  delete dispatcher_;
}

void
TAO_Notify_EventChannel_i::init (const CosNotification::QoSProperties& initial_qos,
                                 const CosNotification::AdminProperties& initial_admin,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  // try to set initial qos params
  this->set_qos (initial_qos, ACE_TRY_ENV);
  ACE_CHECK;

  // try to set initial admin params
  this->set_admin (initial_admin, ACE_TRY_ENV);
  ACE_CHECK;

  // @@ Pradeep: instead of using a factory method it is much better
  // to use an abstract factory to control any (and all) the
  // strategies on each event channel.

  dispatcher_ = TAO_Notify_Dispatcher::create (ACE_TRY_ENV);
  ACE_CHECK;

  // @@ Please check with Irfan on how to activate an object in a
  // thread-safe and exception-safe way!

  TAO_Notify_FilterFactory_i* filter_factory_i;
  ACE_NEW_THROW_EX (filter_factory_i,
                    TAO_Notify_FilterFactory_i (),
                    CORBA::NO_MEMORY ());

  this->filter_factory_ =
    filter_factory_i->get_ref (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::AdminID id_unused;

  this->default_consumeradmin_ =
    this->new_for_consumers (default_op_, id_unused, ACE_TRY_ENV);
  ACE_CHECK;

  this->default_supplieradmin_ =
    this->new_for_suppliers (default_op_, id_unused, ACE_TRY_ENV);
  ACE_CHECK;
}

// @@ Pradeep: here is an excellent candidate for inlining!
TAO_Notify_Dispatcher&
TAO_Notify_EventChannel_i::get_dispatcher (void)
{
  return *dispatcher_;
}

// @@ Pradeep, see my comments about get_ref()...
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
  // @@ Pradeep: see, even you get confused, here you are using
  // _this() instead of get_ref().
  // @@ Pradeep: don't forget this->my_factory_
  // @@ Pradeep: it seems you only use my_factory_ to return the
  // object reference, why don't you store the object reference
  // instead?
  return my_factory_._this (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::default_consumer_admin (CORBA::Environment& /*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::ConsumerAdmin::
    _duplicate (default_consumeradmin_.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::default_supplier_admin (CORBA::Environment & /*ACE_TRY_ENV*/ )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return CosNotifyChannelAdmin::SupplierAdmin::
    _duplicate (default_supplieradmin_.in ());
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
(CosNotifyChannelAdmin::InterFilterGroupOperator op,
 CosNotifyChannelAdmin::AdminID_out id,
 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::ConsumerAdmin_var consumeradmin_ret;
  TAO_Notify_ConsumerAdmin_i* consumer_admin;

  // Get an id.
  id = consumer_admin_ids.get ();

  // Add to the map
  if (consumer_admin_map_.bind (id,
                                consumer_admin) == -1)
    {
      // return the id to the pool.
      consumer_admin_ids.put (id);

      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CosNotifyChannelAdmin::ConsumerAdmin::_nil ());
    }

  ACE_NEW_THROW_EX (consumer_admin,
                    TAO_Notify_ConsumerAdmin_i (*this),
                    CORBA::NO_MEMORY ());

  // @@ Please check my general comments about object activation.
  auto_ptr <TAO_Notify_ConsumerAdmin_i> auto_consumeradmin (consumer_admin);

  consumer_admin->init (id, op, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  dispatcher_->add_dispatcher (consumer_admin->get_dispatcher ());
  // @@ check ret error!

  consumeradmin_ret = consumer_admin->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  auto_consumeradmin.release ();
  return consumeradmin_ret._retn ();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::new_for_suppliers (
                                              CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                              CosNotifyChannelAdmin::AdminID_out id,
                                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::SupplierAdmin_var supplieradmin_ret;
  TAO_Notify_SupplierAdmin_i* supplier_admin;

  ACE_NEW_THROW_EX (supplier_admin,
                    TAO_Notify_SupplierAdmin_i (*this),
                    CORBA::NO_MEMORY ());

  auto_ptr <TAO_Notify_SupplierAdmin_i> auto_supplieradmin (supplier_admin);

  // get a new ID.
  id = supplier_admin_ids.get ();

  // Add to the map
  if (supplier_admin_map_.bind (id,
                                supplier_admin) == -1)
    {
      supplier_admin_ids.put (id);

      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CosNotifyChannelAdmin::SupplierAdmin::_nil ());
    }

  supplier_admin->init (id, op, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  supplieradmin_ret = supplier_admin->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

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

  // @@ get_ref vs. _this?
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
  TAO_Notify_SupplierAdmin_i* admin;

  if (supplier_admin_map_.find (id,admin) == -1)
    ACE_THROW_RETURN (CosNotifyChannelAdmin::AdminNotFound (),
                      CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  // @@ get_ref vs. _this?
  return admin->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel_i::get_all_consumeradmins (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // @@ Use the T_var here...
  CosNotifyChannelAdmin::AdminIDSeq* list;

  // Figure out the length of the list.
  CORBA::ULong len = consumer_admin_map_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    CosNotifyChannelAdmin::AdminIDSeq (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // @@ Synchronization problems.

  // Create an iterator
  CONSUMERADMIN_MAP::ITERATOR iter (consumer_admin_map_);

  // Iterate over and populate the list.
  CONSUMERADMIN_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();

      (*list)[i] =
        hash_entry->ext_id_;
    }

  return list;
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel_i::get_all_supplieradmins (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Figure out the length of the list.
  CORBA::ULong len = supplier_admin_map_.current_size ();

  // @@ Use the right T_var
  // @@ Pradeep: it will make it much easier on you if code like this
  // (it is essentially the same stuff that you did for consumer
  // admins) is either:
  // 1) Identical except for name changes (here your len is obtained
  // before the <list> argument is declared, previously it was the
  // opposite.
  // 2) Use some sort of template.
  CosNotifyChannelAdmin::AdminIDSeq* list;

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    CosNotifyChannelAdmin::AdminIDSeq (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // @@ Synchronization problems.

  // Create an iterator
  SUPPLIERADMIN_MAP::ITERATOR iter (supplier_admin_map_);

  // Iterate over and populate the list.
  SUPPLIERADMIN_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();

      (*list)[i] =
        hash_entry->ext_id_;
    }

  return list;
}

CosNotification::AdminProperties*
TAO_Notify_EventChannel_i::get_admin (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CORBA::Long property_count = 3; //The spec has 3 properties, so far.

  // @@ Please use a T_var
  CosNotification::AdminProperties *admin;

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

  return admin;
}

void
TAO_Notify_EventChannel_i::set_admin (
                                      const CosNotification::AdminProperties &admin,
                                      CORBA::Environment &ACE_TRY_ENV
                                      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedAdmin
                   ))
{

  // @@ Pradeep, please use 'else if' in the same line, it makes the
  // code far more readable.
  for (CORBA::ULong i = 0; i < admin.length (); ++i)
    {
      if (ACE_OS::strcmp (admin[i].name,
                          CosNotification::MaxQueueLength) == 0)
        {
          admin[i].value >>= max_queue_length_;
        }
      else
        if (ACE_OS::strcmp (admin[i].name,
                            CosNotification::MaxSuppliers) == 0)
          {
            admin[i].value >>= max_suppliers_;
          }
        else
          if (ACE_OS::strcmp (admin[i].name,
                              CosNotification::MaxConsumers) == 0)
            {
              admin[i].value >>= max_consumers_;
            }
          else
            ACE_THROW (CosNotification::UnsupportedAdmin ());
    }
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel_i::for_consumers (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosEventChannelAdmin::ConsumerAdmin_var consumeradmin_ret;
  TAO_Notify_ConsumerAdmin_i* consumer_admin;

  ACE_NEW_THROW_EX (consumer_admin,
                    TAO_Notify_ConsumerAdmin_i (*this),
                    CORBA::NO_MEMORY ());

  // @@ More auto_ptr<> that should be ServantBase_var
  auto_ptr <TAO_Notify_ConsumerAdmin_i> auto_consumeradmin (consumer_admin);

  consumer_admin->init (default_id_, default_op_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ConsumerAdmin::_nil ());

  dispatcher_->add_dispatcher (consumer_admin->get_dispatcher ());
  // @@ check ret error!

  consumeradmin_ret = consumer_admin->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ConsumerAdmin::_nil ());

  auto_consumeradmin.release ();
  return consumeradmin_ret._retn ();
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel_i::for_suppliers (
                                          CORBA::Environment& ACE_TRY_ENV
                                          )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosEventChannelAdmin::SupplierAdmin_var supplieradmin_ret;
  TAO_Notify_SupplierAdmin_i* supplier_admin;

  ACE_NEW_THROW_EX (supplier_admin,
                    TAO_Notify_SupplierAdmin_i (*this),
                    CORBA::NO_MEMORY ());

  // @@ More auto_ptr<> that should be ServantBase_var
  auto_ptr <TAO_Notify_SupplierAdmin_i> auto_supplieradmin (supplier_admin);

  supplier_admin->init (default_id_, default_op_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::SupplierAdmin::_nil ());

  supplieradmin_ret = supplier_admin->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::SupplierAdmin::_nil ());

  auto_supplieradmin.release ();
  return supplieradmin_ret._retn ();
}

void TAO_Notify_EventChannel_i::destroy (
                                         CORBA::Environment & //ACE_TRY_ENV
                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // TODO:
  // Think of the EC as the root object of a composite pattern -
  // "destroy" all its constituents and then deactivate it from the POA.

  // @@ Pradeep, the right order is to deactivate from the POA, and
  // then destroy the components.
  // You may need a "is detroyed" flag, to deal with concurrent
  // invocations that are still executing, even after you deactivate
  // the object.
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

/* -*- C++ -*- $Id$ */
#include "Notify_EventChannelFactory_i.h"
#include "Notify_EventChannel_i.h"
#include "ace/Auto_Ptr.h"

// Implementation skeleton constructor
TAO_Notify_EventChannelFactory_i::TAO_Notify_EventChannelFactory_i (void)
{
}

// Implementation skeleton destructor
TAO_Notify_EventChannelFactory_i::~TAO_Notify_EventChannelFactory_i (void)
{
}

void
TAO_Notify_EventChannelFactory_i::activate (CORBA::Environment &ACE_TRY_ENV)
{
  this->self_ = _this (ACE_TRY_ENV);
  ACE_CHECK;
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory_i::create_channel
(
 const CosNotification::QoSProperties& initial_qos,
 const CosNotification::AdminProperties& initial_admin,
 CosNotifyChannelAdmin::ChannelID_out id,
 CORBA::Environment &ACE_TRY_ENV
 )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS,
                   CosNotification::UnsupportedAdmin
                   ))
{
  TAO_Notify_EventChannel_i* channel;
  CosNotifyChannelAdmin::EventChannel_var ec_ret;

  ACE_NEW_THROW_EX (channel,
                    TAO_Notify_EventChannel_i (*this),
                    CORBA::NO_MEMORY ());

  auto_ptr<TAO_Notify_EventChannel_i> auto_channel (channel);

  channel->init (initial_qos, initial_admin, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  ec_ret = channel->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  // Add to the map
  if (ec_map_.bind (id,
                    channel) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      CosNotifyChannelAdmin::EventChannel::_nil ());

  id = ec_ids.get ();
  // Get id after binding so that we don't waste this id in case
  // the bind fails.

  auto_channel.release ();
  return ec_ret._retn ();
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_Notify_EventChannelFactory_i::get_all_channels (CORBA::Environment & ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::ChannelIDSeq* list;

  // Figure out the length of the list.
  CORBA::ULong len = ec_map_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    CosNotifyChannelAdmin::ChannelIDSeq (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // Create an iterator
  EC_MAP::ITERATOR iter (ec_map_);;

  // Iterate over and populate the list.
  EC_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();

      (*list)[i] =
        hash_entry->ext_id_;
    }

  return list;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory_i::get_event_channel (CosNotifyChannelAdmin::ChannelID id, CORBA::Environment & ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ChannelNotFound
                   ))
{
  TAO_Notify_EventChannel_i* ec;

  if (ec_map_.find (id, ec) == -1)
    ACE_THROW_RETURN (CosNotifyChannelAdmin::ChannelNotFound (),
                      CosNotifyChannelAdmin::EventChannel::_nil ());

  return ec->get_ref (ACE_TRY_ENV);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *>;
template class ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::ChannelID, TAO_Notify_EventChannel_i *,ACE_Hash<CosNotifyChannelAdmin::ChannelID>, ACE_Equal_To<CosNotifyChannelAdmin::ChannelID>,ACE_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// $Id$

#include "EventChannelFactory.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannelFactory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_EventChannelFactory, "$Id$")

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Properties.h"
#include "Builder.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "Notify_Service.h"

TAO_NS_EventChannelFactory::TAO_NS_EventChannelFactory (void)
{
}

TAO_NS_EventChannelFactory::~TAO_NS_EventChannelFactory ()
{
}

void
TAO_NS_EventChannelFactory::init (ACE_ENV_SINGLE_ARG_DECL)
{
  this->default_filter_factory_ = TAO_NS_PROPERTIES::instance()->builder ()->build_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::Servant
TAO_NS_EventChannelFactory::servant (void)
{
  return this;
}

void
TAO_NS_EventChannelFactory::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_NS_EventChannelFactory::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_NS_EventChannelFactory::release (void)
{
  delete this;
  //@@ inform factory
}

CosNotifyFilter::FilterFactory_ptr
TAO_NS_EventChannelFactory::get_default_filter_factory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return CosNotifyFilter::FilterFactory::_duplicate (this->default_filter_factory_.in ());
}

::CosNotifyChannelAdmin::EventChannel_ptr TAO_NS_EventChannelFactory::create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotification::UnsupportedQoS
    , CosNotification::UnsupportedAdmin
  ))
{
  return TAO_NS_PROPERTIES::instance()->builder ()->build_event_channel (this, initial_qos, initial_admin, id ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_NS_EventChannelFactory::get_all_channels (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  CosNotifyChannelAdmin::ChannelIDSeq* seq;

  ACE_NEW_THROW_EX (seq,
                    CosNotifyChannelAdmin::ChannelIDSeq (),
                    CORBA::NO_MEMORY ());

  seq->length (this->event_channel_id_list_.size ());

  TAO_NS_Object_Id_Seq::CONST_ITERATOR iter (this->event_channel_id_list_);

  TAO_NS_Object_Id* object_id;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (object_id); iter.advance (), ++i)
    seq[i] = *object_id;

  return seq;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NS_EventChannelFactory::get_event_channel (CosNotifyChannelAdmin::ChannelID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ChannelNotFound
                   ))
{
  TAO_NS_Object* ns_object = this->find (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  if (ns_object != 0)
    {
      CORBA::Object_var object = ns_object->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

      CosNotifyChannelAdmin::EventChannel_var ec_ret =
        CosNotifyChannelAdmin::EventChannel::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

      return ec_ret._retn ();
    }
  else
    ACE_THROW_RETURN (CosNotifyChannelAdmin::ChannelNotFound (), CosNotifyChannelAdmin::EventChannel::_nil ());
}

void
TAO_NS_EventChannelFactory::insert (TAO_NS_EventChannel* event_channel ACE_ENV_ARG_DECL)
{
  inherited::insert (event_channel ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_channel_id_list_.insert (event_channel->id ());
}

void
TAO_NS_EventChannelFactory::remove (TAO_NS_EventChannel* event_channel ACE_ENV_ARG_DECL)
{
  inherited::remove (event_channel ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_channel_id_list_.remove (event_channel->id ());
}

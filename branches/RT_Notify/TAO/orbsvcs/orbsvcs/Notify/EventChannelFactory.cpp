// $Id$

#include "EventChannelFactory.h"
#include "Properties.h"
#include "Builder.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannelFactory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_EventChannelFactory, "$Id$")

TAO_NS_EventChannelFactory::TAO_NS_EventChannelFactory (void)
{
}

TAO_NS_EventChannelFactory::~TAO_NS_EventChannelFactory ()
{
}

PortableServer::Servant
TAO_NS_EventChannelFactory::servant (void)
{
  return this;
}

void
TAO_NS_EventChannelFactory::_add_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_incr_refcnt ();
}

void
TAO_NS_EventChannelFactory::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  this->_decr_refcnt ();
}

void
TAO_NS_EventChannelFactory::release (void)
{
  delete this;
  //@@ inform factory
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
  
::CosNotifyChannelAdmin::ChannelIDSeq *TAO_NS_EventChannelFactory::get_all_channels (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    return 0;
    //Add your implementation here
  }
  
::CosNotifyChannelAdmin::EventChannel_ptr TAO_NS_EventChannelFactory::get_event_channel (
    CosNotifyChannelAdmin::ChannelID id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyChannelAdmin::ChannelNotFound
  ))

  {
    //Add your implementation here
	return 0;
  }


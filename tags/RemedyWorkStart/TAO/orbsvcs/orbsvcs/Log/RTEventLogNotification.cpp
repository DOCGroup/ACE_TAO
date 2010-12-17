// $Id$

#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/RTEventLogNotification.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/ESF/ESF_Peer_Workers.h"
#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_Dispatching.h"
#include "orbsvcs/Event/EC_Filter_Builder.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Scheduling_Strategy.h"
#include "orbsvcs/Event/EC_ConsumerControl.h"
#include "orbsvcs/ESF/ESF_RefCount_Guard.h"
#include "orbsvcs/ESF/ESF_Proxy_RefCount_Guard.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTEventLogNotification::TAO_RTEventLogNotification (
  RtecEventChannelAdmin::EventChannel_ptr ec)
  : TAO_LogNotification (),
    event_channel_ (RtecEventChannelAdmin::EventChannel::_duplicate (ec))
{

  obtainProxyConsumer ();
}

TAO_RTEventLogNotification::~TAO_RTEventLogNotification (void)
{
  // No-Op.
}

void
TAO_RTEventLogNotification::disconnect_push_supplier (void)
{
  // No-Op.
}

void
TAO_RTEventLogNotification::obtainProxyConsumer (void)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    event_channel_->for_suppliers();

  consumer_ = supplier_admin->obtain_push_consumer();

  RtecEventComm::PushSupplier_var supplier =
  this->_this ();

  // Simple publication, but usually the helper classes in
  // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
  RtecEventChannelAdmin::SupplierQOS qos;
  qos.publications.length (1);
  RtecEventComm::EventHeader& h0 =
    qos.publications[0].event.header;
  h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
  h0.source = 1;                      // first free event source

  consumer_->connect_push_supplier (supplier.in(), qos);
}

void
TAO_RTEventLogNotification::send_notification (const CORBA::Any& any)
{
  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
  event[0].header.source = 1;
  event[0].header.ttl    = 1;
#ifndef TAO_LACKS_EVENT_CHANNEL_ANY
  event[0].data.any_value <<= any;
#else
  ACE_UNUSED_ARG (any);
#endif

  consumer_->push(event);
}

TAO_END_VERSIONED_NAMESPACE_DECL

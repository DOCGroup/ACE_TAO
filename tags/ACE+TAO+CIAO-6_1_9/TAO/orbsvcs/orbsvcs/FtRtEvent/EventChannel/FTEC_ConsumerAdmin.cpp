// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ConsumerAdmin.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxySupplier.h"
#include "orbsvcs/FtRtEvent/EventChannel/ProxySupplierStateWorker.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const FtRtecEventChannelAdmin::OperationType
TAO_FTEC_ConsumerAdmin::OBTAIN_ID = FtRtecEventChannelAdmin::OBTAIN_PUSH_SUPPLIER;


TAO_FTEC_ConsumerAdmin::TAO_FTEC_ConsumerAdmin (TAO_EC_Event_Channel_Base *ec)
  :  TAO_EC_ConsumerAdmin (ec)
  , FT_Aspect(this, this->event_channel_->consumer_poa ())
{
}

TAO_FTEC_ConsumerAdmin::~TAO_FTEC_ConsumerAdmin (void)
{
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
TAO_FTEC_ConsumerAdmin::obtain_push_supplier (void)
{
  ORBSVCS_DEBUG((LM_DEBUG,"obtain_push_supplier\n" ));
  return obtain_proxy();
}

void
TAO_FTEC_ConsumerAdmin::disconnect(RtecEventChannelAdmin::ProxyPushSupplier_ptr obj)
{
  try{
    obj->disconnect_push_supplier();
  }
  catch (...){
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

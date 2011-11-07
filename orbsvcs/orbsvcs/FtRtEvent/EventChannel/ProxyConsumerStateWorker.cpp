// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/ProxyConsumerStateWorker.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ProxyConsumerStateWorker::ProxyConsumerStateWorker (
  FtRtecEventChannelAdmin::ProxyConsumerStates& states)
  : consumerStates_(states)
{
}

ProxyConsumerStateWorker::~ProxyConsumerStateWorker()
{
}

void ProxyConsumerStateWorker::set_size(size_t size)
{
  consumerStates_.length(size);
  index_ = 0;
}

void ProxyConsumerStateWorker::work(TAO_EC_ProxyPushConsumer* object)
{
  TAO_FTEC_ProxyPushConsumer* proxy =
    static_cast<TAO_FTEC_ProxyPushConsumer*> (object);
  proxy->get_state(consumerStates_[index_++]);

TAO_END_VERSIONED_NAMESPACE_DECL}

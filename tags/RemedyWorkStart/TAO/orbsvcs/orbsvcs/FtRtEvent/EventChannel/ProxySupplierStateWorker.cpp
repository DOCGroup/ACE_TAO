// $Id$

#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "orbsvcs/FtRtEvent/EventChannel/ProxySupplierStateWorker.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ProxySupplierStateWorker::ProxySupplierStateWorker (
  FtRtecEventChannelAdmin::ProxySupplierStates& states)
  : supplierStates_(states)
{
}

ProxySupplierStateWorker::~ProxySupplierStateWorker()
{
}

void ProxySupplierStateWorker::set_size(size_t size)
{
  supplierStates_.length(size);
  index_ = 0;
}

void ProxySupplierStateWorker::work(TAO_EC_ProxyPushSupplier* object)
{
  TAO_FTEC_ProxyPushSupplier* proxy =
    static_cast<TAO_FTEC_ProxyPushSupplier*> (object);
  proxy->get_state(supplierStates_[index_++]);
}

TAO_END_VERSIONED_NAMESPACE_DECL

// $Id$

#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "ProxySupplierStateWorker.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"

ACE_RCSID (EventChannel,
           ProxySupplierStateWorker,
           "$Id$")


ProxySupplierStateWorker::ProxySupplierStateWorker
  (FtRtecEventChannelAdmin::ProxySupplierStates& states)
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

void ProxySupplierStateWorker::work(TAO_EC_ProxyPushSupplier* object
            ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_FTEC_ProxyPushSupplier* proxy =
    static_cast<TAO_FTEC_ProxyPushSupplier*>(object);
  proxy->get_state(supplierStates_[index_++]);
}

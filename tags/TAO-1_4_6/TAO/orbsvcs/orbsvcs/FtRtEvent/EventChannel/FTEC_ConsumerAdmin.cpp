// $Id$

#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "FTEC_ConsumerAdmin.h"
#include "FTEC_ProxySupplier.h"
#include "ProxySupplierStateWorker.h"

ACE_RCSID (EventChannel,
           TAO_FTEC_ConsumerAdmin,
           "$Id$")

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
TAO_FTEC_ConsumerAdmin::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG((LM_DEBUG,"obtain_push_supplier\n" ));
  return obtain_proxy(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_FTEC_ConsumerAdmin::disconnect(RtecEventChannelAdmin::ProxyPushSupplier_ptr obj)
{
  ACE_TRY_NEW_ENV {
    obj->disconnect_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL {
  }
  ACE_ENDTRY;
}


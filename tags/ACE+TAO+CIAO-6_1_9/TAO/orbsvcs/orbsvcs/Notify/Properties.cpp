// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/NotifyExtC.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Properties.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Properties::TAO_Notify_Properties (void)
  : factory_ (0)
  , builder_ (0)
  , orb_(0)
  , dispatching_orb_ (0)
  , asynch_updates_ (false)
  , allow_reconnect_ (false)
  , validate_client_ (false)
  , separate_dispatching_orb_ (false)
  , updates_ (1)
  , defaultConsumerAdminFilterOp_ (CosNotifyChannelAdmin::OR_OP)
  , defaultSupplierAdminFilterOp_ (CosNotifyChannelAdmin::OR_OP)
{
  // In case no conf. file is specified, the EC will default to reactive concurrency.
  NotifyExt::ThreadPoolParams tp_params =
    {NotifyExt::CLIENT_PROPAGATED,0, 0, 0, 0, 0, 0, 0,0};

  this->ec_qos_.length (1);
  this->ec_qos_[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
  this->ec_qos_[0].value <<= tp_params;

  if (TAO_debug_level > 1)
    ORBSVCS_DEBUG ((LM_DEBUG, "in TAO_Properties ctos %x\n", this));
}

TAO_Notify_Properties::~TAO_Notify_Properties ()
{
}

TAO_Notify_Properties *
TAO_Notify_Properties::instance (void)
{
  return ACE_Unmanaged_Singleton<TAO_Notify_Properties,
                                 TAO_SYNCH_MUTEX>::instance ();
}

void
TAO_Notify_Properties::close (void)
{
  ACE_Unmanaged_Singleton<TAO_Notify_Properties,
                          TAO_SYNCH_MUTEX>::close ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

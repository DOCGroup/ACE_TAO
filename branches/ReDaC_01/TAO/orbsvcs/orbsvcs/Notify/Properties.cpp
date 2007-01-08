// $Id$

#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/NotifyExtC.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Properties.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify,
           TAO_Notify_Properties,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Properties::TAO_Notify_Properties (void)
  : factory_ (0)
  , builder_ (0)
  , asynch_updates_ (0)
  , allow_reconnect_ (false)
  , updates_ (1)
{
  // In case no conf. file is specified, the EC will default to reactive concurrency.
  NotifyExt::ThreadPoolParams tp_params =
    {NotifyExt::CLIENT_PROPAGATED,0, 0, 0, 0, 0, 0, 0,0};

  this->ec_qos_.length (1);
  this->ec_qos_[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
  this->ec_qos_[0].value <<= tp_params;

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "in TAO_Properties ctos %x\n", this));
}

TAO_Notify_Properties::~TAO_Notify_Properties ()
{
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template class TAO_Singleton<TAO_Notify_Properties, ACE_Thread_Mutex> *TAO_Singleton<TAO_Notify_Properties, ACE_Thread_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

TAO_END_VERSIONED_NAMESPACE_DECL

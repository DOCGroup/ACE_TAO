// $Id$

#include "Properties.h"
#include "orbsvcs/NotifyExtC.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Properties.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_NS_Properties, "$Id$")

TAO_NS_Properties::TAO_NS_Properties (void)
  : factory_ (0)
  , builder_ (0)
  , asynch_updates_ (0)
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

TAO_NS_Properties::~TAO_NS_Properties ()
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Singleton<TAO_NS_Properties, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Singleton<TAO_NS_Properties, TAO_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

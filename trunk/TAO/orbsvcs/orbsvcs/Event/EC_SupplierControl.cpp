// $Id$

#include "EC_SupplierControl.h"

#if ! defined (__ACE_INdLINE__)
#include "EC_SupplierControl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_SupplierControl, "$Id$")

TAO_EC_SupplierControl::TAO_EC_SupplierControl (void)
{
}

TAO_EC_SupplierControl::~TAO_EC_SupplierControl (void)
{
}

int
TAO_EC_SupplierControl::activate (void)
{
  return 0;
}

int
TAO_EC_SupplierControl::shutdown (void)
{
  return 0;
}

void
TAO_EC_SupplierControl::supplier_not_exist (TAO_EC_ProxyPushConsumer *
                                            ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_SupplierControl::system_exception (TAO_EC_ProxyPushConsumer *,
                                          CORBA::SystemException &
                                          ACE_ENV_ARG_DECL_NOT_USED)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// $Id$

#include "CEC_SupplierControl.h"

#if ! defined (__ACE_INdLINE__)
#include "CEC_SupplierControl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_SupplierControl, "$Id$")

TAO_CEC_SupplierControl::TAO_CEC_SupplierControl (void)
{
}

TAO_CEC_SupplierControl::~TAO_CEC_SupplierControl (void)
{
}

int
TAO_CEC_SupplierControl::activate (void)
{
  return 0;
}

int
TAO_CEC_SupplierControl::shutdown (void)
{
  return 0;
}

void
TAO_CEC_SupplierControl::supplier_not_exist (TAO_CEC_ProxyPushConsumer *,
                                             CORBA::Environment &)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

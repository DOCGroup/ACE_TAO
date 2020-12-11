#include "orbsvcs/Event/EC_SupplierControl.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_SupplierControl::TAO_EC_SupplierControl ()
{
}

TAO_EC_SupplierControl::~TAO_EC_SupplierControl ()
{
}

int
TAO_EC_SupplierControl::activate ()
{
  return 0;
}

int
TAO_EC_SupplierControl::shutdown ()
{
  return 0;
}

void
TAO_EC_SupplierControl::supplier_not_exist (TAO_EC_ProxyPushConsumer *)
{
}

void
TAO_EC_SupplierControl::system_exception (TAO_EC_ProxyPushConsumer *,
                                          CORBA::SystemException &)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

/**
 * @file EC_Proxy_Disconnector.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "orbsvcs/Event/EC_Proxy_Disconnector.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_EC_Supplier_Proxy_Disconnect_Functor::operator() (
                       RtecEventComm::PushSupplier_ptr supplier) noexcept
{
  try
    {
      supplier->disconnect_push_supplier ();
    }
  catch (...)
    {
    }
}

void
TAO_EC_Consumer_Proxy_Disconnect_Functor::operator() (
                       RtecEventComm::PushConsumer_ptr consumer) noexcept
{
  try
    {
      consumer->disconnect_push_consumer ();
    }
  catch (...)
    {
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

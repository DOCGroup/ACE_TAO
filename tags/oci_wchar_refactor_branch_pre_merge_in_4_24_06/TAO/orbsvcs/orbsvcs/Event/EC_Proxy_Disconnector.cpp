/**
 * @file EC_Proxy_Disconnector.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "EC_Proxy_Disconnector.h"

void
TAO_EC_Supplier_Proxy_Disconnect_Functor::operator() (
                       RtecEventComm::PushSupplier_ptr supplier)
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      supplier->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
    }
  ACE_ENDTRY;
}

void
TAO_EC_Consumer_Proxy_Disconnect_Functor::operator() (
                       RtecEventComm::PushConsumer_ptr consumer)
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
    }
  ACE_ENDTRY;
}

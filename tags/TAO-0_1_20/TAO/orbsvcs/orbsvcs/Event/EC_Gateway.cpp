//
// $Id$
//

#include "orbsvcs/Event/EC_Gateway.h"

TAO_EC_Gateway::TAO_EC_Gateway (void)
  :  consumer_ (this),
     supplier_ (this)
{
}

TAO_EC_Gateway::~TAO_EC_Gateway (void)
{
}

int
TAO_EC_Gateway::open (RtecEventChannelAdmin::EventChannel_ptr remote_ec,
		      RtecEventChannelAdmin::EventChannel_ptr local_ec,
		      const RtecEventChannelAdmin::ConsumerQOS& subscriptions,
		      const RtecEventChannelAdmin::SupplierQOS& publications,
		      CORBA::Environment &_env)
{
  TAO_TRY
    {
      // = Connect as a supplier to the local EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        local_ec->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_ =
        supplier_admin->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                    publications,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        remote_ec->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var consumer_ref =
        this->consumer_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                    subscriptions,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_EC_Gateway::open");
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;

  return 0;
}

void
TAO_EC_Gateway::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier-consumer received disconnect from channel.\n"));
}

void
TAO_EC_Gateway::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier received disconnect from channel.\n"));
}

void
TAO_EC_Gateway::push (const RtecEventComm::EventSet &events,
                CORBA::Environment & _env)
{
  // ACE_DEBUG ((LM_DEBUG, "TAO_EC_Gateway::push - "));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  // ACE_DEBUG ((LM_DEBUG, "ECP: %d event(s)\n", events.length ()));

  // @@ TODO, there is an extra data copy here, we should do the event
  // modification without it and only compact the necessary events.
  int count = 0;
  RtecEventComm::EventSet out (events.length ());
  for (u_int i = 0; i < events.length (); ++i)
    {
      if (events[i].ttl_ > 0)
        {
          count++;
          out.length (count);
          out[count - 1] = events[i];
          out[count - 1].ttl_--;
        }
    }

  if (count > 0)
    {
      this->consumer_proxy_->push (out, _env);
    }
}

int
TAO_EC_Gateway::shutdown (CORBA::Environment& _env)
{
  TAO_TRY
    {
      this->consumer_proxy_->disconnect_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Disconnect from the push supplier.
      this->supplier_proxy_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN(-1);
    }
  TAO_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushConsumer_Adapter<TAO_EC_Gateway>;
template class ACE_PushSupplier_Adapter<TAO_EC_Gateway>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushConsumer_Adapter<TAO_EC_Gateway>
#pragma instantiate ACE_PushSupplier_Adapter<TAO_EC_Gateway>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

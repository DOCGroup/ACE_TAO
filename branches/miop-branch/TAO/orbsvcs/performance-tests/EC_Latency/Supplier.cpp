/**
 * @file Supplier.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Supplier.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Latency, Supplier, "$Id$")

ECL_Supplier::ECL_Supplier (void)
{
}

void
ECL_Supplier::connect (RtecEventChannelAdmin::EventChannel_ptr ec
                       TAO_ENV_ARG_DECL)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_consumer_.in ()))
      return;

    this->proxy_consumer_ =
      supplier_admin->obtain_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  RtecEventComm::PushSupplier_var supplier =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::SupplierQOS supplier_qos;
  supplier_qos.publications.length (1);
  RtecEventComm::EventHeader& sh0 =
    supplier_qos.publications[0].event.header;
  sh0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
  sh0.source = 1;                      // first free event source

  this->proxy_consumer_->connect_push_supplier (supplier.in (),
                                                supplier_qos
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ECL_Supplier::disconnect (TAO_ENV_SINGLE_ARG_DECL)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_._retn ();
  }

  ACE_TRY
    {
      proxy->disconnect_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  PortableServer::POA_var poa = this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id = poa->servant_to_id (this
                                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ECL_Supplier::push (const RtecEventComm::EventSet &events
                    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_;
  }
  proxy->push (events TAO_ENV_ARG_PARAMETER);

}

void
ECL_Supplier::disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_consumer_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

// $Id$

#include "ECSupplier.h"
#include "ace/Time_Value.h"
#include "ace/Thread.h" //for ACE_Thread::self()
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RtecEventCommC.h"
#include <ace/Counter.h>
#include <ace/OS_NS_sys_time.h>

ACE_RCSID(EC_Examples, ECSupplier, "$Id$")

ECSupplier::ECSupplier (SourceID id,
                        const EventTypeVector &pub_types,
                        Service_Handler *handler)
  : id_ (id)
  , pub_types_(pub_types)
  , handler_(handler)
{
}

ECSupplier::~ECSupplier (void)
{}

void
ECSupplier::set_consumer_proxy(ConsumerProxy::_ptr_type proxy)
{
  // copy the ref
  this->consumer_proxy_ = ConsumerProxy::_duplicate(proxy);
}

void
ECSupplier::rt_info(InfoHandle supplier_rt_info)
{
  this->rt_info_ = supplier_rt_info;
}

ECSupplier::InfoHandle
ECSupplier::rt_info(void)
{
  return this->rt_info_;
}

void
ECSupplier::timeout_occured (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"ECSupplier (%P|%t) calling handle_service_start()\n"));
      this->handler_->handle_service_start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"ECSupplier (%P|%t) handle_service_start() DONE\n"));
    }

  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.source = id_;
  event[0].header.ttl    = 1;
  // TODO: For now, just handle first published type
  event[0].header.type   = this->pub_types_[0];

  ACE_DEBUG((LM_DEBUG,"ECSupplier (id %d) in thread %t will push event type %d\n",this->id_,event[0].header.type));

  this->consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);

  ACE_DEBUG((LM_DEBUG,"ECSupplier (id %d) in thread %t pushed event\n",this->id_));

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"ECSupplier (%P|%t) calling handle_service_stop()\n"));
      this->handler_->handle_service_stop(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"ECSupplier (%P|%t) handle_service_stop() DONE\n"));
    }
}

void
ECSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

ECSupplier::SourceID
ECSupplier::get_id(void) const
{
  return this->id_;
}

Service_Handler *
ECSupplier::handler(void) const
{
  return this->handler_;
}

const ECSupplier::EventTypeVector &
ECSupplier::getPublishedTypes (void) const
{
  return this->pub_types_;
}

// ****************************************************************
/*
ECTimeout_Consumer::ECTimeout_Consumer (ECSupplier* supplier)
  :supplier_impl_ (supplier)
{
}

void
ECTimeout_Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutConsumer (%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) Timeout Event received\n"));

  supplier_impl_->timeout_occured (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
ECTimeout_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}
*/
// ****************************************************************

/// Constructor
ECSupplier_Timeout_Handler::ECSupplier_Timeout_Handler (ECSupplier * supplier_impl)
  : supplier_impl_(supplier_impl)
{}

int
ECSupplier_Timeout_Handler::handle_timeout (const ACE_Time_Value &,
                                          const void *)
{
  ACE_DEBUG ((LM_DEBUG, "ECSupplier_Timeout_Handler (%t): timeout received\n"));
  supplier_impl_->timeout_occured (ACE_ENV_SINGLE_ARG_PARAMETER);

  return 0;
} //ECSupplier_Timeout_Handler::handle_timeout()

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

/* -*- C++ -*- */
// $Id$

#include "Consumer.h"

void
Consumer::open (CosEventChannelAdmin::EventChannel_ptr event_channel,
                     CORBA::ORB_ptr orb
                     ACE_ENV_ARG_DECL)
{
  this->orb_ = orb;

  // = Connect as a consumer.
  this->consumer_admin_ =
    event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
Consumer::close (ACE_ENV_SINGLE_ARG_DECL)
{
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_admin_ =
    CosEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
Consumer::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  CosEventComm::PushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_->connect_push_consumer (objref.in ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
Consumer::push (const CORBA::Any &
                     ACE_ENV_ARG_DECL_NOT_USED)
 ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ))
{
  ACE_DEBUG ((LM_DEBUG,
              "in Consumer::push\n"));
  // this->orb_->shutdown ();
  // @@ read comment in run
}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
 ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  // Deactivate this object.

  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

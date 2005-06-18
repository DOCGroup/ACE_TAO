// $Id$

//==============================================================
/**
 * @file RTEventService_exec.cpp
 *
 * Source file for the actual RTEventService and RTEventServiceHome
 * component implementations.
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//==============================================================

#include "RTEventService_exec.h"


MyImpl::RTEventService_exec_impl::RTEventService_exec_impl ()
{
  // Get a reference to the ORB
  char * argv[1] = { "RTEventService_exec" };
  int argc = sizeof (argv) / sizeof (argv[0]);
  this->orb_ = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get a reference to the POA
  CORBA::Object_var poa_object =
  orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (this->poa_.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));

  // Create event channel
  TAO_EC_Event_Channel_Attributes attributes (poa_.in (), poa_.in ());
  TAO_EC_Event_Channel * ec_servant;
  ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
  ec_servant->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->event_channel_ = ec_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

MyImpl::RTEventService_exec_impl::~RTEventService_exec_impl ()
{
  
}


::RtecEventChannelAdmin::ConsumerAdmin_ptr
MyImpl::RTEventService_exec_impl::for_consumers (
ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::RtecEventChannelAdmin::ConsumerAdmin_ptr ret_val =
    this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  return ret_val;
}

::RtecEventChannelAdmin::SupplierAdmin_ptr
MyImpl::RTEventService_exec_impl::for_suppliers (
ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::RtecEventChannelAdmin::SupplierAdmin_ptr ret_val =
    this->event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  return ret_val;
}

void
MyImpl::RTEventService_exec_impl::destroy (
ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->event_channel_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

RtecEventChannelAdmin::Observer_Handle
MyImpl::RTEventService_exec_impl::append_observer (
  RtecEventChannelAdmin::Observer_ptr gw
  ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((
  CORBA::SystemException,
  RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
  RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  RtecEventChannelAdmin::Observer_Handle ret_val =
    this->event_channel_->append_observer (gw ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return ret_val;
}

void
MyImpl::RTEventService_exec_impl::remove_observer (
  RtecEventChannelAdmin::Observer_Handle gw
  ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((
  CORBA::SystemException,
  RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
  RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  this->event_channel_->remove_observer (gw ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void MyImpl::RTEventService_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RTEventService_exec_impl::set_session_context\n"));

  context_ = BasicSP::CCM_RTEventService_Context::_narrow (ctx);
}

void MyImpl::RTEventService_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RTEventService_exec_impl::ccm_activate\n"));
}

void MyImpl::RTEventService_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{

}

void MyImpl::RTEventService_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{

}

RtecEventChannelAdmin::CCM_EventChannel_ptr
MyImpl::RTEventService_exec_impl::get_rt_event_channel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "RTEventService_exec_impl::get_rt_event_channel\n"));
  return RtecEventChannelAdmin::CCM_EventChannel::_duplicate (this);
}

MyImpl::RTEventServiceHome_exec_impl::RTEventServiceHome_exec_impl ()
{
}

MyImpl::RTEventServiceHome_exec_impl::~RTEventServiceHome_exec_impl ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::RTEventServiceHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new RTEventService_exec_impl;
}

extern "C" RTEVENTSERVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createRTEventServiceHome_Impl (void)
{
  return new MyImpl::RTEventServiceHome_exec_impl;
}

MyImpl::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (void)
{
}

MyImpl::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate (orb))
{
}
  
void MyImpl::RTEventServiceSupplier_impl::disconnect_push_supplier (void)
{
  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}

MyImpl::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl (void)
{
}

MyImpl::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl
  (CORBA::ORB_ptr orb,
   Components::EventConsumerBase_ptr consumer) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
{
}

void MyImpl::RTEventServiceConsumer_impl::push (const RtecEventComm::EventSet& events)
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_GLUE_BasicSP::RTEventServiceConsumer_impl::push\n"));
  for (unsigned long i = 0; i < events.length (); i++)
    {
      ::Components::EventBase * ev;
      if (events[i].data.any_value >>= ev)
        {
          this->event_consumer_->push_event (ev
                                           ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void MyImpl::RTEventServiceConsumer_impl::disconnect_push_consumer (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RTEventServiceConsumer_impl::disconnect_push_consumer\n"));

  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}

CIAO::Object_Reference_Cookie::Object_Reference_Cookie ()
{

}

CIAO::Object_Reference_Cookie::~Object_Reference_Cookie ()
{

}

CIAO::Object_Reference_Cookie::Object_Reference_Cookie (CORBA::Object_ptr obj)
{
  CORBA::ORB_var orb = obj->orb_core ()->orb ();

  CORBA::Object_var poa_object =
  orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));

  PortableServer::ObjectId_var oid = poa->reference_to_id (obj);

  // @@ Bala, is this line necessary?
  this->cookieValue ().length (oid->length ());

  this->cookieValue (oid.in ());
}

int
CIAO::Object_Reference_Cookie::insert (CORBA::Object_ptr obj)
{
  CORBA::ORB_var orb = obj->orb_core ()->orb ();

  CORBA::Object_var poa_object =
  orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));

  PortableServer::ObjectId_var oid = poa->reference_to_id (obj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->cookieValue ().length (oid->length ());

  this->cookieValue (oid.in ());

  return 0;
}

int
CIAO::Object_Reference_Cookie::extract (::Components::Cookie *ck,
                                        CORBA::Object_ptr obj)
{

  // @@ Bala, this obviously doesn't work. How can I get a reference to the ORB here?
  CORBA::ORB_var orb = obj->orb_core ()->orb ();

  CORBA::Object_var poa_object =
  orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));

  CIAO::Cookie *c = CIAO::Cookie::_downcast (ck);

  if (c == 0)
    return -1;

  PortableServer::ObjectId_var oid = c->get_cookie ();

  obj = poa->id_to_reference (oid.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  return 0;
}


CORBA::ValueBase *
CIAO::Object_Reference_Cookie_init::create_for_unmarshal (void)
{
  CORBA::ValueBase * return_value = 0;
  ACE_NEW_RETURN (return_value,
                  CIAO::Object_Reference_Cookie,
                  0);
  return return_value;
}

::CORBA::OctetSeq *
CIAO::Object_Reference_Cookie::get_cookie (ACE_ENV_SINGLE_ARG_DECL)
{
  return &this->cookieValue ();
}

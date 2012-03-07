// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "PushSupplier.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"

int
PushSupplier_impl::ReactorTask::svc (void)
{
  ACE_DEBUG((LM_DEBUG, "Reactor Thread started\n"));
  ACE_Reactor reactor (new ACE_Select_Reactor) ;
  reactor_ = &reactor;

  extern ACE_Time_Value timer_interval;

  if (reactor_->schedule_timer(handler_, 0, ACE_Time_Value::zero, timer_interval)== -1)
    ACE_ERROR_RETURN((LM_ERROR,"Cannot schedule timer\n"),-1);

  reactor_->run_reactor_event_loop();
  ACE_DEBUG((LM_DEBUG, "Reactor Thread ended\n"));

  return 0;
}



PushSupplier_impl::PushSupplier_impl(CORBA::ORB_ptr orb)
: orb_(orb), seq_no_(0), reactor_task_(this)
{
}

PushSupplier_impl::~PushSupplier_impl()
{
  reactor_task_.wait();
}

int PushSupplier_impl::init(RtecEventChannelAdmin::EventChannel_ptr channel)
{

  ACE_DEBUG((LM_DEBUG, "for_suppliers\n"));
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    channel->for_suppliers();

  ACE_DEBUG((LM_DEBUG, "obtain_push_consumer\n"));
  consumer_ =
    supplier_admin->obtain_push_consumer();



  ACE_DEBUG((LM_DEBUG, "got push_consumer with %d profiles\n",
                        consumer_->_stubobj ()->base_profiles ().profile_count ()));

    RtecEventChannelAdmin::SupplierQOS qos;
    qos.publications.length (1);
    RtecEventComm::EventHeader& h0 =
        qos.publications[0].event.header;
    h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
    h0.source = 1;                      // first free event source

  RtecEventComm::PushSupplier_var supplier = _this();

    ACE_DEBUG((LM_DEBUG, "connect_push_supplier\n"));
  consumer_->connect_push_supplier(supplier.in(),
    qos);

  ACE_DEBUG((LM_DEBUG, "push_consumer connected\n"));


  if (!reactor_task_.thr_count() &&
    reactor_task_.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
    "Cannot activate reactor thread\n"),
    -1);

  return 0;

}



void  PushSupplier_impl::disconnect_push_supplier (
      )
{

  reactor_task_.reactor_->end_reactor_event_loop();

  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(orb_.in(), "POACurrent");

  PortableServer::POA_var poa = current->get_POA();

  PortableServer::ObjectId_var oid = current->get_object_id();

  poa->deactivate_object (oid.in ());
}

int PushSupplier_impl::handle_timeout (const ACE_Time_Value &current_time,
                     const void *act)
{
  ACE_UNUSED_ARG(act);
  ACE_UNUSED_ARG(current_time);

  try{
    RtecEventComm::EventSet event (1);
    event.length (1);
    event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
    event[0].header.source = 1;
    event[0].header.ttl    = 1;

    ACE_Time_Value time_val = ACE_OS::gettimeofday ();

    event[0].header.ec_send_time = time_val.sec () * 10000000 + time_val.usec ()* 10;
    event[0].data.any_value <<= seq_no_;

    consumer_->push(event);
    ACE_DEBUG((LM_DEBUG, "sending data %d\n", seq_no_));
    ++seq_no_;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("A CORBA Exception occurred.");
  }
  return 0;
}

// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "PushSupplier.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "orbsvcs/FtRtEvent/Utils/Log.h"

ACE_RCSID (FtRtEvent,
           PushSupplier,
           "$Id$")

extern int NUM_ITERATIONS;

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

  return 0;
}



PushSupplier_impl::PushSupplier_impl(CORBA::ORB_ptr orb)
: orb_(orb),supplier_servant_(this), seq_no_(0), reactor_task_(this)
{
}

PushSupplier_impl::~PushSupplier_impl()
{
  reactor_task_.wait();
}

int PushSupplier_impl::init(RtecEventChannelAdmin::EventChannel_ptr channel ACE_ENV_ARG_DECL)
{


  RtecEventChannelAdmin::SupplierQOS qos;
  qos.publications.length (1);
  RtecEventComm::EventHeader& h0 =
    qos.publications[0].event.header;
  h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
  h0.source = 1;                      // first free event source

  RtecEventComm::PushSupplier_var supplier = supplier_servant_._this();

  ACE_Time_Value time_val = ACE_OS::gettimeofday ();

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    channel->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  consumer_ =
    supplier_admin->obtain_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  consumer_->connect_push_supplier(supplier.in(),
    qos   ACE_ENV_ARG_PARAMETER);

   time_val = ACE_OS::gettimeofday () - time_val;

  ACE_DEBUG((LM_DEBUG, "connected to proxy_push_consumer, subscription latency = %d\n", time_val.sec () * 10000000 + time_val.usec ()* 10));


  if (!reactor_task_.thr_count() &&
    reactor_task_.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
    "Cannot activate reactor thread\n"),
    -1);

  return 0;

}



void  PushSupplier_impl::disconnect_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{

  reactor_task_.reactor_->end_reactor_event_loop();

  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(orb_.in(), "POACurrent" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var poa = current->get_POA(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid = current->get_object_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object(oid ACE_ENV_ARG_PARAMETER);
}

int PushSupplier_impl::handle_timeout (const ACE_Time_Value &current_time,
                     const void *act)
{
  ACE_UNUSED_ARG(act);
  ACE_UNUSED_ARG(current_time);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    RtecEventComm::EventSet event (1);
    event.length (1);
    event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
    event[0].header.source = 1;
    event[0].header.ttl    = 1;

    ACE_Time_Value time_val = ACE_OS::gettimeofday ();

    event[0].header.ec_send_time = time_val.sec () * 10000000 + time_val.usec ()* 10;
    event[0].data.any_value <<= seq_no_;

    consumer_->push(event ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    TAO_FTRTEC::Log(1, "sending data %d\n", seq_no_);

    if (NUM_ITERATIONS ==(int) ++seq_no_) {
      ACE_DEBUG((LM_DEBUG, "shutdown orb\n"));
      this->reactor()->cancel_timer(this);
      this->reactor()->end_reactor_event_loop();
      this->reactor(0);
      orb_->shutdown();
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA Exception occurred.");
  }
  ACE_ENDTRY;
  return 0;
}

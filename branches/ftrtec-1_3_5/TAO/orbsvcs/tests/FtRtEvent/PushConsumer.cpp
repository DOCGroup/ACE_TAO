// $Id$

#include "PushConsumer.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "FtRtEvent_Test.h"
#include <fstream>
#include "orbsvcs/FtRtEvent/Utils/Log.h"


ACE_RCSID (FtRtEvent,
           PushConsumer,
           "$Id$")


PushConsumer_impl::PushConsumer_impl()
{
}

int PushConsumer_impl::init(CORBA::ORB_ptr orb,
                            RtecEventChannelAdmin::EventChannel_ptr channel,
                            const Options& options ACE_ENV_ARG_DECL)
{
  orb_ = orb;
  num_iterations_ = options.num_iterations;
  run_times_.assign(options.num_iterations, -1);

  RtecEventChannelAdmin::ConsumerQOS qos;
  qos.is_gateway = 1;
  qos.dependencies.length(1);

  RtecEventComm::EventHeader& h0 =
    qos.dependencies[0].event.header;
  h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
  h0.source = ACE_ES_EVENT_SOURCE_ANY;

  RtecEventComm::PushConsumer_var push_consumer = _this();

  CORBA::String_var str = orb_->object_to_string(push_consumer.in());
  std::ofstream out(options.proxy_consumer_file.c_str());
  out << str.in();

  ACE_Time_Value time_val = ACE_OS::gettimeofday ();

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    channel->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  supplier_ =
    consumer_admin->obtain_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  supplier_->connect_push_consumer(push_consumer.in(),
    qos   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  time_val = ACE_OS::gettimeofday () - time_val;

  ACE_DEBUG((LM_DEBUG, "connected to proxy_push_supplier, subscription latency = %d\n", time_val.sec () * 10000000 + time_val.usec ()* 10));
  return 0;
}


void
PushConsumer_impl::push (const RtecEventComm::EventSet & event
                         ACE_ENV_ARG_DECL_NOT_USED)
                         ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong x;
  ACE_Time_Value time_val = ACE_OS::gettimeofday ();

  if (event.length() >0) {
    TimeBase::TimeT elaps =
      time_val.sec () * 10000000 + time_val.usec ()* 10 - event[0].header.ec_send_time;
    event[0].data.any_value >>= x;

    if ( num_iterations_ > static_cast<int>(x) ) {
      run_times_[x] = static_cast<int>(elaps/10);
      TAO_FTRTEC::Log(3, "received event %d\n", x);
    }
    else {
      ACE_TRY {
        supplier_->disconnect_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCHANY {
      }
      ACE_ENDTRY;

      orb_->shutdown();
      output_result();
    }
  }
}


void
PushConsumer_impl::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
                       ACE_THROW_SPEC ((
                       CORBA::SystemException
                       ))
{
  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(orb_.in(), "POACurrent" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var poa = current->get_POA(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid = current->get_object_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object(oid ACE_ENV_ARG_PARAMETER);

}

void 
PushConsumer_impl::output_result()
{
  int lost = 0;
  for (int i =0; i < num_iterations_; ++i)
    if (run_times_[i] == -1) lost++;
    else
      ACE_DEBUG((LM_DEBUG, "%5d received, elapsed time = %d\n",i, run_times_[i]));
  ACE_DEBUG((LM_DEBUG, "%d events lost out of %d events\n", lost, num_iterations_));;
}


// $Id$

#include "PushConsumer.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include <stdio.h>
#include <vector>


ACE_RCSID (FtRtEvent,
           PushConsumer,
           "$Id$")

std::vector<int> run_times;



PushConsumer_impl::PushConsumer_impl(CORBA::ORB_ptr orb, int num_iterations)
: orb_(CORBA::ORB::_duplicate(orb)), num_iterations_(num_iterations)
{
	run_times.assign(num_iterations, -1);
}


void
PushConsumer_impl::push (const RtecEventComm::EventSet & event
             ACE_ENV_ARG_DECL_NOT_USED
             )
             ACE_THROW_SPEC ((
             CORBA::SystemException
             ))
{
  CORBA::ULong x;
  ACE_Time_Value time_val = ACE_OS::gettimeofday ();

  if (event.length() >0) {
    TimeBase::TimeT elaps =
      time_val.sec () * 10000000 + time_val.usec ()* 10 - event[0].header.ec_send_time;
    event[0].data.any_value >>= x;
	run_times[x] = static_cast<int>(elaps/10);

    if (num_iterations_-1 == x) {
      supplier_->disconnect_push_supplier();
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
    if (run_times[i] == -1) lost++;
    else
      ACE_DEBUG((LM_DEBUG, "%5d received, elapsed time = %d\n",i, run_times[i]));
  ACE_DEBUG((LM_DEBUG, "%d events lost out of %d events\n", lost, num_iterations_));;
}


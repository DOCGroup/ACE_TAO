/**
 * @file Low_Priority_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_LOW_PRIORITY_SETUP_CPP
#define TAO_PERF_RTEC_LOW_PRIORITY_SETUP_CPP

#include "Low_Priority_Setup.h"
#include "ace/Basic_Stats.h"

template<class Client_Type> Low_Priority_Setup<Client_Type>::
Low_Priority_Setup (int nthreads,
                    int iterations,
                    int use_different_types,
                    CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    int workload,
                    ACE_UINT32 gsf,
                    int enable_threads,
                    int thread_priority,
                    int thread_sched_class,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa,
                    RtecEventChannelAdmin::EventChannel_ptr ec,
                    ACE_Thread_Manager *thr_mgr,
                    ACE_Barrier *barrier
                    ACE_ENV_ARG_DECL)
  : nthreads_ (nthreads)
  , clients_ (nthreads ? new Client_Type[nthreads] : 0)
  , disconnect_ (nthreads ? new Client_Auto_Disconnect[nthreads] : 0)
  , tasks_ (nthreads ? new Send_Task[nthreads] : 0)
  , stoppers_ (nthreads ? new Auto_Send_Task_Stopper[nthreads] : 0)
{
  for (int i = 0; i != nthreads; ++i)
    {
      int per_consumer_workload =
        low_priority_workload / nthreads;
      if (per_consumer_workload == 0)
        per_consumer_workload = 1;

      CORBA::Long event_type =
        base_event_type;
      if (use_different_types)
        event_type = base_event_type + 2 * i;

      this->clients_[i].init (experiment_id,
                              event_type,
                              iterations,
                              per_consumer_workload,
                              gsf,
                              supplier_poa,
                              consumer_poa);
      this->clients_[i].connect (ec
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      /// Automatically disconnect the group if the connection was
      /// successful 
      this->disconnect_[i] = &this->clients_[i];

      if (enable_threads)
        {
          this->tasks_[i].init (iterations,
                                low_priority_period,
                                base_event_type,
                                1,
                                this->clients_[i].supplier (),
                                barrier);
          this->tasks_[i].thr_mgr (thr_mgr);
          this->stoppers_[i] = Auto_Send_Task_Stopper (
                  new Send_Task_Stopper (thread_priority,
                                         thread_sched_class,
                                         &this->tasks_[i])
                  );
        }
    }
}

template<class Client_Type> void
Low_Priority_Setup<Client_Type>::stop_all_threads (void)
{
  /// Resetting the auto_ptr<> destroys all the objects.  The
  /// destructors automatically stop and wait for all the threads.
  /// Depending on your personal bias this is either "super neat" or
  /// "a horrible kludge", IMHO is just good use of the language :-)
  this->stoppers_.reset (0);
}

template<class Client_Type> void
Low_Priority_Setup<Client_Type>::collect_basic_stats (ACE_Basic_Stats &stats)
{
  for (int i = 0; i != nthreads; ++i)
    {
      ACE_Sample_History &history =
        this->clients_[i].consumer ()->sample_history ();
      history.collect_basic_stats (stats);
    }
}

#endif /* TAO_PERF_RTEC_LOW_PRIORITY_SETUP_CPP */

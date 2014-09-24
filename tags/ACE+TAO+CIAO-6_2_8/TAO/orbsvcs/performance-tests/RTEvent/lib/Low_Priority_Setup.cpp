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
#include "ace/Sample_History.h"

template<class Client_Type> Low_Priority_Setup<Client_Type>::
Low_Priority_Setup (int consumer_count,
                    int iterations,
                    int use_different_types,
                    CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    int workload,
                    ACE_High_Res_Timer::global_scale_factor_type gsf,
                    int nthreads,
                    int thread_priority,
                    int thread_sched_class,
                    int per_thread_period,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa,
                    RtecEventChannelAdmin::EventChannel_ptr ec,
                    ACE_Barrier *barrier)
  : consumer_count_ (consumer_count)
  , clients_ (consumer_count ? new Client_Type[consumer_count] : 0)
  , disconnect_ (consumer_count ? new Client_Auto_Disconnect[consumer_count] : 0)
  , nthreads_ (nthreads)
  , tasks_ (nthreads ? new Send_Task[nthreads] : 0)
  , stoppers_ (nthreads ? new Auto_Send_Task_Stopper[nthreads] : 0)
{
  for (int i = 0; i != consumer_count; ++i)
    {
      int per_consumer_workload =
        workload / this->consumer_count_;
      if (workload != 0 && per_consumer_workload == 0)
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
      this->clients_[i].connect (ec);
      // Automatically disconnect the group if the connection was
      // successful
      this->disconnect_[i] = &this->clients_[i];
    }

  for (int j = 0; j != nthreads; ++j)
    {
      CORBA::Long event_type =
        base_event_type;
      if (use_different_types)
        event_type = base_event_type + 2 * j;

      this->tasks_[j].init (0,
                            per_thread_period,
                            j * per_thread_period,
                            event_type,
                            experiment_id,
                            this->clients_[j].supplier (),
                            barrier);
      this->tasks_[j].thr_mgr (&this->thr_mgr_);
      ACE_auto_ptr_reset (this->stoppers_[j],
                          new Send_Task_Stopper (thread_priority,
                                                 thread_sched_class,
                                                 &this->tasks_[j]));
    }
}

template<class Client_Type> void
Low_Priority_Setup<Client_Type>::stop_all_threads (void)
{
  ACE_DEBUG ((LM_DEBUG, "Stopping:"));
  for (int i = 0; i != this->nthreads_; ++i)
    {
      this->tasks_[i].stop ();
      ACE_DEBUG ((LM_DEBUG, " %d", i));
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));
  this->thr_mgr_.wait ();

  /// Resetting the auto_ptr<> destroys all the objects.  The
  /// destructors automatically stop and wait for all the threads.
  /// Depending on your personal bias this is either "super neat" or
  /// "a horrible kludge", IMHO is just good use of the language :-)
  this->stoppers_.reset (0);
}

template<class Client_Type> void
Low_Priority_Setup<Client_Type>::collect_basic_stats (ACE_Basic_Stats &stats)
{
  for (int i = 0; i != this->consumer_count_; ++i)
    {
      ACE_Sample_History &history =
        this->clients_[i].consumer ()->sample_history ();
      history.collect_basic_stats (stats);
    }
}

#endif /* TAO_PERF_RTEC_LOW_PRIORITY_SETUP_CPP */

/**
 * @file Low_Priority_Setup.h
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_LOW_PRIORITY_SETUP_H
#define TAO_PERF_RTEC_LOW_PRIORITY_SETUP_H

#include "Auto_Disconnect.h"
#include "Send_Task.h"
#include "Send_Task_Stopper.h"
#include "ace/High_Res_Timer.h"
#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RT_Class;

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Basic_Stats;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * @class Low_Priority_Setup
 *
 * @brief Simplify the initialization of performance tests clients
 */
template<class Client_Type>
class Low_Priority_Setup
{
public:
  /// Constructor
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
                      ACE_Barrier *barrier);

  /// Stop all running threads
  void stop_all_threads ();

  /// Collect the stats from all the clients
  void collect_basic_stats (ACE_Basic_Stats &stats);

  typedef std::unique_ptr<Client_Type[]> Client_Array;
  typedef Auto_Disconnect<Client_Type> Client_Auto_Disconnect;
  typedef std::unique_ptr<Client_Auto_Disconnect[]> Client_Auto_Disconnect_Array;
  typedef std::unique_ptr<Send_Task[]> Send_Task_Array;
  typedef std::unique_ptr<Send_Task_Stopper> Auto_Send_Task_Stopper;
  typedef std::unique_ptr<Auto_Send_Task_Stopper[]> Send_Task_Stopper_Array;

private:
  int consumer_count_;
  Client_Array clients_;
  Client_Auto_Disconnect_Array disconnect_;

  int nthreads_;
  Send_Task_Array tasks_;
  Send_Task_Stopper_Array stoppers_;
  ACE_Thread_Manager thr_mgr_;
};

#include "Low_Priority_Setup.cpp"

#endif /* TAO_PERF_RTEC_LOW_PRIORITY_SETUP_H */

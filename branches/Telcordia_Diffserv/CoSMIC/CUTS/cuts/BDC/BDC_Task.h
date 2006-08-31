// -*- C++ -*-

//=============================================================================
/**
 * @file    BDC_Task.h
 *
 * @brief   Active object for the Benchmark_Data_Collector component.
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BDC_TASK_H_
#define _CUTS_BDC_TASK_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_exec_export.h"
#include "CCM_Component_Registry.h"
#include "ace/Task_Ex_T.h"
#include "ace/Message_Queue_T.h"
#include "ace/Event.h"
#include "ace/Atomic_Op_T.h"
#include "ace/Time_Value.h"
#include "ace/streams.h"

// Forward decl.
class CUTS_System_Metric;

namespace CUTS
{
  // Forward decl.
  class Testing_Service_exec_i;

  //=========================================================================
  /**
   * @class   BDC_Task
   *
   * @brief   Active object for the Benchmark_Data_Collector.
   *
   * The BDC_Task object is the main logic of the BDC. It is responsible
   * for collecting all the metrics from each of the components in the
   * system. Once metrics are collected, it stores to metrics in a
   * temporary cache for that collection period. The BDC_Task can
   * be configured in a number of ways. For example, you set the number of
   * threads used to collect metrics at activation time. Also, the metics
   * stored in the cache can be dumped to an file on the same node as the
   * Benchmark_Data_Collector in XML format.
   */
  //=========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export BDC_Task :
    public Benchmark_Agent_Handler,
    public ACE_Task_Ex <ACE_MT_SYNCH, ::CUTS::Benchmark_Agent>
  {
  public:
    /**
     * Initializing contructor. The reference to the system metrics
     * objects is needed so the BDC_Task always an object to cache
     * its metrics.
     *
     * @param[in]      metrics   Reference to a system metrics object.
     */
    BDC_Task (void);

    /// Destructor.
    virtual ~BDC_Task (void);

    /// Set the testing service for the task.
    void testing_service (Testing_Service_exec_i * testing_service);

    /**
     * Activate the task. This will activate all the threads in
     * the BDC_Task. One thread however is responsible for handling
     * the timeout notification and getting all gathering all the
     * agents. The other threads, if applicable, are responsible for
     * collecting the performance metrics from the agents as the
     * thread gathering the agents delegates the work.
     */
    bool activate (void);

    /// Deactivate the task.
    void deactivate (void);

    /**
     * Get the timeout value for the task. The timeout value
     * is in the order of seconds.
     *
     * return         Number of seconds to wait per timeout.
     */
    long timeout (void) const;

    /**
     * Set the timeout value for the task. The timeout value
     * is in order of seconds.
     *
     * @param[in]     timeout     Number of seconds per collection.
     */
    void timeout (long timeout);

    /**
     * Set the number of threads used to collect the performance
     * metrics from all the components in the system.
     *
     * @param[in]     threads     Number of threads.
     */
    void collection_threads (size_t threads);

    /**
     * Get the number of threads used to collect the performance
     * metrics from all the components.
     *
     * @return      Number of threads.
     */
    size_t collection_threads (void) const;

    /**
     * Force the task to collect performance metrics for all to
     * components. This is a non-blocking method. When all the
     * metrics are finished collecting, the collect_done_event ()
     * is signaled.
     */
    void collect_data (void);

    /**
     * Get the name of the metrics output file.
     *
     * @return      Name of the metrics outfile.
     */
    const std::string & metrics_outfile (void) const;

    /**
     * Set the name of the metrics output file.
     *
     * @param[in]   filename    Name of the file.
     */
    void metrics_outfile (const std::string & filename);

    void wait_for_collection_done (void);

    void system_metrics (CUTS_System_Metric * metrics);

  protected:
    /// Storage location for the system metrics.
    CUTS_System_Metric * system_metrics_;

    /// Enqueues a benchmark agent for data collection.
    virtual int handle_agent (const char * instance,
                              ::CUTS::Benchmark_Agent_ptr agent);

    /// Input handler for the task.
    virtual int handle_input (ACE_HANDLE);

  private:
    /// Service handler for the task.
    int svc (void);

    /// Timeout handler for the task.
    int handle_timeout (const ACE_Time_Value & curr_time, const void * act);

    /// Helper method to descrement the count.
    int decrement_count (void);

    /// Helper method to write metrics to output file.
    void write_metrics_to_file (void);

    /// Flag containing the active state for the task.
    bool active_;

    /// Timeout value for the task.
    long timeout_;

    /// ID of the timer for the timeout.
    long timer_;

    /// Pointer to the testing service that contains the
    /// component registry for the test.
    Testing_Service_exec_i * testing_service_;

    /// Number of components remaining to collect.
    ACE_Atomic_Op <ACE_RW_Thread_Mutex, size_t> count_;

    /// Event that is signaled with the collection is complete.
    ACE_Event collection_done_;

    /// Output file for the performance metrics.
    ofstream outfile_;

    /// Number of threads used to collect the performance metrics.
    size_t collection_threads_;

    /// Name of the output file for the metrics.
    std::string metrics_outfile_;
  };
}

#if defined (__CUTS_INLINE__)
#include "BDC_Task.inl"
#endif

#include /**/ "ace/post.h"

#endif  // !defined _CUTS_BDC_TASK_H_

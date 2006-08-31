// $Id$

#include "BDC_Task.h"

#if !defined (__CUTS_INLINE__)
#include "BDC_Task.inl"
#endif

#include "Testing_Service_exec_i.h"
#include "cuts/Time_Metric.h"
#include "cuts/Port_Metric.h"
#include "cuts/Component_Metric.h"
#include "cuts/System_Metric.h"
#include "cuts/XML_System_Metrics_Visitor.h"
#include "cuts/IDL_Streams.h"
#include "ace/TP_Reactor.h"

namespace CUTS
{
  //
  // BDC_Task
  //
  BDC_Task::BDC_Task (void)
    : system_metrics_ (0),
      active_ (false),
      timeout_ (0),
      timer_ (-1),
      testing_service_ (0),
      count_ (0),
      collection_done_ (0, 0),
      collection_threads_ (1)
  {
    ACE_TP_Reactor * tp_reactor = 0;
    ACE_NEW (tp_reactor, ACE_TP_Reactor ());

    if (tp_reactor != 0)
    {
      ACE_Reactor * reactor = 0;
      ACE_NEW (reactor,
               ACE_Reactor (tp_reactor, 1));

      this->reactor (reactor);
    }
  }

  //
  // ~BDC_Task
  //
  BDC_Task::~BDC_Task (void)
  {
    delete this->reactor ();
    this->reactor (0);
  }

  //
  // activate
  //
  bool BDC_Task::activate (void)
  {
    if (this->timeout_ > 0)
    {
      ACE_Time_Value interval (this->timeout_);

      this->timer_ =
        this->reactor ()->schedule_timer (this, 0, interval, interval);
    }

    // Activate the task.
    this->active_ = true;

    ACE_Task_Base::activate (
      THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
      this->collection_threads_);

    return this->active_;
  }

  //
  // deactivate
  //
  void BDC_Task::deactivate (void)
  {
    this->active_ = false;

    // Cancel the <timer_>. This prevent any more data collection
    // from happening.
    if (this->timer_ != -1)
    {
      this->reactor ()->cancel_timer (this->timer_);
      this->timer_ = -1;
    }

    // Wait for all the threads to terminate.
    this->msg_queue_->deactivate ();
    this->wait ();
  }

  //
  // collect_data
  //
  void BDC_Task::collect_data (void)
  {
    if (this->testing_service_ == 0)
    {
      ACE_ERROR ((
        LM_ERROR,
        "[%M] -%T - no testing service present\n"));
    }
    else if (!this->active_)
    {
      ACE_ERROR ((
        LM_WARNING,
        "[%M] -%T - collection task not active\n"));
    }
    else if (this->count_ != 0)
    {
      ACE_ERROR ((
        LM_WARNING,
        "[%M] -%T - still collecting metrics from previous time\n"));
    }
    else
    {
      ACE_ERROR ((LM_DEBUG,
                  "[%M] -%T - requesting references to all benchmark agents\n"));

      // Update the timestamp fro the system metrics. This will
      // negate any metrics currently kept in the cache.
      this->system_metrics_->timestamp (ACE_OS::gettimeofday ());

      // Get all the benchmark agents contained in the testing service.
      CCM_Component_Registry * registry =
        this->testing_service_->registry_i ();

      size_t count =
        registry->get_benchmark_agents (this,
                                        this->count_.value_i ());

      // Apparently there wasn't any components in the registration
      // so we can go ahead and signal the waiting task.
      if (count == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "[%M] -%T - finished collecting performance metrics; "
                    "notifying waiting objects\n"));

        this->collection_done_.signal ();
      }
    }
  }

  //
  // svc
  //
  int BDC_Task::svc (void)
  {
    while (this->active_)
    {
      this->reactor ()->handle_events ();
    }

    return 0;
  }

  //
  // handle_timeout
  //
  int BDC_Task::handle_timeout (const ACE_Time_Value & curr_time,
                                const void * act)
  {
    this->collect_data ();
    return 0;

    ACE_UNUSED_ARG (curr_time);
    ACE_UNUSED_ARG (act);
  }

  //
  // handle_input
  //
  int BDC_Task::handle_input (ACE_HANDLE)
  {
    // Get the next agent from the message queue.
    ::CUTS::Benchmark_Agent * agent = 0;
    this->getq (agent);

    if (this->system_metrics_ != 0 && agent != 0)
    {
      try
      {
        // Get <performance_data> and store it in the cache.
        ::CUTS::Benchmark_Data_var data = agent->collect_performance_data ();
        data >> (*this->system_metrics_);
      }
      catch (const CORBA::Exception & ex)
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - %s\n",
                    ex._info ().c_str ()));
      }
      catch (...)
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - unknown exception occured\n"));
      }
    }

    return this->decrement_count ();
  }

  //
  // handle_agent
  //
  int BDC_Task::handle_agent (const char * instance,
                              ::CUTS::Benchmark_Agent_ptr agent)
  {
    int retval = 1;

    if (this->active_)
    {
      // Verify this is actual an agent connected to the testing
      // service. If the component was "preregistered" then the
      // agent reference will be NIL until it comes online.
      if (!::CORBA::is_nil (agent))
      {
        this->putq (agent);
        this->reactor ()->notify (this, ACE_Event_Handler::READ_MASK);
      }
      else
      {
        retval = this->decrement_count ();
      }
    }

    return retval;
  }

  //
  // decrement_count
  //
  int BDC_Task::decrement_count (void)
  {
    size_t remaining = 0;

    if (this->count_ > 0)
    {
      remaining = -- this->count_;
    }

    if (remaining == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - finished collecting performance metrics\n"));

      this->collection_done_.signal ();

      // Write the metrics to file.
      write_metrics_to_file ();
    }

    return remaining ? 1 : 0;
  }

  //
  // wait_for_collection_done
  //
  void BDC_Task::wait_for_collection_done (void)
  {
    this->collection_done_.wait ();
  }

  //
  // system_metrics
  //
  void BDC_Task::system_metrics (CUTS_System_Metric * metrics)
  {
    this->system_metrics_ = metrics;
  }

  //
  // write_metrics_to_file
  //
  void BDC_Task::write_metrics_to_file (void)
  {
    if (!this->metrics_outfile_.empty () &&
         this->system_metrics_)
    {
      // Open the output file for writing in append mode.
      // We do not want to overwrite any of the existing
      // data in the file.
      this->outfile_.open (this->metrics_outfile_.c_str (),
                           std::ios_base::app | std::ios_base::out);

      if (this->outfile_.is_open ())
      {
        CUTS_Component_Registry * registry =
          this->testing_service_->registry ();

        if (registry)
        {
          // Write the metrics to a file in XML format.
          CUTS_XML_System_Metrics_Visitor visitor (*registry, this->outfile_);

          this->system_metrics_->accept (visitor);
          this->outfile_.close ();

          ACE_DEBUG ((LM_DEBUG,
                      "[%M] -%T - finish writing metrics to file\n",
                      this->metrics_outfile_.c_str ()));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
                      "[%M] -%T - bad registry pointer\n"));
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - failed to open %s for writing\n",
                    this->metrics_outfile_.c_str ()));
      }
    }
  }
}

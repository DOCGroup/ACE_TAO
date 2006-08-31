// $Id$

#include "CUTS_exec.h"
#include "ciao/CIAO_common.h"
#include "cuts/Component_Metric.h"
#include "cuts/Port_Metric.h"
#include "cuts/Time_Metric.h"
#include "cuts/Time.h"

#include "ace/OS_NS_unistd.h"
#include "ace/TP_Reactor.h"
#include "ace/Log_Msg.h"

#include <strstream>

namespace CUTS
{
  //
  // operator << (CUTS_Time_Metric &, const Time_Sample &)
  //
  void operator << (CUTS_Time_Metric & time_metric,
                    const Time_Sample & time_sample)
  {
    // Calculate the average timing metric.
    long average = 0;

    if (time_sample.count > 0)
      average = time_sample.time.total / time_sample.count;

    // Update the timing information.
    time_metric.update (
      time_sample.count,
      average,
      time_sample.time.min,
      time_sample.time.max);
  }

  //
  //
  // operator << (CUTS_Port_Metric &, const Port_Measurement &)
  //
  void operator << (CUTS_Port_Metric & port_metric,
                    const Mapped_Port_Measurement & port_measurement)
  {
    do
    {
      // Save the transit time.
      ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, port_metric.lock ());
      port_metric.transit_time () << port_measurement.transit_time;
    } while (false);

    // Get all the exit times for this component.
    long length = port_measurement.exit_times.length ();

    for (long i = 0; i < length; i ++)
    {
      // Get the next <exit_point> from the collection.
      const Exit_Point_Time & exit_point = port_measurement.exit_times[i];

      // Locate the <endpoint> in the <port_metric>.
      CUTS_Time_Metric * time_metric =
        port_metric.endpoint (exit_point.exit_point_);

      if (time_metric)
      {
        // Store the <time_sample_> in the <time_metric>.
        (*time_metric) << exit_point.time_sample_;
      }
    }
  }

  //
  //
  //

  //
  // operator << (CUTS_System_Metric &, const Benchmark_Data_var &)
  //
  void operator << (CUTS_System_Metric & sys_metrics,
                    const Benchmark_Data_var & data)
  {
    // Get the <CUTS_Component_Metric> for this <data>. If one does not
    // exist it will create a new one, which should be a one time only
    // occurrence.
    CUTS_Component_Metric * metric =
      sys_metrics.component_metrics (data->owner);

    // Get the <ports> and the <length> of the <port> sequence.
    Port_Measurement_Seq ports = data->ports;
    long length = ports.length ();

    for (long i = 0; i < length; i ++)
    {
      // Get the next <port_measurement> in the collection.
      Port_Measurement & port_measurement = ports[i];

      long map_length = port_measurement.measurements.length ();

      for (long j = 0; j < map_length; j ++)
      {
        Mapped_Port_Measurement & mapped_measurement =
          port_measurement.measurements[j];

        CUTS_Port_Metric * port_metric =
          metric->port_metrics (
            port_measurement.port,
            mapped_measurement.owner);

        if (port_metric)
        {
          // Store the <port_measurements
          (*port_metric) << mapped_measurement;
        }
      }
    }
  }

  namespace CIDL_Benchmark_Data_Collector_Impl
  {
    //=========================================================================
    /*
     * BDC_Task implementation
     */
    //=========================================================================

    //
    // BDC_Task
    //
    BDC_Task::BDC_Task (void)
      : active_ (false),
        timeout_ (0),
        timer_ (-1),
        testing_service_ (0),
        count_ (0),
        collection_done_ (1)
    {
      // Create a new <reactor_>.
      ACE_Reactor * reactor = 0;
      reactor = new ACE_Reactor (new ACE_TP_Reactor (), 1);

      this->reactor (reactor);

      // Allocate a new strategy.
      ACE_Reactor_Notification_Strategy * strategy = 0;

      ACE_NEW (strategy,
               ACE_Reactor_Notification_Strategy (
               reactor, this, ACE_Event_Handler::READ_MASK));

      this->notify_strategy_.reset (strategy);

      // Attach the notification strategy to the <closed_list_>.
      this->bma_queue_.notification_strategy (strategy);
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
    // testing_service
    //
    void BDC_Task::testing_service (Testing_Service_exec_i * testing_service)
    {
      this->testing_service_ = testing_service;
    }

    //
    // activate
    //
    bool BDC_Task::activate (void)
    {
      if (this->timeout_ > 0)
      {
        // Schedule a timer with the <reactor>.
        ACE_Time_Value interval (this->timeout_);
        this->timer_ = this->reactor ()->schedule_timer (
          this, 0, interval, interval);
      }

      // Activate the task.
      this->active_ = true;

      ACE_Task_Base::activate (
        THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
        CUTS_BDC_THREAD_COUNT);

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
      this->reactor ()->notify (this);
      this->wait ();
    }

    //
    // timeout
    //
    long BDC_Task::timeout (void) const
    {
      return this->timeout_;
    }

    //
    // timeout
    //
    void BDC_Task::timeout (long timeout)
    {
      this->timeout_ = timeout;
    }

    //
    // collect_data
    //
    void BDC_Task::collect_data (void)
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered BDC_Task::collect_data\n"));

      // We can only handle the timeout if we have collected
      // all the data from the previous timeout, and we are
      // in the active state.
      if (this->testing_service_ &&
          this->active_ &&
          this->count_ == 0)
      {
        this->testing_service_->get_benchmark_agents (this, this->count_);
      }

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::collect_data\n"));
    }

    //
    // svc
    //
    int BDC_Task::svc (void)
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered BDC_Task::svc\n"));

      this->reactor ()->owner (ACE_OS::thr_self ());

      while (this->active_)
      {
        this->reactor ()->handle_events ();
      }

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::svc\n"));
      return 0;
    }

    //
    // handle_timeout
    //
    int BDC_Task::handle_timeout (const ACE_Time_Value & curr_time,
                                  const void * act)
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered BDC_Task::handle_timeout\n"));

      collect_data ();

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::handle_timeout\n"));
      return 0;
      ACE_UNUSED_ARG (curr_time);
      ACE_UNUSED_ARG (act);
    }

    //
    // collection_done_event
    //
    ACE_Event & BDC_Task::collection_done_event (void)
    {
      return this->collection_done_;
    }

    //
    // handle_input
    //
    int BDC_Task::handle_input (ACE_HANDLE handle)
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered BDC_Task::handle_input\n"));

      ::CUTS::Benchmark_Agent * agent = 0;
      this->bma_queue_.dequeue (agent);

      // Attach to the object since the method inserting the agent is giving
      // us the responsibility of destroying the object.
      if (this->active_)
      {
        try
        {
          // Get the performance data from the <agent>.
          ::CUTS::Benchmark_Data_var data = agent->collect_performance_data ();

          // We only need to cache the performance <data> locally
          // if we have a pointer to the <system_metrics_>.
          if (this->system_metrics_)
          {
            (*this->system_metrics_) << data;
          }
        }
        catch (const ::CORBA::Exception &)
        {
          ACE_ERROR ((LM_ERROR,
                      "[%M] -%T - unknown exception occured\n"));
        }
        catch (...)
        {
          ACE_ERROR ((LM_ERROR,
                      "[%M] -%T - unknown exception occured\n"));
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - failed to collect the data for agent\n"));
      }

      // Decrement the number of remaining agents to handle. If we
      // have collected all the data then we need to signal the
      // <collection_done_> event.
      if (-- this->count_ == 0)
      {
        this->collection_done_.signal ();
      }

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::handle_input\n"));
      return 0;

      ACE_UNUSED_ARG (handle);
    }

    //
    // handle_agent
    //
    void BDC_Task::handle_agent (::CUTS::Benchmark_Agent_ptr agent)
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered BDC_Task::handle_agent\n"));

      if (this->active_)
      {
        this->bma_queue_.enqueue (agent);
      }

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::handle_agent\n"));
    }

    //
    // system_metrics
    //
    void BDC_Task::system_metrics (CUTS_System_Metric * metrics)
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::system_metrics\n"));

      this->system_metrics_  = metrics;

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting BDC_Task::system_metrics\n"));
    }

    //=========================================================================
    /*
     * Path_Measurement_exec_i implementation
     */
    //=========================================================================

    //
    // Path_Measurement_exec_i
    //
    Path_Measurement_exec_i::Path_Measurement_exec_i (
      CUTS_System_Metric & system_metrics)
      : system_metrics_ (system_metrics),
        testing_service_ (0)
    {

    }

    //
    // ~Path_Measurement_exec_i
    //
    Path_Measurement_exec_i::~Path_Measurement_exec_i (void)
    {

    }

    //
    // bind_to_path
    //
    ::CORBA::Long Path_Measurement_exec_i::bind_to_path (
      const ::CUTS::Path_Sequence & path
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CUTS::Binding_Failed))
    {
      ACE_DEBUG ((
        LM_TRACE,
        "[%M] -%T - entered Path_Measurement_exec_i::bind_to_path\n"));

      long length = path.length ();

      // If there is not a path defined, or we do not have access to
      // the <testing_service_> then we should throw an exception. In
      // the future, the necessary information in the testing service
      // is going to be moved to a global location using a singleton.
      if (length == 0)
      {
        ACE_ERROR ((
          LM_ERROR,
          "[%M] -%T - length of path is zero\n"));

        throw ::CUTS::Binding_Failed ();
      }
      else if (this->testing_service_ == 0)
      {
        ACE_ERROR ((
          LM_ERROR,
          "[%M] -%T - testing_service_ == 0\n"));

        throw ::CUTS::Binding_Failed ();
      }

      Path_Definition * path_def = 0;

      ACE_NEW_THROW_EX (path_def,
                        Path_Definition (length * 2),
                        ::CUTS::Binding_Failed ());

      ACE_ERROR ((
        LM_ERROR,
        "[%M] -%T - creating a path with %u measurements\n",
        path_def->size ()));

      // This will clean up the memory just in case an exception
      // is thrown before we can insert the <path_def> into the
      // collection of <paths_>.
      ACE_Auto_Ptr <Path_Definition> auto_clean (path_def);

      // The following case is an optimization that will prevent
      // us from having a conditional statement inside the loop
      // to verify we are not using the "first" element. Since
      // we are only using the "first" element once, we can hard-
      // code its behavior outside the for loop.

      const Path_Element * element = &(path[0]);

      // Locate the element in the <paths_>.
      CUTS_Component_Metric * component_metric =
        this->system_metrics_.component_metrics (element->node);

      // Get the source port metrics.
      CUTS_Port_Metric * port_metric =
        component_metric->port_metrics (element->src);

      // Get the endpoing timing metrics.
      CUTS_Time_Metric * endpoint = port_metric->endpoint (element->dst);

      // Insert the time_metric into the temp store.
      (*path_def)[0] = (&port_metric->transit_time ());
      (*path_def)[1] = endpoint;

      // Save the <element> as the <prev_element>.
      const Path_Element * prev_element = element;

      for (long i = 1; i < length; i ++)
      {
        // Get the next path element.
        element = &(path[i]);

        // Locate the element in the <paths_>.
        CUTS_Component_Metric * component_metric =
          this->system_metrics_.component_metrics (element->node);

        // Get the source port metrics.
        long regid =
          this->testing_service_->get_registration_id (prev_element->node);

        ACE_DEBUG ((
          LM_DEBUG,
          "[%M] -%T - component %s has registration ID %u\n",
          prev_element->node,
          regid));

        CUTS_Port_Metric * port_metric =
          component_metric->port_metrics (element->src, regid);

        // Get the endpoing timing metrics.
        CUTS_Time_Metric * endpoint = port_metric->endpoint (element->dst);

        // Insert the time_metric into the temp store.
        int index = i * 2;
        (*path_def)[index] = (&port_metric->transit_time ());
        (*path_def)[index + 1]  = endpoint;

        // Save <element> as the <prev_element>.
        prev_element = element;
      }

      ACE_DEBUG ((
        LM_DEBUG,
        "[%M] -%T - inserting constructed path into the registry\n"));

      do
      {
        // Insert the newly created <path> into the collection.
        ACE_WRITE_GUARD_THROW_EX (
          ACE_RW_Thread_Mutex,
          guard,
          this->lock_,
          ::CUTS::Binding_Failed ());

        this->paths_.insert (path_def);
      } while (0);

      // Since we made it this far we did not throw any exceptions.
      // We can therefore release the memory and not destroy it.
      auto_clean.release ();

      ACE_DEBUG ((
        LM_TRACE,
        "[%M] -%T - exiting Path_Measurement_exec_i::bind_to_path\n"));

      return reinterpret_cast <long> (path_def);
    }

    //
    // unbind_to_path
    //
    void Path_Measurement_exec_i::unbind_to_path (
      ::CORBA::Long path_id ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      Path_Definition * path = reinterpret_cast <Path_Definition *> (path_id);

      do
      {
        ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->lock_);

        // Try and erase the <path> from the <paths_> collection.
        Path_Registry::iterator result = this->paths_.find (path);

        if (result != this->paths_.end ())
        {
          this->paths_.erase (result);
        }
        else
        {
          path = 0;
        }
      } while (false);

      // Delete the specified path if it exists.
      if (path != 0)
      {
        delete path;
      }
    }

    //
    // execution_time
    //
    ::CUTS::Time_Info Path_Measurement_exec_i::execution_time (
      ::CORBA::Long path_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CUTS::ID_Not_Found,
                       ::CUTS::Operation_Failed))
    {
      CUTS_Time_Metric time_metric;
      ::CUTS::Time_Info time_info;

      // Until we have implemented a sophisticated version of the
      // <Path_Registry> we have to protect against invalid id's.
      // Therefore, we have to located the reference in the
      // set the <paths_> to determine is validity.

      Path_Registry::iterator result;

      do {
        ACE_READ_GUARD_THROW_EX (
          ACE_RW_Thread_Mutex,
          guard,
          this->lock_,
          ::CUTS::Operation_Failed ());

        result =
          this->paths_.find (reinterpret_cast <Path_Definition *> (path_id));
      } while (false);

      if (result != this->paths_.end ())
      {
        // Since we found the path. We need to accumulate the
        // timing information along the path.
        Path_Definition::const_iterator path_iter;

        for (path_iter = (*result)->begin ();
             path_iter != (*result)->end ();
             path_iter ++)
        {
          time_metric += *(*path_iter);
        }
      }
      else
      {
        throw ::CUTS::ID_Not_Found ();
      }

      // Create a new <Time_Info> for the client to view its path
      // performance.
      time_info.total = time_metric.average_time ();
      time_info.max   = time_metric.worse_time ();
      time_info.min   = time_metric.best_time ();
      return time_info;
    }

    //
    // testing_service
    //
    void Path_Measurement_exec_i::testing_service (
      Testing_Service_exec_i * testing_service)
    {
      this->testing_service_ = testing_service;
    }

    //=========================================================================
    /*
     * BDC_Control_Handle_exec_i implementation
     */
    //=========================================================================

    //
    // BDC_Control_Handle_exec_i
    //
    BDC_Control_Handle_exec_i::BDC_Control_Handle_exec_i (BDC_Task & task)
      : task_ (task)
    {

    }

    //
    // ~BDC_Control_Handle_exec_i
    //
    BDC_Control_Handle_exec_i::~BDC_Control_Handle_exec_i (void)
    {

    }

    //
    // collect_performance_data
    //
    void BDC_Control_Handle_exec_i::collect_performance_data (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      ACE_DEBUG ((
        LM_TRACE,
        "[%M] -%T - entered BDC_Control_Handle_exec_i::collect_performance_data\n"));

      // Notify the <task_> to begin collection.
      this->task_.collect_data ();

      // Wait for signaling of the <collection_done_> event.
      this->task_.collection_done_event ().wait ();
//      this->task_.collection_done_event ().reset ();

      ACE_DEBUG ((
        LM_TRACE,
        "[%M] -%T - exiting BDC_Control_Handle_exec_i::collect_performance_data\n"));
    }

    //=========================================================================
    /*
     * Testing_Service_exec_i implementation
     */
    //=========================================================================

    //
    // Testing_Service_exec_i
    //
    Testing_Service_exec_i::Testing_Service_exec_i (BDC_Task & task)
      : task_ (task)
    {

    }

    //
    // ~Testing_Service_exec_i
    //
    Testing_Service_exec_i::~Testing_Service_exec_i (void)
    {
      for (Component_Registry::iterator iter = this->registry_.begin ();
           iter != this->registry_.end ();
           iter ++)
      {
        // Release the agent.
        if (!CORBA::is_nil ((*iter)->agent_))
        {
          ::CORBA::release ((*iter)->agent_);
        }

        // Delete the registration.
        delete (*iter);
      }
    }

    //
    // get_registration_id
    //
    long Testing_Service_exec_i::get_registration_id (const char * uuid)
    {
      ACE_READ_GUARD_RETURN (
        ACE_RW_Thread_Mutex,
        guard,
        this->registry_mutex_,
        -1);

      // Locate the registration id for the component. If the
      // registration cannot be found then we have to return a
      // value of -1. We do not want to throw an exception in
      // this case.
      for ( Component_Registry::iterator iter = this->registry_.begin ();
            iter != this->registry_.end ();
            iter ++)
      {
        if ((*iter)->uuid_ == uuid)
        {
          return reinterpret_cast <long> (*iter);
        }
      }

      return -1;
    }

    //
    // register_component
    //
    ::CORBA::Long Testing_Service_exec_i::register_component (
      const char * uuid, ::CUTS::Benchmark_Agent_ptr agent
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CUTS::Registration_Failed,
                       ::CUTS::Registration_Limit))
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered Testing_Service_exec_i::register_component\n"));

      // Create a new <Component_Registration>.
      Component_Registration * registration = 0;
      ACE_Auto_Ptr <Component_Registration> auto_clean;

      ACE_NEW_THROW_EX (registration,
                        Component_Registration,
                        ::CUTS::Registration_Failed ());
      auto_clean.reset (registration);

      // Initialize the elements in the structure.
      registration->uuid_ = uuid;
      registration->agent_ = ::CUTS::Benchmark_Agent::_duplicate (agent);

      // Insert the <registration> in the <registry_>.
      ACE_DEBUG ((LM_INFO,
                  "[%M ] -%T - registering component %s\n",
                  uuid));

      try
      {
        ACE_WRITE_GUARD_THROW_EX (
          ACE_RW_Thread_Mutex,
          guard,
          this->registry_mutex_,
          ::CUTS::Registration_Failed ());

        // Insert the <registration> into the <registry>.
        this->registry_.insert (registration);

        // We can releaes the <auto_clean> since there was
        // no exception thrown. This means that the allocated
        // memory was successfully inserted into the <registry>
        // and will be managed accordingly.
        auto_clean.release ();
      }
      catch (const ::CUTS::Registration_Failed &)
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - registration failed for %s\n",
                    uuid));

        // Re-throw the same exception.
        throw;
      }

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting Testing_Service_exec_i::register_component\n"));
      return reinterpret_cast <long> (registration);
    }

    //
    // unregister_component
    //
    void Testing_Service_exec_i::unregister_component (
      ::CORBA::Long regid ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException, ::CUTS::ID_Not_Found))
    {
      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - entered Testing_Service_exec_i::unregister_component\n"));

      if (regid != -1)
      {
        // Convert the <regid> to a <Component_Registration *>.
        Component_Registration * registration =
          reinterpret_cast <Component_Registration *> (regid);

        // Locate the <registration> in the <registry_>.
        Component_Registry::iterator iter;
        do
        {
          ACE_READ_GUARD (ACE_RW_Thread_Mutex, guard, this->registry_mutex_);
          iter = this->registry_.find (registration);
        } while (0);

        // If we were not able to find the registration in the <registry>
        // we can only assume that it's invalid. We therefore throw an
        // exception to warn the client.
        if (iter == this->registry_.end ())
        {
          ACE_ERROR ((LM_ERROR,
                      "[%M] -%T - failed to locate registration %X\n",
                      regid));
          throw ::CUTS::ID_Not_Found ();
        }

        ACE_DEBUG ((LM_INFO,
                    "[%M ] -%T - unregistering component %s\n",
                    (*iter)->uuid_.c_str ()));

        do
        {
          ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->registry_mutex_);

          // To complete release all the memory, we have to release the
          // reference to the <agent_> and delete the registration for
          // the component. Only then can we remove the entry from the
          // <registry_>.

          CORBA::release ((*iter)->agent_);
          delete (*iter);

          this->registry_.erase (iter);
        } while (0);
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - invalid registration id %X",
                    regid));
      }

      ACE_DEBUG ((LM_TRACE,
                  "[%M] -%T - exiting Testing_Service_exec_i::unregister_component\n"));
    }

    //
    // registered_components
    //
    size_t Testing_Service_exec_i::registered_components (void) const
    {
      return this->registry_.size ();
    }

    //
    // get_benchmark_agents
    //
    void Testing_Service_exec_i::get_benchmark_agents (
      Benchmark_Agent_Handler * callback,
      size_t & count)
    {
      ACE_READ_GUARD (
        ACE_RW_Thread_Mutex,
        guard,
        this->registry_mutex_)

      count = this->registry_.size ();

      for ( Component_Registry::iterator iter = this->registry_.begin ();
            iter != this->registry_.end ();
            iter ++)
      {
        callback->handle_agent ((*iter)->agent_);
      }
    }

    //=========================================================================
    /*
     * Benchmark_Data_Collector_exec_i implementation
     */
    //=========================================================================

    //
    // Benchmark_Data_Collector_exec_i
    //
    Benchmark_Data_Collector_exec_i::Benchmark_Data_Collector_exec_i (void)
      : context_ (0)
    {
      // Create the <BDC_Control_Handle>.
      BDC_Control_Handle_exec_i * control_handle;

      ACE_NEW (
        control_handle,
        BDC_Control_Handle_exec_i (this->task_));

      this->bdc_control_handle_.reset (control_handle);

      // Create the <Path_Measurement>. We have to give it the location
      // of the <system_metric_> for doing its calculations.
      Path_Measurement_exec_i * path_measurement = 0;

      ACE_NEW (
        path_measurement,
        Path_Measurement_exec_i (this->system_metric_));

      this->path_measurements_.reset (path_measurement);

      // Create the <Testing_Service_exec_i> object. This is used
      // by all the CoWorkErs to register themselves.
      Testing_Service_exec_i * testing_service = 0;

      ACE_NEW (
        testing_service,
        Testing_Service_exec_i (this->task_));

      this->testing_service_.reset (testing_service);

      // Attach the necessary handles to the <task_>.
      this->task_.system_metrics (&this->system_metric_);
      this->task_.testing_service (this->testing_service_.get ());

      // Attach the <testing_service_> to the <path_measurements_>.
      this->path_measurements_->testing_service (
        this->testing_service_.get ());
    }

    //
    // ~Benchmark_Data_Collector_exec_i
    //
    Benchmark_Data_Collector_exec_i::~Benchmark_Data_Collector_exec_i (void)
    {
      this->task_.system_metrics (0);
    }

    //
    // get_controls
    //
    ::CUTS::CCM_BDC_Control_Handle_ptr
      Benchmark_Data_Collector_exec_i::get_controls (void)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      return ::CUTS::CCM_BDC_Control_Handle::_duplicate (
        this->bdc_control_handle_.get ());
    }

    //
    // get_online_measurements
    //
    ::CUTS::CCM_Path_Measurement_ptr
      Benchmark_Data_Collector_exec_i::get_online_measurements (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      return CUTS::CCM_Path_Measurement::_duplicate (
        this->path_measurements_.get ());;
    }

    //
    // get_testing_service
    //
    ::CUTS::CCM_Testing_Service_ptr
      Benchmark_Data_Collector_exec_i::get_testing_service (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      return CUTS::CCM_Testing_Service::_duplicate (
        this->testing_service_.get ());
    }

    //
    // server_name
    //
    char * Benchmark_Data_Collector_exec_i::server_name (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return 0;
    }

    void Benchmark_Data_Collector_exec_i::server_name (
    const char * server_name
    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_UNUSED_ARG (server_name);
    }

    //
    // timeout
    //
    ::CORBA::Long
    Benchmark_Data_Collector_exec_i::timeout (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return this->task_.timeout ();
    }

    void
    Benchmark_Data_Collector_exec_i::timeout (
    ::CORBA::Long timeout
    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
    {
      this->task_.timeout (timeout);
    }

    //
    // set_session_context
    //
    void
    Benchmark_Data_Collector_exec_i::set_session_context (
      ::Components::SessionContext_ptr ctx ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
    {
      this->context_ =
        Benchmark_Data_Collector_Context::_narrow (ctx
        ACE_ENV_ARG_PARAMETER);

      ACE_CHECK;

      if (this->context_ == 0)
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
    }

    //
    // ciao_preactivate
    //
    void Benchmark_Data_Collector_exec_i::ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException))
    {

    }

    //
    // ccm_activate
    //
    void
    Benchmark_Data_Collector_exec_i::ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
    {
      ACE_DEBUG ((LM_TRACE,
        "[%M] -%T - entered Benchmark_Data_Collector_exec_i::ccm_activate\n"));

      if (this->testing_service_.get ())
      {
        ACE_DEBUG ((LM_INFO,
                    "[%M ] -%T - %u component(s) are registered\n",
                    this->testing_service_->registered_components ()));
      }

      ACE_DEBUG ((LM_TRACE,
        "[%M] -%T - exiting Benchmark_Data_Collector_exec_i::ccm_activate\n"));
    }

    //
    // ciao_postactivate
    //
    void
    Benchmark_Data_Collector_exec_i::ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
    {
      ACE_DEBUG ((LM_TRACE,
        "[%M] -%T - entered Benchmark_Data_Collector_exec_i::ciao_postactivate\n"));

      // Explicitly activate the task. In the future, there will be
      // an attribute that specifies if the task should be activated.
      this->task_.activate ();

      ACE_DEBUG ((LM_TRACE,
        "[%M] -%T - exiting Benchmark_Data_Collector_exec_i::ciao_postactivate\n"));
    }

    //
    // ccm_passivate
    //
    void
    Benchmark_Data_Collector_exec_i::ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException))
    {
      ACE_DEBUG ((LM_TRACE,
        "entered Benchmark_Data_Collector_exec_i::ccm_passivate\n"));

      // Deactivate the task.
      this->task_.deactivate ();

      ACE_DEBUG ((LM_TRACE,
        "exiting Benchmark_Data_Collector_exec_i::ccm_passivate\n"));
    }

    //
    // ccm_remove
    //
    void
    Benchmark_Data_Collector_exec_i::ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException))
    {

    }

    //=========================================================================
    /*
     * Benchmark_Data_Collector_Home_exec_i
     */
    //=========================================================================

    //
    // Benchmark_Data_Collector_Home_exec_i
    //
    Benchmark_Data_Collector_Home_exec_i::Benchmark_Data_Collector_Home_exec_i (void)
    {

    }

    //
    // ~Benchmark_Data_Collector_Home_exec_i
    //
    Benchmark_Data_Collector_Home_exec_i::~Benchmark_Data_Collector_Home_exec_i (void)
    {

    }

    //
    // create
    //
    ::Components::EnterpriseComponent_ptr
    Benchmark_Data_Collector_Home_exec_i::create (
      ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException))
    {
      ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

      ACE_NEW_THROW_EX (
        retval,
        Benchmark_Data_Collector_exec_i,
        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

      return retval;
    }

    //
    // createBenchmark_Data_Collector_Home_Impl
    //
    extern "C" CUTS_EXEC_Export ::Components::HomeExecutorBase_ptr
    createBenchmark_Data_Collector_Home_Impl (void)
    {
      ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

      ACE_NEW_RETURN (
        retval,
        Benchmark_Data_Collector_Home_exec_i,
        ::Components::HomeExecutorBase::_nil ());

      return retval;
    }
  }
}


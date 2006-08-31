// $Id$

#ifndef CIAO_CUTS_EXEC_H
#define CIAO_CUTS_EXEC_H

#include /**/ "ace/pre.h"

#include "CUTS_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CUTS_exec_export.h"
#include "tao/LocalObject.h"

#include "cuts/System_Metric.h"

#include "ace/Task.h"
#include "ace/Time_Value.h"
#include "ace/Message_Queue_T.h"
#include "ace/Reactor_Notification_Strategy.h"
#include "ace/Event.h"

#include <vector>
#include <set>
#include <map>

class CUTS_Time_Metric;

namespace CUTS
{
  class Benchmark_Agent_Handler
  {
  public:
    virtual void handle_agent (::CUTS::Benchmark_Agent_ptr agent) = 0;
  };

  namespace CIDL_Benchmark_Data_Collector_Impl
  {
    // forward declarations
    class Benchmark_Data_Collector_exec_i;
    class BDC_Control_Handle_exec_i;
    class Testing_Service_exec_i;

    //=========================================================================
    /**
     * @class BDC_Task
     */
    //=========================================================================

    class BDC_Task :
      public Benchmark_Agent_Handler,
      public ACE_Task_Base
    {
    public:
      /// Constructor.
      BDC_Task (void);

      /// Destructor.
      virtual ~BDC_Task (void);

      /// Set the collection point for the system metrics.
      void system_metrics (CUTS_System_Metric * metrics);

      /// Set the testing service for the task.
      void testing_service (Testing_Service_exec_i * testing_service);

      /// Activate the task.
      bool activate (void);

      /// Deactivate the task.
      void deactivate (void);

      /// Get the timeout value for the task.
      long timeout (void) const;

      /// Set the timeout value for the task.
      void timeout (long timeout);

      ACE_Event & collection_done_event (void);

      void collect_data (void);

    private:
      /// Service handler for the task.
      int svc (void);

      /// Enqueues a benchmark agent for data collection.
      void handle_agent (::CUTS::Benchmark_Agent_ptr agent);

      /// Timeout handler for the task.
      int handle_timeout (const ACE_Time_Value & curr_time, const void * act);

      /// Input handler for the task.
      int handle_input (ACE_HANDLE handle);

      /// Flag containing the active state for the task.
      bool active_;

      /// Timeout value for the task.
      long timeout_;

      /// ID of the timer for the timeout.
      long timer_;

      /// Storage location for the system metrics.
      CUTS_System_Metric * system_metrics_;

      /// Notification strategy for the <closed_list_>.
      ACE_Auto_Ptr <ACE_Reactor_Notification_Strategy> notify_strategy_;

      /// Collection of free activation records.
      ACE_Message_Queue_Ex <CUTS::Benchmark_Agent, ACE_MT_SYNCH> bma_queue_;

      Testing_Service_exec_i * testing_service_;

      size_t count_;

      ACE_Event collection_done_;
    };

    //=========================================================================
    /**
    * @class Path_Measurement_exec_i
    */
    //=========================================================================

    class CUTS_EXEC_Export Path_Measurement_exec_i :
      public virtual ::CUTS::CCM_Path_Measurement,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      Path_Measurement_exec_i (CUTS_System_Metric & metrics);

      virtual ~Path_Measurement_exec_i (void);

      virtual ::CORBA::Long bind_to_path (
        const ::CUTS::Path_Sequence & path
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::CUTS::Binding_Failed));

      virtual void unbind_to_path (
          ::CORBA::Long path_id
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((::CORBA::SystemException));

      virtual ::CUTS::Time_Info execution_time (
          ::CORBA::Long path_id
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((::CORBA::SystemException,
                           ::CUTS::ID_Not_Found,
                           ::CUTS::Operation_Failed));

      void testing_service (Testing_Service_exec_i * ts);

    private:
      /// Type definition for the path definitions.
      typedef std::vector <CUTS_Time_Metric *> Path_Definition;

      /// This is not optimal but will due for now!!!
      typedef std::set <Path_Definition *> Path_Registry;

      /// The system metrics.
      CUTS_System_Metric & system_metrics_;

      /// Data storgae for the registered paths.
      Path_Registry paths_;

      /// Locking mechanism for thread synchronization.
      ACE_RW_Thread_Mutex lock_;

      Testing_Service_exec_i * testing_service_;
    };

    //=========================================================================
    /**
     * @class BDC_Control_Handle_exec_i
     */
    //=========================================================================

    class CUTS_EXEC_Export BDC_Control_Handle_exec_i :
      public virtual ::CUTS::CCM_BDC_Control_Handle,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      /// Constructor.
      BDC_Control_Handle_exec_i (BDC_Task & task);

      /// Destructor.
      virtual ~BDC_Control_Handle_exec_i (void);

      virtual void
        collect_performance_data (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
      /// Pointer reference to the benchmark data collector.
      BDC_Task & task_;
    };

   //=========================================================================
    /*
     * Testing_Service_exec_i implementation
     */
    //=========================================================================

    class CUTS_EXEC_Export Testing_Service_exec_i
      : public virtual ::CUTS::CCM_Testing_Service,
        public virtual TAO_Local_RefCounted_Object
    {
    public:
      Testing_Service_exec_i (BDC_Task & task);

      virtual ~Testing_Service_exec_i (void);

      virtual ::CORBA::Long register_component (
        const char * uid, ::CUTS::Benchmark_Agent_ptr agent
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::CUTS::Registration_Failed,
                         ::CUTS::Registration_Limit));

      virtual void unregister_component (
        ::CORBA::Long regid ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::CUTS::ID_Not_Found));

      /// Get the number of registered components.
      size_t registered_components (void) const;

      /// Get all the benchmark agents from the registered components.
      void get_benchmark_agents (
        Benchmark_Agent_Handler * callback,
        size_t & count);

      long get_registration_id (const char * uuid);

    private:
      struct Component_Registration
      {
        /// ID of the registered component.
        std::string uuid_;

        /// Benchmark agent for the registered component.
        CUTS::Benchmark_Agent_ptr agent_;
      };

      ACE_RW_Thread_Mutex registry_mutex_;

      /// Type definition for the component registry.
      typedef std::set <Component_Registration *> Component_Registry;

      /// Component registration for the service.
      Component_Registry registry_;

      /// Task associated with the testing service.
      BDC_Task & task_;
    };

    //=========================================================================
    /**
     * @class Benchmark_Data_Collector_exec_i
     */
    //=========================================================================

    class CUTS_EXEC_Export Benchmark_Data_Collector_exec_i
    : public virtual Benchmark_Data_Collector_Exec,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      Benchmark_Data_Collector_exec_i (void);

      virtual ~Benchmark_Data_Collector_exec_i (void);

      virtual char *
        server_name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void
        server_name (const char * ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual ::CORBA::Long
        timeout (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void
        timeout (::CORBA::Long ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual ::CUTS::CCM_BDC_Control_Handle_ptr
        get_controls (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

      virtual ::CUTS::CCM_Path_Measurement_ptr
        get_online_measurements (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

      virtual ::CUTS::CCM_Testing_Service_ptr
        get_testing_service (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

      virtual void
        set_session_context (::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));

      virtual void
      ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));

      virtual void
      ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));

      virtual void
      ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));

      virtual void
      ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));

      virtual void
      ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));

    protected:
      /// Context for this object.
      Benchmark_Data_Collector_Context *context_;

    private:
      ACE_Auto_Ptr <BDC_Control_Handle_exec_i> bdc_control_handle_;

      ACE_Auto_Ptr <Path_Measurement_exec_i> path_measurements_;

      ACE_Auto_Ptr <Testing_Service_exec_i> testing_service_;
      BDC_Task task_;

      CUTS_System_Metric system_metric_;
    };

    //=========================================================================
    /**
     * @class Benchmark_Data_Collector_Home_exec_i
     */
    //=========================================================================

    class CUTS_EXEC_Export Benchmark_Data_Collector_Home_exec_i
      : public virtual Benchmark_Data_Collector_Home_Exec,
        public virtual TAO_Local_RefCounted_Object
    {
    public:
      /// Constructor.
      Benchmark_Data_Collector_Home_exec_i (void);

      /// Destructor.
      virtual ~Benchmark_Data_Collector_Home_exec_i (void);

      /// Factory operation for creating the component.
      virtual ::Components::EnterpriseComponent_ptr
      create (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::Components::CCMException));
    };

    extern "C" CUTS_EXEC_Export ::Components::HomeExecutorBase_ptr
    createBenchmark_Data_Collector_Home_Impl (void);
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_CUTS_EXEC_H */


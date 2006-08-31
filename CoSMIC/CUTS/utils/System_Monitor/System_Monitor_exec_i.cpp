// $Id$

#include "System_Monitor_exec_i.h"
#include "ciao/CIAO_common.h"
#include "ace/Reactor.h"

namespace CUTS
{
  namespace CIDL_System_Monitor_Impl
  {
    //==================================================================
    // Component Executor Implementation Class:   System_Monitor_exec_i
    //==================================================================

    //
    // System_Monitor_exec_i
    //
    System_Monitor_exec_i::System_Monitor_exec_i (void)
      : context_ (0),
        active_ (false),
        task_ (active_),
        timer_ (-1),
        timeout_ (0),
        path_id_ (0)
    {

    }

    //
    // ~System_Monitor_exec_i
    //
    System_Monitor_exec_i::~System_Monitor_exec_i (void)
    {

    }

    //
    // timeout
    //
    ::CORBA::Long System_Monitor_exec_i::timeout (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return this->timeout_;
    }

    //
    // timeout
    //
    void System_Monitor_exec_i::timeout (
      ::CORBA::Long timeout
      ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      this->timeout_ = timeout;
    }

    //
    // set_session_context
    //
    void System_Monitor_exec_i::set_session_context (
      ::Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      this->context_ =
        System_Monitor_Context::_narrow (ctx ACE_ENV_ARG_PARAMETER);

      ACE_CHECK;

      if (this->context_ == 0)
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
    }

    //
    // ciao_preactivate
    //
    void System_Monitor_exec_i::ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      // Activate the <timer_> in the <task_>.
      ACE_Time_Value timeout;
      timeout.msec (this->timeout_);

      this->timer_ = this->task_.reactor ()->schedule_timer (
        this,
        0,
        timeout,
        timeout);

      if (this->timer_ != -1)
      {
        ACE_DEBUG ((
          LM_INFO,
          "[%M] -%T - system monitor timer scheduled at %u msecs\n",
          this->timeout_));
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - failed to schedule monitor timer\n"));
      }
    }

    //
    // ciao_postactivate
    //
    void System_Monitor_exec_i::ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      // Get the performance metrics of the path of interest.
      ::CUTS::Path_Measurement_var system_measurements =
        this->context_->get_connection_system_measurements ();

      if (!::CORBA::is_nil (system_measurements.in ()))
      {
        // Register the path w/ the data collector.
        CUTS::Path_Sequence path (6);
        path.length (6);

        path[0].node =
          CORBA::string_dup ("SLICE.ComponentImplementations.SLICE.SLICE.Planner_Three");
        path[0].src = CORBA::string_dup ("command");
        path[0].dst = CORBA::string_dup ("command");

        path[1].node =
          CORBA::string_dup ("SLICE.ComponentImplementations.SLICE.SLICE.EnvDetector1");
        path[1].src = CORBA::string_dup ("command");
        path[1].dst = CORBA::string_dup ("track");

        path[2].node =
          CORBA::string_dup ("SLICE.ComponentImplementations.SLICE.SLICE.Planner_Three");
        path[2].src = CORBA::string_dup ("track");
        path[2].dst = CORBA::string_dup ("situation");

        path[3].node =
          CORBA::string_dup ("SLICE.ComponentImplementations.SLICE.SLICE.Planner_One");
        path[3].src = CORBA::string_dup ("situation");
        path[3].dst = CORBA::string_dup ("assessment");

        path[4].node =
          CORBA::string_dup ("SLICE.ComponentImplementations.SLICE.SLICE.Config_Op");
        path[4].src = CORBA::string_dup ("assessment");
        path[4].dst = CORBA::string_dup ("command");

        path[5].node =
          CORBA::string_dup ("SLICE.ComponentImplementations.SLICE.SLICE.Effector_Main");
        path[5].src = CORBA::string_dup ("command");
        path[5].dst = CORBA::string_dup ("command");

        try
        {
          ACE_DEBUG ((LM_DEBUG, "[%M] -%T -  registering path of interest"));

          this->path_id_ = system_measurements->bind_to_path (
            path,
            ::CUTS::Path_Measurement::BPF_BOTH);

          ACE_DEBUG ((
            LM_DEBUG,
            "[%M] -%T -  succesfully registered path; id = %u\n",
            this->path_id_));
        }
        catch (const CUTS::Binding_Failed &)
        {
          ACE_ERROR ((LM_ERROR, "[%M] -%T - failed to bind to path\n"));
        }
        catch (...)
        {
          ACE_ERROR ((LM_ERROR, "[%M] -%T - caught unknown exception\n"));
        }
      }
    }

    //
    // ccm_activate
    //
    void System_Monitor_exec_i::ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      ACE_DEBUG ((LM_DEBUG, "[%M] -%T - activating the system monitor\n"));

      this->active_ = true;
      this->task_.activate ();
    }

    //
    // ccm_passivate
    //
    void System_Monitor_exec_i::ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      if (this->path_id_ != 0)
      {
        // Unregister the <path_id_>.
        ::CUTS::Path_Measurement_var system_measurements =
          this->context_->get_connection_system_measurements ();

        if (!::CORBA::is_nil (system_measurements.in ()))
        {
          try
          {
            ACE_DEBUG ((
              LM_DEBUG,
              "[%M] -%T - unregistering the path with id %u\n",
              this->path_id_));

            system_measurements->unbind_to_path (this->path_id_);
            this->path_id_ = 0;
          }
          catch (const CUTS::ID_Not_Found &)
          {
            ACE_ERROR ((
              LM_ERROR,
              "[%M] -%T - path id %u does not exist\n",
              this->path_id_));

            this->path_id_ = 0;
          }
          catch (...)
          {
            ACE_ERROR ((LM_ERROR, "[%M] -%T - caught unknown exception\n"));
          }
        }
      }

      // Cancel the <timer_> in the <task_>. This will prevent the
      // handle_timeout () method from being invoked.
      if (this->timer_ != -1)
      {
        this->reactor ()->cancel_timer (this->timer_);
      }

      // If the task is <active_> then we need to end the
      // task and wait for all the threads to exit.
      if (this->active_)
      {
        this->active_ = false;

        this->task_.reactor ()->notify (this);
        this->task_.wait ();
      }
    }

    //
    // ccm_remove
    //
    void System_Monitor_exec_i::ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {

    }

    //
    // handle_timeout
    //
    int System_Monitor_exec_i::handle_timeout (
      const ACE_Time_Value & ,
      const void * )
    {
      if (this->context_ != 0)
      {
        ::CUTS::BDC_Control_Handle_var controls =
          this->context_->get_connection_control_handle ();

        if (!CORBA::is_nil (controls.in ()))
        {
          try
          {
            // Collect the performance data.
            controls->collect_performance_data ();

            // Get the performance metrics of the path of interest.
            ::CUTS::Path_Measurement_var paths =
              this->context_->get_connection_system_measurements ();

            if (this->path_id_ != 0 && !CORBA::is_nil (paths.in ()))
            {
              // Get the execution time of the path.
              CUTS::Event_Time_Info ti = paths->execution_time (this->path_id_);

              ACE_DEBUG ((
                LM_INFO,
                "[%M] -%T - Best Time: %u; Average Time: %u; Worse Time: %u\n",
                ti.time.min,
                ti.time.total,
                ti.time.max));
            }
          }
          catch (...)
          {

          }
        }
        else
        {
          ACE_DEBUG ((
            LM_INFO,
            "[%M] -%T - the monitor is not connected to a "
            "BDC control handle\n"));
        }
      }
      else
      {
        ACE_DEBUG ((
          LM_ERROR,
          "[%M] -%T - system monitors doe not have a context\n"));
      }

      return 0;
    }

    //==================================================================
    // Home Executor Implementation Class:   System_Monitor_Home_exec_i
    //==================================================================

    System_Monitor_Home_exec_i::System_Monitor_Home_exec_i (void)
    {
    }

    System_Monitor_Home_exec_i::~System_Monitor_Home_exec_i (void)
    {
    }

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    ::Components::EnterpriseComponent_ptr
    System_Monitor_Home_exec_i::create (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException))
    {
      ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

      ACE_NEW_THROW_EX (
        retval,
        System_Monitor_exec_i,
        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

      return retval;
    }

    extern "C" SYSTEM_MONITOR_EXEC_Export ::Components::HomeExecutorBase_ptr
    create_CUTS_System_Monitor_Home_Impl (void)
    {
      ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

      ACE_NEW_RETURN (
        retval,
        System_Monitor_Home_exec_i,
        ::Components::HomeExecutorBase::_nil ());

      return retval;
    }
  }
}


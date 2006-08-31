// $Id$

#include "Benchmark_Data_Collector_exec_i.h"
#include "BDC_Control_Handle_exec_i.h"
#include "BDC_Task.h"
#include "Testing_Service_exec_i.h"

namespace CUTS
{
  namespace CIDL_Benchmark_Data_Collector_Impl
  {
    //
    // Benchmark_Data_Collector_exec_i
    //
    Benchmark_Data_Collector_exec_i::Benchmark_Data_Collector_exec_i (void)
    {
      // Create a new <Testing_Service_Ex_exec_i> object.
      Testing_Service_exec_i * testing_service = 0;

      ACE_NEW_THROW_EX (testing_service,
                        Testing_Service_exec_i (),
                        CORBA::NO_MEMORY ());
      this->testing_service (testing_service);

      // Create the default active object and store it in the
      // implementation for the BDC.
      BDC_Task * task = 0;

      ACE_NEW_THROW_EX (task,
                        BDC_Task (),
                        CORBA::NO_MEMORY ());

      task->testing_service (testing_service);
      this->task (task);

      // Create a new <BDC_Control_Handle_exec_i> object and store it
      // in the implemenation.
      BDC_Control_Handle_exec_i * handle = 0;

      ACE_NEW_THROW_EX (handle,
                        BDC_Control_Handle_exec_i (),
                        CORBA::NO_MEMORY ());

      handle->task (task);
      this->control_handle (handle);
    }

    //
    // ~Benchmark_Data_Collector_exec_i
    //
    Benchmark_Data_Collector_exec_i::~Benchmark_Data_Collector_exec_i (void)
    {

    }

    //
    // set_session_context
    //
    void Benchmark_Data_Collector_exec_i::set_session_context (
      ::Components::SessionContext_ptr ctx ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException))
    {
      this->context_ =
        Benchmark_Data_Collector_Context::_narrow (
        ctx
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
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {

    }

    //
    // ciao_postactivate
    //
    void Benchmark_Data_Collector_exec_i::ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {

    }

    //
    // ccm_activate
    //
    void Benchmark_Data_Collector_exec_i::ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - activating the data collector\n"));

      this->task ()->activate ();
    }

    //
    // ccm_passivate
    //
    void Benchmark_Data_Collector_exec_i::ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - deactivating the data collector\n"));

      this->task ()->deactivate ();
    }

    //
    // ccm_remove
    //
    void Benchmark_Data_Collector_exec_i::ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Components::CCMException))
    {

    }

    //==================================================================
    // Benchmark_Data_Collector_Home_exec_i
    //==================================================================

    Benchmark_Data_Collector_Home_exec_i::Benchmark_Data_Collector_Home_exec_i (void)
    {

    }

    Benchmark_Data_Collector_Home_exec_i::~Benchmark_Data_Collector_Home_exec_i (void)
    {

    }

    ::Components::EnterpriseComponent_ptr
    Benchmark_Data_Collector_Home_exec_i::create (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
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

    extern "C" BENCHMARK_DATA_COLLECTOR_EXEC_Export
      ::Components::HomeExecutorBase_ptr
      create_CUTS_Benchmark_Data_Collector_Home_Impl (void)
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

// $Id$

#include "Benchmark_Data_Collector_Ex_exec_i.h"
#include "BDC_Control_Handle_exec_i.h"
#include "BDC_Task_Ex.h"
#include "Testing_Service_Ex_exec_i.h"
#include "Database_Service_exec_i.h"

namespace CUTS
{
  namespace CIDL_Benchmark_Data_Collector_Ex_Impl
  {
    //=========================================================================
    /*
     * Benchmark_Data_Collector_Ex_exec_i
     */
    //=========================================================================

    //
    // Benchmark_Data_Collector_Ex_exec_i
    //
    Benchmark_Data_Collector_Ex_exec_i::Benchmark_Data_Collector_Ex_exec_i (void)
    {
      // Create a new <Testing_Service_Ex_exec_i> object.
      Testing_Service_Ex_exec_i * testing_service = 0;

      ACE_NEW_THROW_EX (
        testing_service,
        Testing_Service_Ex_exec_i (this->database_service_),
        CORBA::NO_MEMORY ());

      this->testing_service (testing_service);

      // Create the default active object and store it in the
      // implementation for the BDC.
      BDC_Task_Ex * task = 0;

      ACE_NEW_THROW_EX (task,
                        BDC_Task_Ex (this->database_service_),
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
    // ~Benchmark_Data_Collector_Ex_exec_i
    //
    Benchmark_Data_Collector_Ex_exec_i::~Benchmark_Data_Collector_Ex_exec_i (void)
    {

    }

    //
    // get_database_service
    //
    CUTS::CCM_Database_Service_ptr
      Benchmark_Data_Collector_Ex_exec_i::get_database_service (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      CUTS::Database_Service_exec_i * dbsvc = 0;

      ACE_NEW_THROW_EX (
        dbsvc,
        CUTS::Database_Service_exec_i (this->database_service_),
        CORBA::NO_MEMORY ());

      return dbsvc;
    }

    //
    // server_name
    //
    char * Benchmark_Data_Collector_Ex_exec_i::server_name (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      CORBA::String_var str =
        CORBA::string_dup (this->server_name_.c_str ());

      return str._retn ();
    }

    //
    // server_name
    //
    void Benchmark_Data_Collector_Ex_exec_i::server_name (
      const char * server_name ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException))
    {
      if (this->database_service_.connect (CUTS_USERNAME,
                                           CUTS_PASSWORD,
                                           server_name,
                                           CUTS_DEFAULT_PORT))
      {
        // Store server name and create a new test.
        this->server_name_ = server_name;
        this->database_service_.create_new_test ();
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - failed to connect to database on %s; "
                    "[username = %s; password = %s; port = %u]\n",
                    server_name,
                    CUTS_USERNAME,
                    CUTS_PASSWORD,
                    CUTS_DEFAULT_PORT));
      }
    }

    //
    // set_session_context
    //
    void Benchmark_Data_Collector_Ex_exec_i::set_session_context (
      ::Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException))
    {
      this->context_ =
        Benchmark_Data_Collector_Ex_Context::_narrow (
        ctx ACE_ENV_ARG_PARAMETER);

      ACE_CHECK;

      if (this->context_ == 0)
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
    }

    //
    // ciao_preactivate
    //
    void Benchmark_Data_Collector_Ex_exec_i::ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
      ::Components::CCMException))
    {

    }

    //
    // ccm_activate
    //
    void Benchmark_Data_Collector_Ex_exec_i::ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
      ::Components::CCMException))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - activating the data collection task\n"));

      this->task ()->activate ();
    }

    //
    // ciao_postactivate
    //
    void Benchmark_Data_Collector_Ex_exec_i::ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
    {

    }

    //
    // ccm_passivate
    //
    void Benchmark_Data_Collector_Ex_exec_i::ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
      ::Components::CCMException))
    {
      // Deactivate the base task.
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - deactivating the data collection task\n"));

      this->task ()->deactivate ();
    }

    //
    // ccm_remove
    //
    void Benchmark_Data_Collector_Ex_exec_i::ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
      ::Components::CCMException))
    {
      // Stop the current test.
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - stopping current test\n"));
      this->database_service_.stop_current_test ();

      // Disconnect from the database.
      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - disconnecting from database service\n"));
      this->database_service_.disconnect ();
    }

    //=========================================================================
    /*
     * Benchmark_Data_Collector_Ex_Home_exec_i
     */
    //=========================================================================

    //
    // Benchmark_Data_Collector_Ex_Home_exec_i
    //
    Benchmark_Data_Collector_Ex_Home_exec_i::
      Benchmark_Data_Collector_Ex_Home_exec_i (void)
    {

    }

    //
    // ~Benchmark_Data_Collector_Ex_Home_exec_i
    //
    Benchmark_Data_Collector_Ex_Home_exec_i::
      ~Benchmark_Data_Collector_Ex_Home_exec_i (void)
    {

    }

    //
    // create
    //
    ::Components::EnterpriseComponent_ptr
      Benchmark_Data_Collector_Ex_Home_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException))
    {
      ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

      ACE_NEW_THROW_EX (
        retval,
        Benchmark_Data_Collector_Ex_exec_i,
        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

      return retval;
    }

    //=========================================================================
    /*
     * create_CUTS_Benchmark_Data_Collector_Ex_Home_Impl
     */
    //=========================================================================

    extern "C" BENCHMARK_DATA_COLLECTOR_EX_EXEC_Export
      ::Components::HomeExecutorBase_ptr
      create_CUTS_Benchmark_Data_Collector_Ex_Home_Impl (void)
    {
      ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

      ACE_NEW_RETURN (
        retval,
        Benchmark_Data_Collector_Ex_Home_exec_i,
        ::Components::HomeExecutorBase::_nil ());

      return retval;
    }
  }
}

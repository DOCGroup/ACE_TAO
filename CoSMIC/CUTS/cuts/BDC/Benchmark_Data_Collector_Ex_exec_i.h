// -*- C++ -*-

#ifndef _BENCHMARK_DATA_COLLECTOR_EX_EXEC_H_
#define _BENCHMARK_DATA_COLLECTOR_EX_EXEC_H_


#include /**/ "ace/pre.h"

#include "Benchmark_Data_Collector_Ex_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_Ex_exec_export.h"
#include "Benchmark_Data_Collector_Impl.h"
#include "cuts/svcs/dbase/DBase_Service.h"

namespace CUTS
{
  class Database_Service_exec_i;

  namespace CIDL_Benchmark_Data_Collector_Ex_Impl
  {
    //=========================================================================
    /**
     * @class Benchmark_Data_Collector_exec_i
     */
    //=========================================================================

    class BENCHMARK_DATA_COLLECTOR_EX_EXEC_Export Benchmark_Data_Collector_Ex_exec_i :
      public virtual Benchmark_Data_Collector_Ex_Exec,
      public virtual Benchmark_Data_Collector_Impl
    {
    public:
      /// Constructor.
      Benchmark_Data_Collector_Ex_exec_i (void);

      /// Destructor.
      virtual ~Benchmark_Data_Collector_Ex_exec_i (void);

      /// Get the database service interface.
      virtual CUTS::CCM_Database_Service_ptr
        get_database_service (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Get the name of the logging server.
      virtual char *
        server_name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Set the name of the logging server.
      virtual void
        server_name (const char * ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void
        set_session_context (::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

    protected:
      Benchmark_Data_Collector_Ex_Context * context_;

    private:
      /// Name of the logging server.
      std::string server_name_;

      /// The database service for the object.
      CUTS_Database_Service database_service_;
    };

    //=========================================================================
    /**
     * @class Benchmark_Data_Collector_Home_exec_i
     */
    //=========================================================================

    class BENCHMARK_DATA_COLLECTOR_EX_EXEC_Export Benchmark_Data_Collector_Ex_Home_exec_i :
      public virtual Benchmark_Data_Collector_Ex_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      Benchmark_Data_Collector_Ex_Home_exec_i (void);

      virtual ~Benchmark_Data_Collector_Ex_Home_exec_i (void);

      virtual ::Components::EnterpriseComponent_ptr
        create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException));
    };

    extern "C" BENCHMARK_DATA_COLLECTOR_EX_EXEC_Export
      ::Components::HomeExecutorBase_ptr
      create_CUTS_Benchmark_Data_Collector_Ex_Home_Impl (void);
  }
}

#endif  // !defined _BENCHMARK_DATA_COLLECTOR_EX_EXEC_H_

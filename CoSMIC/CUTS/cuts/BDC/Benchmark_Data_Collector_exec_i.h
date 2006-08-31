// -*- C++ --

#ifndef CIAO_BENCHMARK_DATA_COLLECTOR_EXEC_H
#define CIAO_BENCHMARK_DATA_COLLECTOR_EXEC_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_Impl.h"
#include "Benchmark_Data_Collector_svnt.h"
#include "tao/LocalObject.h"

namespace CUTS
{
  class BDC_Task;

  namespace CIDL_Benchmark_Data_Collector_Impl
  {
    //=========================================================================
    /**
     * @class Benchmark_Data_Collector_exec_i
     *
     * @brief Executor implementation of the Benchmark_Data_Collector
     * component.
     */
    //=========================================================================

    class BENCHMARK_DATA_COLLECTOR_EXEC_Export Benchmark_Data_Collector_exec_i :
      public virtual Benchmark_Data_Collector_Exec,
      public virtual Benchmark_Data_Collector_Impl
    {
    public:
      /// Constructor.
      Benchmark_Data_Collector_exec_i (void);

      /// Destructor.
      virtual ~Benchmark_Data_Collector_exec_i (void);

      virtual void
        set_session_context (Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CCMException));

      virtual void
        ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CCMException));

      virtual void
        ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CCMException));

      virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CCMException));

      virtual void
        ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CCMException));

      virtual void
        ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::CCMException));

    protected:
      /// Pointer to it context.
      Benchmark_Data_Collector_Context * context_;
    };

    //=========================================================================
    /**
     * @class Benchmark_Data_Collector_Home_exec_i
     *
     * @brief Implemenation of the Benchmark_Data_Collector_Home
     */
    //=========================================================================

    class BENCHMARK_DATA_COLLECTOR_EXEC_Export Benchmark_Data_Collector_Home_exec_i
    : public virtual Benchmark_Data_Collector_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      /// Constructor.
      Benchmark_Data_Collector_Home_exec_i (void);

      /// Destructor.
      virtual ~Benchmark_Data_Collector_Home_exec_i (void);

      /**
       * Creation method.
       *
       * @return Newly created Benchmark_Data_Collector component.
       */
      virtual ::Components::EnterpriseComponent_ptr
        create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException));
    };

    //=========================================================================
    /**
     * Entry point for the Benchmark_Data_Collector executor.
     *
     * @return Reference to the factory object for creating the
     * Benchmark_Data_Collector component.
     */
    //=========================================================================

    extern "C" BENCHMARK_DATA_COLLECTOR_EXEC_Export
      ::Components::HomeExecutorBase_ptr
      create_CUTS_Benchmark_Data_Collector_Home_Impl (void);
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_BENCHMARK_DATA_COLLECTOR_EXEC_H */

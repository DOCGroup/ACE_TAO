// -*- C++ -*-

#ifndef _BENCHMARK_DATA_COLLECTOR_IMPL_H_
#define _BENCHMARK_DATA_COLLECTOR_IMPL_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_CollectorEC.h"
#include "tao/LocalObject.h"
#include "Benchmark_Data_Collector_exec_export.h"
#include "cuts/System_Metric.h"

namespace CUTS
{
  class BDC_Task;
  class BDC_Control_Handle_exec_i;
  class Testing_Service_exec_i;
  class Path_Measurement_exec_i;

  //===========================================================================
  /**
   * @class Benchmark_Data_Collector_Impl
   */
  //===========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export Benchmark_Data_Collector_Impl :
    public virtual CCM_Benchmark_Data_Collector,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    Benchmark_Data_Collector_Impl (void);

    virtual ~Benchmark_Data_Collector_Impl (void);

    virtual ::CORBA::Long
      timeout (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
      timeout (::CORBA::Long ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CORBA::Long
      collection_threads (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
      collection_threads (::CORBA::Long ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual char *
      metrics_outfile (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
      metrics_outfile (const char * ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CUTS::CCM_Testing_Service_ptr
      get_testing_service (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CUTS::CCM_Path_Measurement_ptr
      get_online_measurements (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CUTS::CCM_BDC_Control_Handle_ptr
      get_controls (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /// Set the task implementation for the object.
    void task (BDC_Task * task);

    /// Get the task implementation for the object.
    BDC_Task * task (void);

    /// Set the testing service for the component.
    void testing_service (Testing_Service_exec_i * testing_service);

    /**
     * Get the testing service for the component.
     *
     * @return The current control handle.
     */
    Testing_Service_exec_i * testing_service (void);

    /**
     * Set the control handle for the benchmark data collector's
     * implementation.
     *
     * @param[in]       control_handle      New control handle.
     */
    void control_handle (BDC_Control_Handle_exec_i * control_handle);

    /// System metrics fro the component.
    CUTS_System_Metric system_metric_;

  private:
    /// Active object for the component.
    ACE_Auto_Ptr <BDC_Task> task_;

    /// Testing service object.
    ACE_Auto_Ptr <Testing_Service_exec_i> testing_service_;

    /// Control handle object.
    ACE_Auto_Ptr <BDC_Control_Handle_exec_i> control_handle_;

    /// Path measurement object.
    ACE_Auto_Ptr <Path_Measurement_exec_i> path_measurement_;
  };
}

#if defined (__CUTS_INLINE__)
#include "Benchmark_Data_Collector_Impl.inl"
#endif

#include /**/ "ace/post.h"

#endif  // !defined _BENCHMARK_DATA_COLLECTOR_IMPL_H_

// $Id$

#include "Benchmark_Data_Collector_Impl.h"

#if !defined (__CUTS_INLINE__)
#include "Benchmark_Data_Collector_Impl.inl"
#endif

#include "BDC_Task.h"
#include "BDC_Control_Handle_exec_i.h"
#include "Path_Measurement_exec_i.h"
#include "Testing_Service_exec_i.h"

namespace CUTS
{
  //
  // Benchmark_Data_Collector_Impl
  //
  Benchmark_Data_Collector_Impl::Benchmark_Data_Collector_Impl (void)
  {
    // Create a new <Path_Measurement_exec_i> object.
    Path_Measurement_exec_i * path_measurement = 0;

    ACE_NEW (
      path_measurement,
      Path_Measurement_exec_i (this->system_metric_));

    this->path_measurement_.reset (path_measurement);
  }

  //
  // ~Benchmark_Data_Collector_Impl
  //
  Benchmark_Data_Collector_Impl::~Benchmark_Data_Collector_Impl (void)
  {

  }

  //
  // timeout
  //
  ::CORBA::Long
    Benchmark_Data_Collector_Impl::timeout (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->task_->timeout ();
  }

  //
  // timeout
  //
  void Benchmark_Data_Collector_Impl::timeout (
    ::CORBA::Long timeout ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->task_->timeout (timeout);
  }

  //
  // metrics_outfile
  //
  char *
    Benchmark_Data_Collector_Impl::metrics_outfile (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::String_var str =
      CORBA::string_dup (this->task_->metrics_outfile ().c_str ());
    return str._retn ();
  }

  //
  // timeout
  //
  void Benchmark_Data_Collector_Impl::metrics_outfile (
    const char * filename ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->task_->metrics_outfile (filename);
  }

  //
  // collection_threads
  //
  ::CORBA::Long
    Benchmark_Data_Collector_Impl::collection_threads (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->task_->collection_threads ();
  }

  //
  // collection_threads
  //
  void Benchmark_Data_Collector_Impl::collection_threads (
    ::CORBA::Long threads ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->task_->collection_threads (threads);
  }

  //
  // get_online_measurements
  //
  ::CUTS::CCM_Path_Measurement_ptr
    Benchmark_Data_Collector_Impl::get_online_measurements (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return ::CUTS::CCM_Path_Measurement::_duplicate (
      this->path_measurement_.get ());
  }

  //
  // control_handle
  //
  void Benchmark_Data_Collector_Impl::control_handle (
    BDC_Control_Handle_exec_i * control_handle)
  {
    this->control_handle_.reset (control_handle);
  }

  //
  // testing_service
  //
  void Benchmark_Data_Collector_Impl::testing_service (
    Testing_Service_exec_i * testing_service)
  {
    this->testing_service_.reset (testing_service);

    if (this->path_measurement_.get ())
    {
      this->path_measurement_->testing_service (testing_service);
    }
  }

  //
  // get_testing_service
  //
  ::CUTS::CCM_Testing_Service_ptr
    Benchmark_Data_Collector_Impl::get_testing_service (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return ::CUTS::CCM_Testing_Service::_duplicate (
      this->testing_service_.get ());
  }

  //
  // get_controls
  //
  ::CUTS::CCM_BDC_Control_Handle_ptr
    Benchmark_Data_Collector_Impl::get_controls (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return ::CUTS::CCM_BDC_Control_Handle::_duplicate (
      this->control_handle_.get ());
  }

  //
  // task
  //
  void Benchmark_Data_Collector_Impl::task (BDC_Task * task)
  {
    this->task_.reset (task);
    this->task_->system_metrics (&this->system_metric_);
  }

  //
  // task
  //
  BDC_Task * Benchmark_Data_Collector_Impl::task (void)
  {
    return this->task_.get ();
  }
}

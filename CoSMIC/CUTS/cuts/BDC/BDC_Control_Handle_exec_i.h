// -*- C++ -*-

#ifndef _BDC_CONTROL_HANDLE_EXEC_H_
#define _BDC_CONTROL_HANDLE_EXEC_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_exec_export.h"
#include "Benchmark_Data_CollectorEC.h"
#include "cuts/config.h"
#include "tao/LocalObject.h"

namespace CUTS
{
  // forward declarations
  class BDC_Task;

  //=========================================================================
  /**
   * @class BDC_Control_Handle_exec_i
   *
   * @brief Implementation of the BDC_Control_Handle object.
   */
  //=========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export BDC_Control_Handle_exec_i :
    public virtual CUTS::CCM_BDC_Control_Handle,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Constructor.
    BDC_Control_Handle_exec_i (void);

    /// Destructor.
    virtual ~BDC_Control_Handle_exec_i (void);

    /**
     * Force the collection of the performance data from all the
     * component in the system. This is a blocking method and does
     * not return until all components are handled.
     */
    virtual
      void collect_performance_data (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * Initialize the task of the control agent.
     *
     * @param[in]     task      Pointer to the new task agent.
     */
    void task (BDC_Task * task);

    /**
     * Get the task contained in the control.
     *
     * @return  Pointer to the task.
     */
    BDC_Task * task (void) const;

  private:
    /// Pointer reference to the benchmark data collector.
    BDC_Task * task_;
  };
}

#include /**/ "ace/post.h"

#endif  // !defined _BDC_CONTROL_HANDLE_EXEC_H_

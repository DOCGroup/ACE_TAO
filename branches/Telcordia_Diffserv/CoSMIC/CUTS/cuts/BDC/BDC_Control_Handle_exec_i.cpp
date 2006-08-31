// $Id$

#include "BDC_Control_Handle_exec_i.h"
#include "BDC_Task.h"

namespace CUTS
{
  //
  // BDC_Control_Handle_exec_i
  //
  BDC_Control_Handle_exec_i::BDC_Control_Handle_exec_i (void)
  : task_ (0)
  {

  }

  //
  // ~BDC_Control_Handle_exec_i
  //
  BDC_Control_Handle_exec_i::~BDC_Control_Handle_exec_i (void)
  {

  }

  //
  // task
  //
  void BDC_Control_Handle_exec_i::task (BDC_Task * task)
  {
    this->task_ = task;
  }

  //
  // task
  //
  BDC_Task * BDC_Control_Handle_exec_i::task (void) const
  {
    return this->task_;
  }

  //
  // collect_performance_data
  //
  void BDC_Control_Handle_exec_i::collect_performance_data (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (this->task_ != 0)
    {
      // Notify the <task_> to begin collection.
      this->task_->collect_data ();

      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - waiting for data collection to complete\n"));

      // Wait for signaling of the <collection_done_> event.
      this->task_->wait_for_collection_done ();

      ACE_DEBUG ((LM_DEBUG,
                  "[%M] -%T - collection of performance data is complete\n"));
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - control handle missing BDC task reference\n"));
    }
  }
}

// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  TaskImpl.h
 *
 * This file contains the TaskImpl class definition for the encapsulation
 * of a task implementation's details.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_TASK_IMPL_H_
#define SA_POP_TASK_IMPL_H_

#include "SA_POP_Types.h"

namespace SA_POP {

  /**
   * @class TaskImpl
   *
   * @brief Encapsulation of a task implementation's details.
   */
  class TaskImpl {
  public:
    /// Constructor.
    /**
     * @param id  Unique ID of this task implementation.
     *
     * @param params  Implementation parameters (ID, kind, and value).
     */
    TaskImpl (TaskImplID id, const ImplParamSet &params);

    /// Destructor.
    virtual ~TaskImpl (void);

    /// Set duration.
    /**
     * @param duration  Duration of implementation.
     */
    virtual void set_duration (TimeValue duration);
    /// Get duration.
    /**
     * @return Duration of implementation.
     */
    virtual TimeValue get_duration (void);

    /// Add resource utilization.
    /**
     * @param id  Resource id.
     *
     * @param value  Resource utilization value.
     */
    virtual void add_resource (ResourceID id, ResourceValue value);

    /// Get task implementation id.
    /**
     * @return  Unique ID of this this task implementation.
     */
    virtual TaskImplID get_id (void);

    /// Get task implementation parameters.
    /**
     * @return  Set of parameters for this implementation.
     */
    virtual ImplParamSet get_params (void);

    /// Get utilization of a resource.
    /**
     * @param resource_id  The resource id.
     *
     * @return  The quantity of resource used.
     */
    virtual ResourceValue get_resource_usage (ResourceID resource_id);

    /// Get all resources with utilization.
    /**
     * @return  The set of all resources used (with associated usage values).
     */
    virtual const ResourceMap& get_all_resources ();

  protected:
    /// Unique ID of the task implementation.
    TaskImplID id_;

    /// Set of parameters.
    ImplParamSet params_;

    /// Duration.
    TimeValue duration_;

    /// Map from resource IDs to resource utilization.
    ResourceMap resources_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_TASK_IMPL_H_ */

// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  TaskMap.h
 *
 * This file contains the TaskMap class definition for objects that provide
 * the mapping from tasks to parameterized components, resource capacities, and
 * resource usage for parameterized components.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_TASK_MAP_H_
#define SA_POP_TASK_MAP_H_

#include <set>
#include <map>
#include "SA_POP_Types.h"
#include "TaskImpl.h"

namespace SA_POP {


  /**
   * @class TaskMap
   *
   * @brief Object that provides the mapping from tasks to parameterized
   *        components, resource capacities, and resource usage for
   *        parameterized components.
   */
  class TaskMap {
  public:
    /// Constructor.
    TaskMap (void);

    /// Destructor.
    virtual ~TaskMap (void);



    // ************************************************************************
    // TaskMap construction methods.
    // ************************************************************************

    /// Add a resource.
    /**
     * @param resource  The resource info.
     */
    virtual void add_resource (Resource resource);

    /// Add an implementation.
    /**
     * @param task_impl  The task implementation info.
     */
    virtual void add_task_impl (TaskImpl *task_impl);

    /// Associate a task with a task implementation.
    /**
     * @param task_id  ID of the task.
     *
     * @param task_impl_id  ID of the task implementation.
     *
     * @param duration  The duration of the task implementation for this task.
     */
    virtual void assoc_task_with_impl (TaskID task_id, TaskImplID task_impl_id,
      TimeValue duration);

    /// Associate a task implementation with its utilization of a resource.
    /**
     * @param impl_id  ID of the task implementation.
     *
     * @param resource_id  ID of the associated resource.
     *
     * @param resource_usage  The quantity of resource used.
     */
    virtual void assoc_impl_with_resource (TaskImplID impl_id,
      ResourceID resource_id, ResourceValue resource_usage);



    // ************************************************************************
    // Implementation and resource accessor methods.
    // ************************************************************************

    /// Get all implementations of a task.
    /**
     * @param task_id  The task id.
     *
     * @return  The set of all implementations (ids) for the given task.
     */
    virtual TaskImplSet get_all_impls (TaskID task_id);

    /// Get task implementation.
    /**
     * @param impl_id  The task implementation id.
     *
     * @return  Reference to the task implementation.
     */
    virtual TaskImpl *get_impl (TaskImplID impl_id);

    /// Get utilization info of a task implementation for a resource.
    /**
     * @param impl_id  The task implementation id.
     *
     * @param resource_id  The resource id.
     *
     * @return  The quantity of resource used.
     */
    virtual ResourceValue get_resource_usage (TaskImplID impl_id,
      ResourceID resource_id);

    /// Get all resources used by a task implementation.
    /**
     * @param impl_id  The task implementation id.
     *
     * @return  The set of all resources used (with associated usage values).
     */
    virtual ResourceMap get_all_resources (TaskImplID impl_id);

    /// Get the capacity of a resource.
    /**
     * @param res_id The resource id whose capacity that we want to get.
     *
     * @return The capacity of the resource
     */
    virtual ResourceValue get_capacity (ResourceID res_id);

  protected:
    /// Map from task implementation IDs to objects.
    typedef std::map <TaskImplID, TaskImpl *> ImplIDObjMap;
    ImplIDObjMap task_impls_;

    /// Map from tasks to task implementations.
    typedef std::multimap <TaskID, TaskImplID> TaskToImplMap;
    TaskToImplMap task_to_impls_;

    /// Set of resources and associated capacity.
    Resources resources_;

  };

};  /* SA_POP namespace */

#endif /* SA_POP_TASK_MAP_H_ */

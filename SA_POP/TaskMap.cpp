// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  TaskMap.cpp
 *
 * This file contains the TaskMap class implementation for objects that provide
 * the mapping from tasks to parameterized components, resource capacities, and
 * resource usage for parameterized components.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <set>
#include <map>
#include "SA_POP_Types.h"
#include "TaskMap.h"
#include "TaskImpl.h"

using namespace SA_POP;

// Constructor.
TaskMap::TaskMap (void)
{
  this->task_impls_.clear ();
  this->task_to_impls_.clear ();
  this->resources_.clear ();
};

// Destructor.
TaskMap::~TaskMap (void)
{
  // Delete task implementations.
  for (TaskMap::ImplIDObjMap::iterator iter = this->task_impls_.begin ();
    iter != this->task_impls_.end (); iter++)
  {
    delete iter->second;
  }
};

// Add a resource.
void TaskMap::add_resource (Resource resource)
{
  this->resources_.insert (std::make_pair (resource.id, resource));
};

// Add an implementation.
void TaskMap::add_task_impl (TaskImpl *task_impl)
{
  this->task_impls_.insert (std::make_pair (task_impl->get_id (), task_impl));
};

// Associate a task with a task implementation.
void TaskMap::assoc_task_with_impl (TaskID task_id, TaskImplID task_impl_id,
  TimeValue duration)
{
  this->task_to_impls_.insert (std::make_pair (task_id, task_impl_id));
  ImplIDObjMap::iterator iter = this->task_impls_.find (task_impl_id);
  if (iter == this->task_impls_.end ())
    throw "SA_POP::TaskMap::assoc_task_with_impl (): Unknown implementation ID.";
  iter->second->set_duration (duration);
};

// Associate a task implementation with its utilization of a resource.
void TaskMap::assoc_impl_with_resource (TaskImplID impl_id,
  ResourceID resource_id, ResourceValue resource_usage)
{
  ImplIDObjMap::iterator iter = this->task_impls_.find (impl_id);
  if (iter == this->task_impls_.end ())
    throw "SA_POP::TaskMap::assoc_impl_with_resource (): Unknown implementation ID.";
  iter->second->add_resource (resource_id, resource_usage);
};

// Get all implementations of a task.
TaskImplSet TaskMap::get_all_impls (TaskID task_id)
{
  TaskImplSet temp;
  temp.clear ();
  for (TaskMap::TaskToImplMap::iterator iter =
    this->task_to_impls_.lower_bound (task_id);
    iter != this->task_to_impls_.upper_bound (task_id); iter++)
  {
    temp.insert (iter->second);
  }
  return temp;
};

// Get task implementation.
TaskImpl *TaskMap::get_impl (TaskImplID impl_id)
{
  ImplIDObjMap::iterator iter = this->task_impls_.find (impl_id);
  if (iter == this->task_impls_.end ())
    throw "SA_POP::TaskMap::get_impl (): Unknown implementation ID.";
  return iter->second;
};

// Get utilization info of a task implementation for a resource.
ResourceValue TaskMap::get_resource_usage (TaskImplID impl_id,
  ResourceID resource_id)
{
  ImplIDObjMap::iterator iter = this->task_impls_.find (impl_id);
  if (iter == this->task_impls_.end ())
    throw "SA_POP::TaskMap::get_resource_usage (): Unknown implementation ID.";
  return iter->second->get_resource_usage (resource_id);
};

// Get all resources used by a task implementation.
ResourceMap TaskMap::get_all_resources (TaskImplID impl_id)
{
  ImplIDObjMap::iterator iter = this->task_impls_.find (impl_id);
  if (iter == this->task_impls_.end ())
    throw "SA_POP::TaskMap::get_all_resources (): Unknown implementation ID.";
  return iter->second->get_all_resources ();
};

/// Get the capacity of a resource.
ResourceValue TaskMap::get_capacity (ResourceID res_id)
{
  return this->resources_.find(res_id)->second.capacity;
}

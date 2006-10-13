// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  TaskImpl.cpp
 *
 * This file contains the TaskImpl class implementation for the encapsulation
 * of a task implementation's details.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SA_POP_Types.h"
#include "TaskImpl.h"

using namespace SA_POP;

// Constructor.
TaskImpl::TaskImpl (TaskImplID id, const ImplParamSet &params)
: id_ (id),
  params_ (params),
  duration_ (NULL_TIME)
  
{
  this->resources_.clear ();
};

// Destructor.
TaskImpl::~TaskImpl (void)
{
  // Nothing to do.
};

// Set duration.
void TaskImpl::set_duration (TimeValue duration)
{
  this->duration_ = duration;
};
// Get duration.
TimeValue TaskImpl::get_duration (void)
{
  return this->duration_;
};

// Add resource utilization.
void TaskImpl::add_resource (ResourceID id, ResourceValue value)
{
  this->resources_.insert (std::make_pair (id, value));
};

// Get task implementation id.
TaskImplID TaskImpl::get_id (void)
{
  return this->id_;
};

// Get task implementation parameters.
ImplParamSet TaskImpl::get_params (void)
{
  return this->params_;
};

// Get utilization of a resource.
ResourceValue TaskImpl::get_resource_usage (ResourceID resource_id)
{
  ResourceMap::iterator iter = this->resources_.find (resource_id);
  if (iter == this->resources_.end ())
    return 0;

  return iter->second;
};

// Get all resources with utilization.
const ResourceMap& TaskImpl::get_all_resources ()
{
  return this->resources_;
};


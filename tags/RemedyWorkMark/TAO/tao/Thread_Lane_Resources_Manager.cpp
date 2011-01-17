// -*- C++ -*-
// $Id$

#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/ORB_Core.h"
#include "tao/LF_Strategy.h"

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Thread_Lane_Resources_Manager::TAO_Thread_Lane_Resources_Manager (TAO_ORB_Core &orb_core)
  : orb_core_ (&orb_core),
    lf_strategy_ (0)
{
  this->lf_strategy_ =
    this->orb_core_->resource_factory ()->create_lf_strategy ();
}

TAO_Thread_Lane_Resources_Manager::~TAO_Thread_Lane_Resources_Manager (void)
{
  delete this->lf_strategy_;
}

TAO_LF_Strategy &
TAO_Thread_Lane_Resources_Manager::lf_strategy (void)
{
  return *this->lf_strategy_;
}

TAO_Thread_Lane_Resources_Manager_Factory::~TAO_Thread_Lane_Resources_Manager_Factory (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

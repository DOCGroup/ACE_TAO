// -*- C++ -*-
//
// $Id$

#include "Reply_Handler_i.h"

// Implementation skeleton constructor
Deployment_AMI_NodeApplicationManagerHandler_i::Deployment_AMI_NodeApplicationManagerHandler_i (void)
: retn_node_app_ (0),
  retn_connections_ (0)
{
}

// Implementation skeleton destructor
Deployment_AMI_NodeApplicationManagerHandler_i::~Deployment_AMI_NodeApplicationManagerHandler_i (void)
{
}

void Deployment_AMI_NodeApplicationManagerHandler_i::startLaunch (
    ::Deployment::Application_ptr ami_return_val,
    const ::Deployment::Connections & providedReference)
{
  // Add your implementation here
  ACE_Guard<ACE_Thread_Mutex> guard (lock_);

  // Narrow down to NodeApplication object reference
  this->retn_node_app_ = 
    ::Deployment::NodeApplication::_narrow (ami_return_val);

  this->retn_connections_ = providedReference;
}

::Deployment::NodeApplication_ptr
Deployment_AMI_NodeApplicationManagerHandler_i::get_node_app ()
{
  return ::Deployment::NodeApplication::_duplicate (
    this->retn_node_app_.in ());
}


::Deployment::Connections
Deployment_AMI_NodeApplicationManagerHandler_i::get_connections ()
{
  return this->retn_connections_;
}

void Deployment_AMI_NodeApplicationManagerHandler_i::perform_redeployment (
    ::Deployment::Application_ptr,
    const ::Deployment::Connections &)
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::perform_redeployment_excep (
    ::Messaging::ExceptionHolder *)
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::reset_plan ()
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::reset_plan_excep (
    ::Messaging::ExceptionHolder *)
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::set_shared_components ()
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::set_shared_components_excep (
    ::Messaging::ExceptionHolder *)
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::set_priority (
    ::CORBA::Long)
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::set_priority_excep (
    ::Messaging::ExceptionHolder *)
{
  // Add your implementation here
}



void Deployment_AMI_NodeApplicationManagerHandler_i::startLaunch_excep (
    ::Messaging::ExceptionHolder *)
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::destroyApplication ()
{
  // Add your implementation here
}

void Deployment_AMI_NodeApplicationManagerHandler_i::destroyApplication_excep (
    ::Messaging::ExceptionHolder *)
{
  // Add your implementation here
}


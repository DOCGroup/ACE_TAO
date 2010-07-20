// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1756

#include "PL_Daemon_Impl.h"
#include "Convert_Plan/Convert_Plan_Impl.h"

namespace DAnCE
{

  // Implementation skeleton constructor
  Plan_Launcher_Daemon_i::Plan_Launcher_Daemon_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }
  
  // Implementation skeleton destructor
  Plan_Launcher_Daemon_i::~Plan_Launcher_Daemon_i (void)
  {
  }

  ::Deployment::DeploymentPlan * 
  Plan_Launcher_Daemon_i::parse_xml_plan (const char * plan_location)
  {
    if (!plan_location)
      throw CORBA::BAD_PARAM ();
  
    Deployment::DeploymentPlan * plan = 
      DAnCE::Convert_Plan::read_xml_plan (plan_location);
  
    if (plan)
      return plan;
  
    throw CORBA::UNKNOWN ();
  }


  ::Deployment::DeploymentPlan * 
  Plan_Launcher_Daemon_i::parse_cdr_plan (const char * plan_location)
  {
    if (!plan_location)
      throw CORBA::BAD_PARAM ();
  
    Deployment::DeploymentPlan * plan = 
      DAnCE::Convert_Plan::read_xml_plan (plan_location);
  
    if (plan)
      return plan;
  
    throw CORBA::UNKNOWN ();
  }

  void Plan_Launcher_Daemon_i::shutdown (void)
  {
    this->orb_->shutdown ();
  }
}

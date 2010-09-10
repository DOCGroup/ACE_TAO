// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1698

#ifndef PL_DAEMONI_SPRPP7_H_
#define PL_DAEMONI_SPRPP7_H_

#include "PL_DaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace DAnCE
{

class Plan_Launcher_Daemon_i
  : public virtual POA_DAnCE::Plan_Launcher_Daemon
{
public:
  // Constructor 
  Plan_Launcher_Daemon_i (CORBA::ORB_ptr orb);
  
  // Destructor 
  virtual ~Plan_Launcher_Daemon_i (void);
  
  virtual
  ::Deployment::DeploymentPlan * parse_xml_plan (
    const char * plan_location);

  virtual
  ::Deployment::DeploymentPlan * parse_cdr_plan (
    const char * plan_location);
  
  virtual
  void shutdown (void);
  
 private:
  CORBA::ORB_var orb_;
};

}

#endif /* PL_DAEMONI_H_  */

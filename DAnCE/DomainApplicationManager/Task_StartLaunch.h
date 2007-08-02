// -*- C++ -*-

//=============================================================================
/**
 *  @file    Task_StartLaunch.h
 *
 *  $Id$
 *
 *  An ACE_Task subclass that implements the <startLaunch> task, which
 *  basically calls <startLaunch> operation on a NodeApplicationManager.
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_TASK_START_LAUNCH_H
#define CIAO_TASK_START_LAUNCH_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "ace/Task.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationManagerS.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationS.h"

namespace CIAO
{
  /**
   * @class Task_StartLaunch
   *
   * @brief An ACE_Task subclass that allow the <startLaunch> invocation
   * to be completed in a separate thread than then main thread running
   * the DomainApplicationManager.
   */
  class Task_StartLaunch
    : public virtual ACE_Task_Base
  {
  public:
    Task_StartLaunch (const char * node_name,
                      const ::Deployment::NodeApplicationManager_ptr nam,
                      const ::Deployment::Properties & configProperty);

    /// Service entry point
    virtual int svc (void);

    /// Accessor method to get returned connections
    ::Deployment::Connections get_connections ();

    /// Accessor method to get returned NA
    ::Deployment::NodeApplication_ptr get_node_app ();

  protected:
    //CORBA::ORB_var orb_;

    /// The node name which hosts the NAM
    ACE_CString node_name_;

    /// Input parameter which identifies the NAM to invoke the call
    ::Deployment::NodeApplicationManager_var nam_;

    /// Input parameter of the configProperty
    const ::Deployment::Properties & configProperty_;
    
    /// The NA obj ref is a return value by calling startLaunch on NAM
    ::Deployment::NodeApplication_var retn_node_app_;

    /// Returned object references of <connections>, i.e., provided ports
    ::Deployment::Connections_var retn_connections_;
  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_TASK_START_LAUNCH_H */

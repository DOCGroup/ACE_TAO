//==================================================================
/**
 *  @file DomainApplicationManager_ActiveObject_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Deployment::DomainApplicationManager_AMI_Impl.
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=====================================================================

#ifndef CIAO_DOMAINAPPLICATIONMANAGER_ACTIVE_OBJECT_IMPL_H
#define CIAO_DOMAINAPPLICATIONMANAGER_ACTIVE_OBJECT_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DomainApplicationManager_Impl.h"
#include "Task_StartLaunch.h"

namespace CIAO
{
  /**
   * @class DomainApplicationManager_ActiveObject_Impl
   *
   * @brief A subclass of the DomainApplicationManager_Impl servant
   * which uses active object mechanism to handle deployment.  Internally
   * it spawns threads with each thread runs an active object
   * to invoke the remote invocation, such as invoking startLaunch
   * on a corresponding NodeApplicationManager.
   */
  class DomainApplicationManager_Export DomainApplicationManager_ActiveObject_Impl
    : public DomainApplicationManager_Impl
  {
  public:
    /// Constructor
    DomainApplicationManager_ActiveObject_Impl (
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               Execution_Manager::Execution_Manager_Impl *em,
                               const Deployment::DeploymentPlan &plan,
                               const char * deployment_file);

    virtual void
      startLaunch (const ::Deployment::Properties & configProperty,
                   ::CORBA::Boolean start);

  private:
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Task_StartLaunch *,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> StartLaunch_Task_Table;
    typedef StartLaunch_Task_Table::iterator StartLaunch_Task_Table_Iterator;

    StartLaunch_Task_Table start_launch_task_table_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_ACTIVE_OBJECT_IMPL_H */

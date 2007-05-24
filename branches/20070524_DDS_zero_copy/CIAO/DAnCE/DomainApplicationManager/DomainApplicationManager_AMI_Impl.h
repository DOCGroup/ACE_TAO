//==================================================================
/**
 *  @file DomainApplicationManager_AMI_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Deployment::DomainApplicationManager_AMI_Impl.
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=====================================================================

#ifndef CIAO_DOMAINAPPLICATIONMANAGER_AMI_IMPL_H
#define CIAO_DOMAINAPPLICATIONMANAGER_AMI_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DomainApplicationManager_AMH_Impl.h"
#include "Reply_Handler_i.h"
#include "DAnCE/Deployment/Deployment_NodeApplicationManagerC.h"
#include "DAnCE/Deployment/Deployment_ApplicationC.h"

namespace CIAO
{
  /**
   * @class DomainApplicationManager_AMI_Impl
   *
   * @brief A subclass of the DomainApplicationManager_Impl servant
   * which uses AMI mechanism to handle deployment.
   */
  class DomainApplicationManager_Export DomainApplicationManager_AMI_Impl
    : public DomainApplicationManager_AMH_Impl
  {
  public:
    /// Constructor
    DomainApplicationManager_AMI_Impl (CORBA::ORB_ptr orb,
                                       PortableServer::POA_ptr poa,
                                       Deployment::TargetManager_ptr manager,
                                       Execution_Manager::Execution_Manager_Impl *em,
                                       const Deployment::DeploymentPlan &plan,
                                       const char * deployment_file);

    virtual void
      startLaunch (Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh,
                   const ::Deployment::Properties & configProperty,
                   ::CORBA::Boolean start);

    /// This operation will be called after *all* the AMI callbacks
    /// are received
    void
    post_ami_startLaunch (void);

    void decrease_start_launch_reply_count (void);

    int start_launch_reply_count (void);

  private:
    typedef struct
    {
      Deployment_AMI_NodeApplicationManagerHandler_i * servant_;
      ::Deployment::AMI_NodeApplicationManagerHandler_var obj_ref_;
    } AMI_NAM_Handler;

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    AMI_NAM_Handler,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> AMI_NAM_Handler_Table;
    typedef AMI_NAM_Handler_Table::iterator AMI_NAM_Handler_Table_Iterator;

    /// A table to trace the AMI reply handler
    AMI_NAM_Handler_Table ami_nam_handler_table_;

    /// Disable copy assignment
    DomainApplicationManager_AMI_Impl (const DomainApplicationManager_AMI_Impl&);

    /// AMI reply count
    volatile int start_launch_reply_count_;

    /// Object reference of the AMH response handler
    Deployment::AMH_DomainApplicationManagerResponseHandler_var amh_response_handler_;

    /// Object ID of the AMH response handler
    //PortableServer::ObjectId_var rh_oid_;
    ACE_Vector<PortableServer::ObjectId_var> rh_oid_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_AMI_IMPL_H */

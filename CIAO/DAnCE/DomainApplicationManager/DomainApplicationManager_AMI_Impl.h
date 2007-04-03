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

#include "DomainApplicationManager_Impl.h"

namespace CIAO
{
  /**
   * @class DomainApplicationManager_AMI_Impl
   *
   * @brief A subclass of the DomainApplicationManager_Impl servant
   * which uses AMI mechanism to handle deployment.
   */
  class DomainApplicationManager_Export DomainApplicationManager_AMI_Impl
    : public DomainApplicationManager_Impl
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
      startLaunch (const ::Deployment::Properties & configProperty,
                   ::CORBA::Boolean start);

  private:

  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATIONMANAGER_AMI_IMPL_H */

// $Id$
//===============================================================
/**
 * @file TargetManager_exec.h
 *
 * @brief TargetManager Executor code
 *
 * @author Nilabja Roy nilabjar@dre.vanderbilt.edu
 */
//===============================================================

#ifndef CIAO_TARGETMANAGER_EXEC_H
#define CIAO_TARGETMANAGER_EXEC_H

#include /**/ "ace/pre.h"

#include "TargetManager_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TargetManager_exec_export.h"
#include "tao/LocalObject.h"
#include "DomainDataManager.h"

/**
 * TargetManager Executor namespace
 */
namespace CIDL_TargetManager_i
{
  class TargetManager_exec_i;

  class TARGETMANAGER_EXEC_Export TargetManagerImpl_exec_i
  : public virtual TargetManagerImpl_Exec,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    TargetManagerImpl_exec_i (void);
    virtual ~TargetManagerImpl_exec_i (void);

    // Supported or inherited operations.

    // Attribute operations.

    // Port operations.

    virtual ::Deployment::CCM_TargetManager_ptr
    get_targetMgr ();

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
    ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

  protected:
    /// The service context pointer
    TargetManagerImpl_Context *context_;

    /// The exec Object
    ::Deployment::CCM_TargetManager_var exec_object_;
  };
  class TARGETMANAGER_EXEC_Export TargetManager_exec_i
  : public virtual ::Deployment::CCM_TargetManager,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    TargetManager_exec_i (TargetManagerImpl_exec_i* exec,
                          CORBA::ORB_ptr orb,
                          TargetManagerImpl_Context *context);
    virtual ~TargetManager_exec_i (void);

    // Operations from ::Deployment::TargetManager

    virtual ::Deployment::Domain * getAllResources ();

    virtual ::Deployment::Domain * getAvailableResources ();

    virtual void commitResources (const ::Deployment::DeploymentPlan & plan);

    virtual void releaseResources (const ::Deployment::DeploymentPlan & argname);

    virtual void
    updateDomain (
    const ::CORBA::StringSeq & elements,
    const ::Deployment::Domain & domainSubset,
    ::Deployment::DomainUpdateKind updateKind);

    virtual ::Deployment::ResourceCommitmentManager_ptr
    createResourceCommitment (const ::Deployment::ResourceAllocations& manager);

    virtual void
    destroyResourceCommitment (
    ::Deployment::ResourceCommitmentManager_ptr resources);

  private:
    TargetManagerImpl_exec_i * _exec;

    /// The pointer to the Domain Manager
    auto_ptr<CIAO::DomainDataManager> dataManager_;

    /// The CORBA ORB ...
    CORBA::ORB_var orb_;

    /// The context object ...
    TargetManagerImpl_Context *context_;
  };



  class TARGETMANAGER_EXEC_Export TargetManagerHome_exec_i
  : public virtual TargetManagerHome_Exec,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    TargetManagerHome_exec_i (void);
    virtual ~TargetManagerHome_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" TARGETMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
    create_CIAO_TargetManagerHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_TARGETMANAGER_EXEC_H */

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
#include "TargetManagerEC.h"

/**
 * TargetManager Executor namespace
 */
namespace CIDL_TargetManager_i
{
  class TARGETMANAGER_EXEC_Export TargetManagerImpl_exec_i
  : public virtual TargetManagerImpl_Exec,
    public virtual ::CORBA::LocalObject
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

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    virtual void configuration_complete ();

  public:
    /// The service context pointer
    ::CIAO::CCM_TargetManagerImpl_Context_var context_;

    /// The exec Object
    ::Deployment::CCM_TargetManager_var exec_object_;
  };

  class TARGETMANAGER_EXEC_Export TargetManager_exec_i
  : public virtual ::Deployment::CCM_TargetManager,
    public virtual ::CORBA::LocalObject
  {
    public:
    TargetManager_exec_i (TargetManagerImpl_exec_i* exec,
                          CORBA::ORB_ptr orb);
    virtual ~TargetManager_exec_i (void);

    // Operations from ::Deployment::TargetManager

    virtual ::Deployment::Domain * getAllResources ();

    virtual ::Deployment::Domain * getAvailableResources ();

    virtual ::Deployment::ResourceCommitmentManager_ptr
      commitResources (const ::Deployment::ResourceAllocations & resources);

    virtual void releaseResources (::Deployment::ResourceCommitmentManager_ptr manager);

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
    TargetManagerImpl_exec_i * exec_;

    /// The pointer to the Domain Manager
    auto_ptr<CIAO::DomainDataManager> dataManager_;

    /// The CORBA ORB ...
    CORBA::ORB_var orb_;
  };

  class TARGETMANAGER_EXEC_Export TargetManagerHome_exec_i
  : public virtual TargetManagerHome_Exec,
    public virtual ::CORBA::LocalObject
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

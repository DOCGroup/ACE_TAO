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
    get_targetMgr (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CIAO::CCM_TargetManagerExt_ptr
      get_target_manager_ext (
                              ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
                              )
      ACE_THROW_SPEC ((::CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
   ::Components::CCMException));

    virtual void
    ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

  protected:
    /// The service context pointer
    TargetManagerImpl_Context *context_;

    /// The exec Object
    ::Deployment::CCM_TargetManager_var exec_object_;

    ::CIAO::CCM_TargetManagerExt_var exec_ext_object_;
  };

  class TARGETMANAGER_EXEC_Export TargetManagerExt_exec_i
    : public virtual ::CIAO::CCM_TargetManagerExt,
  public virtual TAO_Local_RefCounted_Object
    {
    public:
      TargetManagerExt_exec_i (void);
      virtual ~TargetManagerExt_exec_i (void);

      // Operations from ::CIAO::TargetManagerExt

      virtual ::CORBA::Long
        get_pid (
      const char * component_uuid
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual ::CIAO::Host_Infos *
        get_host_cpu (
                      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual ::CIAO::Component_Infos *
        get_component_cpu (
                           ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual ::CIAO::Host_NodeManager_seq *
        get_all_node_managers (
                           ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
    };

 class TARGETMANAGER_EXEC_Export TargetManager_exec_i
  : public virtual ::Deployment::CCM_TargetManager,
  public virtual TAO_Local_RefCounted_Object
  {
    public:
    TargetManager_exec_i (TargetManagerImpl_exec_i* exec,
                          CORBA::ORB_ptr orb,
                          TargetManagerImpl_Context *context
                          );
    virtual ~TargetManager_exec_i (void);

    // Operations from ::Deployment::TargetManager

    virtual ::Deployment::Domain *
    getAllResources (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Deployment::Domain *
    getAvailableResources (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    commitResources (
    const ::Deployment::DeploymentPlan & plan
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Deployment::ResourceNotAvailable,
    ::Deployment::PlanError));

    virtual void
    releaseResources (
    const ::Deployment::DeploymentPlan & argname
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    updateDomain (
    const ::CORBA::StringSeq & elements,
    const ::Deployment::Domain & domainSubset,
    ::Deployment::DomainUpdateKind updateKind
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Deployment::ResourceCommitmentManager_ptr 
    createResourceCommitment (
    const ::Deployment::ResourceAllocations& manager
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Deployment::ResourceCommitmentFailure));

    virtual void 
    destroyResourceCommitment (
    ::Deployment::ResourceCommitmentManager_ptr resources
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  private:
    TargetManagerImpl_exec_i * _exec;

    ///The pointer to the Domain Manager
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
    create (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));
  };

  extern "C" TARGETMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
    create_CIAO_TargetManagerHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_TARGETMANAGER_EXEC_H */

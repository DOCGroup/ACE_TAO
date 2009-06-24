// $Id$
#include "TargetManager_exec.h"
//#include "ciao/CIAO_common.h"
#include "tools/Config_Handlers/DD_Handler.h"
#include "tools/Config_Handlers/DnC_Dump.h"

#include "DomainEventsC.h"

#include "ResourceCommitmentManager.h"

namespace CIDL_TargetManager_i
{
  //==================================================================
  // Facet Executor Implementation Class:   TargetManager_exec_i
  //==================================================================

  TargetManager_exec_i::
  TargetManager_exec_i (TargetManagerImpl_exec_i* exec ,
                        CORBA::ORB_ptr orb)
    : exec_ (exec),
      orb_ (::CORBA::ORB::_duplicate (orb))
  {
    // The DomainDataManager created here ...

    // get its own obj ref , then call
    ::Deployment::TargetManager_var target = this->exec_->get_targetMgr ();
    
    // Create Domain Data here

    CIAO::DomainDataManager::create (orb_.in (), target.in ());
  }

  TargetManager_exec_i::~TargetManager_exec_i (void)
  {
  }

  // Operations from ::Deployment::TargetManager

  ::Deployment::Domain *
  TargetManager_exec_i::getAllResources ()
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->get_initial_domain ();
  }

  ::Deployment::Domain *
  TargetManager_exec_i::getAvailableResources ()
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->get_current_domain ();
  }

  ::Deployment::ResourceCommitmentManager_ptr
  TargetManager_exec_i::commitResources (
  const ::Deployment::ResourceAllocations & resources)
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->commitResources (resources);
  }

  void
  TargetManager_exec_i::releaseResources (
  ::Deployment::ResourceCommitmentManager_ptr manager)
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->releaseResources (manager);
  }

  void
  TargetManager_exec_i::updateDomain (
  const ::CORBA::StringSeq &  elements ,
  const ::Deployment::Domain &  domainSubset ,
  ::Deployment::DomainUpdateKind  updateKind)
  {
    // Your code here.
    CIAO::DomainDataManager::
      get_data_manager ()->update_domain (
                    elements,
                    domainSubset,
                    updateKind);

    // here tell the planner about the changes

    // first get the node names which have failed ...
    // assuming nodes to only fail , for now

    if (updateKind == ::Deployment::Delete ||
        updateKind == ::Deployment::Add)
      {
        CIAO::Domain_Changed_Event_var changed_event =
          new OBV_CIAO::Domain_Changed_Event ();

        ::Deployment::Domain_var temp_domain =
          new ::Deployment::Domain (domainSubset);

        changed_event->changes (temp_domain);
        changed_event->change_kind (updateKind);
        this->exec_->context_->push_changes (changed_event);
      }

  }

  ::Deployment::ResourceCommitmentManager_ptr
  TargetManager_exec_i::createResourceCommitment (
  const ::Deployment::ResourceAllocations& manager)
  {

    CIAO::ResourceCommitmentManager_i *commit_servant =
      new CIAO::ResourceCommitmentManager_i ();

    // Standard owner transfer mechanisms.
    //
    PortableServer::ServantBase_var safe_daemon (commit_servant);

    commit_servant->commitResources (manager);

    Deployment::ResourceCommitmentManager_var mgrv =
      commit_servant->_this ();

    return mgrv._retn ();
  }

  void
  TargetManager_exec_i::destroyResourceCommitment (
  ::Deployment::ResourceCommitmentManager_ptr manager)
  {
    ::Deployment::ResourceAllocations res;
    res.length (0);
    manager->releaseResources (res);
    return;
  }
  //==================================================================
  // Component Executor Implementation Class:   TargetManagerImpl_exec_i
  //==================================================================

  TargetManagerImpl_exec_i::TargetManagerImpl_exec_i (void)
  : exec_object_ (0)
  {
  }

  TargetManagerImpl_exec_i::~TargetManagerImpl_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  ::Deployment::CCM_TargetManager_ptr
  TargetManagerImpl_exec_i::get_targetMgr ()
  {
    // Your code here.

    if (CORBA::is_nil (this->exec_object_.in ()))
      {
        this->exec_object_ = new TargetManager_exec_i(this,
                                     context_->_get_orb());
      }

    return ::Deployment::CCM_TargetManager::_duplicate (this->exec_object_.in ());
  }

  // Operations from Components::SessionComponent

  void
  TargetManagerImpl_exec_i::set_session_context (
  ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::CIAO::CCM_TargetManagerImpl_Context::_narrow (ctx);
    if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
  }

  void
  TargetManagerImpl_exec_i::configuration_complete ()
  {
    // Your code here.
  }

  void
  TargetManagerImpl_exec_i::ccm_activate ()
  {
    // Your code here.
    this->get_targetMgr ();
  }

  void
  TargetManagerImpl_exec_i::ccm_passivate ()
  {
    // Your code here.
  }

  void
  TargetManagerImpl_exec_i::ccm_remove ()
  {
    // Your code here.

    //CIAO::DomainDataManager::get_data_manager ()->stop_monitors ();

    return;
  }


  //==================================================================
  // Home Executor Implementation Class:   TargetManagerHome_exec_i
  //==================================================================

  TargetManagerHome_exec_i::TargetManagerHome_exec_i (void)
  {
  }

  TargetManagerHome_exec_i::~TargetManagerHome_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  TargetManagerHome_exec_i::create ()
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
    retval,
    TargetManagerImpl_exec_i,
    CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" TARGETMANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_CIAO_TargetManagerHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
    retval,
    TargetManagerHome_exec_i,
    ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

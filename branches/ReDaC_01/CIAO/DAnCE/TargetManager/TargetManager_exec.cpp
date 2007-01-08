// $Id$
//===============================================================
/**
 * @file TargetManager_exec.cpp
 *
 * @brief TargetManager Executor code
 *
 * @author Nilabja Roy nilabjar@dre.vanderbilt.edu
 */
//===============================================================
#include "TargetManager_exec.h"
#include "ciao/CIAO_common.h"
#include <orbsvcs/CosNamingC.h>
#include "Config_Handlers/DD_Handler.h"
#include "Config_Handlers/DnC_Dump.h"

#include "DomainEventsC.h"

using namespace std;

namespace CIDL_TargetManager_i
{
  //==================================================================
  // Facet Executor Implementation Class:   TargetManager_exec_i
  //==================================================================

  TargetManager_exec_i::
  TargetManager_exec_i (TargetManagerImpl_exec_i* exec ,
                        CORBA::ORB_ptr orb,
                        TargetManagerImpl_Context *context
                       )
    : _exec (exec),
      orb_ (orb),
      context_ (context)
  {
    // The DomainDataManager created here ...

    // get its own obj ref , then call

    ACE_DEBUG ((LM_DEBUG, "TM_Exec: getting ccm object\n"));
    CORBA::Object_var object = context_->get_CCM_object ();
    ACE_DEBUG ((LM_DEBUG, "TM_Exec: narrowing target_impl\n"));
    CIAO::TargetManagerImpl_var target_impl =
            CIAO::TargetManagerImpl::_narrow (object.in ());
    ACE_DEBUG ((LM_DEBUG, "TM_Exec: provide target manager\n"));
    ::Deployment::TargetManager_var target =
            target_impl->provide_targetMgr ();
    //    dataManager_.reset (new CIAO::DomainDataManager (orb, target.in ()));
    ACE_DEBUG ((LM_DEBUG, "TM_Exec: creating domain data manager\n"));
    CIAO::DomainDataManager::create (orb, target.in());
    ACE_DEBUG ((LM_DEBUG, "TM_Exec: DDD created!\n"));
  }

  TargetManager_exec_i::~TargetManager_exec_i (void)
  {
  }

  // Operations from ::Deployment::TargetManager

  ::Deployment::Domain *
  TargetManager_exec_i::getAllResources (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return  CIAO::DomainDataManager::
      get_data_manager ()->get_initial_domain ();
  }

  ::Deployment::Domain *
  TargetManager_exec_i::getAvailableResources (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->get_current_domain ();
  }

  void
  TargetManager_exec_i::commitResources (
  const ::Deployment::DeploymentPlan & plan
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Deployment::ResourceNotAvailable,
  ::Deployment::PlanError))
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->commitResources (plan);
  }

  void
  TargetManager_exec_i::releaseResources (
  const ::Deployment::DeploymentPlan &  plan
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->releaseResources (plan);
  }

  void
  TargetManager_exec_i::updateDomain (
  const ::CORBA::StringSeq &  elements ,
  const ::Deployment::Domain &  domainSubset ,
  ::Deployment::DomainUpdateKind  updateKind
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , ".. Update Domain called ...\n"));
    }
    CIAO::DomainDataManager::
      get_data_manager ()->update_domain (
                    elements,
                    domainSubset,
                    updateKind
                    );

    // here tell the planner about the changes

    // first get the node names which have failed ...
    // assuming nodes to only fail , for now

    if (updateKind == ::Deployment::Delete ||
        updateKind == ::Deployment::Add)
      {
        ACE_DEBUG ((LM_DEBUG , "TM::Creating the changed event\n"));

        CIAO::Domain_Changed_Event_var changed_event =
          new OBV_CIAO::Domain_Changed_Event ();

        ::Deployment::Domain_var temp_domain =
          new ::Deployment::Domain (domainSubset);

        ACE_DEBUG ((LM_DEBUG , "TM::After getting the current domain\n"));
        changed_event->changes (temp_domain);
        changed_event->change_kind (updateKind);

        ACE_DEBUG ((LM_DEBUG , "TM::Sending the event to the Planner_Manager\n"));
        context_->push_changes (changed_event);
        ACE_DEBUG ((LM_DEBUG , "TM::After   Sending the event to the Planner_Manager\n"));
      }

  }

  //==================================================================
  // Facet Executor Implementation Class:   TargetManagerExt_exec_i
  // required for RACE
  //==================================================================

  TargetManagerExt_exec_i::TargetManagerExt_exec_i (void)
  {
  }

  TargetManagerExt_exec_i::~TargetManagerExt_exec_i (void)
  {
  }

  // Operations from ::CIAO::TargetManagerExt

  ::CORBA::Long
  TargetManagerExt_exec_i::get_pid (
                                    const char * component_uuid
                                    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    ACE_DEBUG ((LM_DEBUG, "Get PID :: Skeleton Impl"));
    return CIAO::DomainDataManager::
      get_data_manager ()->get_pid (component_uuid);
  }

  ::CIAO::Host_Infos *
  TargetManagerExt_exec_i::get_host_cpu (
                                         ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    ACE_DEBUG ((LM_DEBUG, "Get host cpu :: Skeleton Impl entering\n"));
    return CIAO::DomainDataManager::
      get_data_manager ()->get_cpu_info ();
  }

  ::CIAO::Component_Infos *
  TargetManagerExt_exec_i::get_component_cpu (
                                              ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    ACE_DEBUG ((LM_DEBUG, "Get component cpu :: Skeleton Impl"));
    return 0;
  }

  ::CIAO::Host_NodeManager_seq *
  TargetManagerExt_exec_i::get_all_node_managers
  (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CIAO::DomainDataManager::
      get_data_manager ()->get_node_managers ();
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
  TargetManagerImpl_exec_i::get_targetMgr (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Calling TM constructor"));
    }

    if (this->exec_object_.in () == 0)
    {
      this->exec_object_ = new TargetManager_exec_i(this,
                    context_->_ciao_the_Container()->the_ORB(),
                    context_
                    );
    }
    return this->exec_object_.in ();
  }

  ::CIAO::CCM_TargetManagerExt_ptr TargetManagerImpl_exec_i
  ::get_target_manager_ext (
                            ACE_ENV_SINGLE_ARG_DECL_NOT_USED
                            ) ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (this->exec_ext_object_.in () == 0)
    {
      this->exec_ext_object_ = new TargetManagerExt_exec_i();
    }
    return this->exec_ext_object_.in ();
  }

  // Operations from Components::SessionComponent

  void
  TargetManagerImpl_exec_i::set_session_context (
  ::Components::SessionContext_ptr ctx
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    this->context_ =
    TargetManagerImpl_Context::_narrow (
    ctx
    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (this->context_ == 0)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }
  }

  void
  TargetManagerImpl_exec_i::ciao_preactivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    // Your code here.
  }

  void
  TargetManagerImpl_exec_i::ciao_postactivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    // Your code here.
  }


  void
  TargetManagerImpl_exec_i::ccm_activate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    // Your code here.
    if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Inside CCM_ACTIVATE\n"));
    }
    get_targetMgr ();
  }

  void
  TargetManagerImpl_exec_i::ccm_passivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    // Your code here.
  }

  void
  TargetManagerImpl_exec_i::ccm_remove (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    // Your code here.
    ACE_DEBUG ((LM_DEBUG , "TM::ccm_remove , calling LeaveDomain\n"));

    //CIAO::DomainDataManager::get_data_manager ()->stop_monitors ();

    ACE_DEBUG ((LM_DEBUG , "TM::ccm_remove , After calling LeaveDomain\n"));
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
  TargetManagerHome_exec_i::create (
  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
    retval,
    TargetManagerImpl_exec_i,
    CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

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

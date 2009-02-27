// -*- C++ -*-

//=============================================================================
/**
 *  @file    FaultCorrelationManager_Impl.cpp
 *
 *  $Id$
 *
 *  @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#include "FaultCorrelationManager_Impl.h"
#include "ciao/FTComponentServer/CIAO_PropertiesC.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  FaultCorrelationManager_Impl::FaultCorrelationManager_Impl (
    CORBA::ORB_ptr orb,
    DAnCE::ExecutionManagerDaemon_ptr exec_mgr,
    const PROPERTY_MAP & options)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      exec_mgr_ (DAnCE::ExecutionManagerDaemon::_duplicate (exec_mgr)),
      properties_ (options.current_size ())
  {
    PROPERTY_MAP::const_iterator i = options.begin ();
    while (!i.done ())
      {
        DANCE_DEBUG ((LM_TRACE, DLINFO "FaultCorrelationManager_Impl::FaulCorrelationManager_Impl - "
                      "Binding property %C provided by caller.\n", i->key ().c_str ()));
        this->properties_.bind (i->key (), i->item ());
        i.advance ();
      }
  }

  FaultCorrelationManager_Impl::~FaultCorrelationManager_Impl()
  {
  }

  void
  FaultCorrelationManager_Impl::stop_failver_unit (const char * /* fou_id */)
  {
  }

  void
  FaultCorrelationManager_Impl::proc_failure (const char * object_id,
                                              const char * node_id)
  {
    TObjectIdMap node;
    if (nodes_.find (node_id,
                     node) != 0)
      {
        DANCE_DEBUG ((LM_WARNING,
                      "FCM::proc_failure (%C, %C): node '%C' not found.\n",
                      object_id,
                      node_id,
                      node_id));
        return;
      }

    ACE_CString component_id = node[object_id];

    if (component_id.length () == 0)
      {
        DANCE_DEBUG ((LM_WARNING,
                      "FCM::proc_failure (%C, %C): "
                      "object_id '%C' on '%C' not found.\n",
                      object_id,
                      node_id,
                      object_id,
                      node_id));
        return;
      }

    ACE_CString plan_id;
    if (instances_.find (component_id,
                         plan_id) != 0)
      {
        DANCE_DEBUG ((LM_WARNING,
                      "FCM::proc_failure (%C, %C): "
                      "plan for component '%C' not found.\n",
                      object_id,
                      node_id,
                      component_id.c_str ()));
        return;
      }

    DANCE_DEBUG ((LM_TRACE, 
                  "FCM::proc_failure (%C, %C): "
                  "caused by component '%C' in plan '%C'.\n",
                  object_id,
                  node_id,
                  component_id.c_str (),
                  plan_id.c_str ()));

    try
      {
        Deployment::DomainApplicationManager_var dam;

        if (dams_.find (plan_id.c_str (),
                        dam) != 0)
          {
            DANCE_DEBUG ((LM_TRACE, 
                          "FCM::proc_failure (%C, %C): "
                          "could not resolce DAM for plan '%C'.\n",
                          object_id,
                          node_id,
                          plan_id.c_str ()));
          }

        Deployment::Applications_var apps = dam->getApplications();

        for (size_t i = 0; i < apps->length(); ++i)
          {
            dam->destroyApplication(apps[i]);
          }

        exec_mgr_->destroyManager (dam.in ());

        DANCE_DEBUG ((LM_TRACE, 
                      "FCM::proc_failure (%C, %C): "
                      "plan '%C' was shutdown sucessfully.\n",
                      object_id,
                      node_id,
                      plan_id.c_str ()));
      }
    catch (const CORBA::Exception & ex)
      {
        DANCE_DEBUG ((LM_ERROR,
                      "FCM::proc_failure caught %n",
                      ex._info ().c_str ()));
      }
  }

  ::Deployment::DomainApplicationManager_ptr
  FaultCorrelationManager_Impl::preparePlan (
        const ::Deployment::DeploymentPlan & plan,
        ::Deployment::ResourceCommitmentManager_ptr resourceCommitment)
  {
    Deployment::DomainApplicationManager_var dam =
      exec_mgr_->preparePlan (plan, resourceCommitment);

    dams_.bind (plan.UUID.in(), 
                Deployment::DomainApplicationManager::_duplicate (dam.in ()));

    this->process_deployment_plan (plan);

    return Deployment::DomainApplicationManager::_duplicate (dam.in ());
  }
    
  ::Deployment::DomainApplicationManagers * 
  FaultCorrelationManager_Impl::getManagers (void)
  {
    return exec_mgr_->getManagers ();
  }

  void
  FaultCorrelationManager_Impl::destroyManager (
    Deployment::DomainApplicationManager_ptr manager)
  {
    Deployment::DeploymentPlan_var plan = manager->getPlan ();

    dams_.unbind (plan->UUID.in ());

    exec_mgr_->destroyManager (manager);
  }

  void
  FaultCorrelationManager_Impl::shutdown (void)
  {
    exec_mgr_->shutdown ();
  }

  void
  FaultCorrelationManager_Impl::process_deployment_plan (
    const Deployment::DeploymentPlan &  plan)
  {
    // add all found component instances to the map
    const Deployment::InstanceDeploymentDescription id;
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      {        
        // add component with the plan id it belongs to
        instances_.bind (plan.instance[i].name.in (), 
                         plan.UUID.in ());

        DANCE_DEBUG ((LM_TRACE, 
                      "FCM: instance[%C] -> plan[%C]\n",
                      plan.instance[i].name.in (), 
                      plan.UUID.in ()));

        // find object_id property if existing
        CORBA::String_var object_id (
          this->get_property (CIAO::Deployment::OBJECT_ID,
                              plan.instance[i].configProperty));

        if (object_id.in () == 0)
          object_id = plan.instance[i].name.in ();

        TObjectIdMap oidmap;
        if (nodes_.find (plan.instance[i].node.in (),
                         oidmap) == 0)
          {
            // the new component to exisiting node map
            oidmap[object_id.in ()] = plan.instance[i].name.in ();
            nodes_.rebind (plan.instance[i].node.in (),
                           oidmap);
          }
        else
          {
            // if no entry for this node exists, add a new one
            TObjectIdMap om;
            om[object_id.in ()] = plan.instance[i].name.in ();
            nodes_.bind (plan.instance[i].node.in (),
                         om);
          }

        DANCE_DEBUG ((LM_TRACE, 
                      "FCM: node[%C] -> oid[%C] -> instance[%C]\n",
                      plan.instance[i].node.in (), 
                      object_id.in (),
                      plan.instance[i].name.in ()));
      }
  }

  char * 
  FaultCorrelationManager_Impl::get_property (
    const char * name, 
    const Deployment::Properties & properties)
  {
    for (CORBA::ULong i = 0; i < properties.length (); ++i)
      {
        if (ACE_OS::strcmp (name,
                            properties[i].name.in ()) == 0)
          {
            const CORBA::Any & value = properties[i].value;
            const char * cval;
            if (value >>= cval)
              return CORBA::string_dup (cval);

            break;
          }
      }

    return 0;
  }

};

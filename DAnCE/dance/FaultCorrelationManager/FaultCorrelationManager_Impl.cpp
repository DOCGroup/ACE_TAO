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

#include "ace/High_Res_Timer.h"
#include "ace/Date_Time.h"
#include "FaultCorrelationManager_Impl.h"
#include "ciao/Base/CIAO_PropertiesC.h"
#include "dance/Logger/Log_Macros.h"

namespace DAnCE
{
  FaultCorrelationManager_Impl::FaultCorrelationManager_Impl (
    CORBA::ORB_ptr orb,
    ::Deployment::ExecutionManager_ptr exec_mgr,
    ReplicationManager_ptr rep_mgr,
    const PROPERTY_MAP & options)
    : orb_ (CORBA::ORB::_duplicate (orb)),
    exec_mgr_ (::Deployment::ExecutionManager::_duplicate (exec_mgr)),
      rep_mgr_ (ReplicationManager::_duplicate (rep_mgr)),
      properties_ (options.current_size ()),
      stop_ (false),
      new_notification_ (app_failure_lock_)
  {
    PROPERTY_MAP::const_iterator i = options.begin ();
    while (!i.done ())
      {
        DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Impl::FaulCorrelationManager_Impl - ")
                      ACE_TEXT("Binding property %C provided by caller.\n"), i->key ().c_str ()));
        this->properties_.bind (i->key (), i->item ());
        i.advance ();
      }
  }

  FaultCorrelationManager_Impl::~FaultCorrelationManager_Impl()
  {
    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Impl::~FaultCorrelationManager_Impl (void)\n")));
    stop_ = true;
    new_notification_.signal ();

    this->wait ();
  }

  int 
  FaultCorrelationManager_Impl::svc (void)
  {
    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Impl::svc (void)\n")));

    while (!stop_)
      {
        FailureInfo fi;

        {
          ACE_Guard <ACE_Thread_Mutex> guard (app_failure_lock_);

          while (notification_queue_.empty () && !stop_)
            {
              new_notification_.wait ();
            }

          if (notification_queue_.empty ())
            continue;

          fi = notification_queue_.front ();
          notification_queue_.pop ();
        }
        
        this->app_failure_i (fi.host.c_str (), 
                             fi.application.c_str ());

        // add all logged proc_failure info to the log file
        std::ofstream out;
        out.open ("fou-shutdown.txt", ios_base::app);
        ACE_Date_Time start, end;

        {
          ACE_Guard <ACE_Thread_Mutex> guard (history_lock_);

          for (SHUTDOWN_TIME_LIST::iterator it = history_.begin ();
               it != history_.end ();
               ++it)
            {
              start.update (it->start_shutdown);
              end.update (it->end_shutdown);
              out << it->fou_id << " " 
                  << start.hour () << ":"
                  << start.minute () << ":"
                  << start.second () << ":"
                  << start.microsec () << " "
                  << end.hour () << ":"
                  << end.minute () << ":"
                  << end.second () << ":"
                  << end.microsec () << " "
                  << it->shutdown << std::endl;
            }

          history_.clear ();
        }
        
        out.close ();
      }

    return 0;
  }
  
  void
  FaultCorrelationManager_Impl::stop_failover_unit (const char * fou_id)
  {
    try
      {
        DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM::stop_failover_unit (%C)\n"),
                      fou_id));        

        Deployment::DomainApplicationManager_var dam;

        if (dams_.find (fou_id,
                        dam) != 0)
          {
            DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM::stop_failover_unit (%C): ")
                          ACE_TEXT("could not resolve DAM'.\n"),
                          fou_id));

            return;
          }

        ACE_Time_Value start (ACE_OS::gettimeofday ());

        timer_.start ();

        Deployment::Applications_var apps = dam->getApplications();

        for (size_t i = 0; i < apps->length(); ++i)
          {
            try
              {
                dam->destroyApplication(apps[i]);
              }
            catch (const CORBA::SystemException & ex)
              {
                ACE_DEBUG ((LM_WARNING,
                            "FCM: application %d already stopped.\n",
                            i));
              }
          }

        timer_.stop ();

        Deployment::DeploymentPlan_var plan = dam->getPlan ();

        this->remove_constraints (plan);

        this->destroyManager (dam.in ());

        RankListConstraints_var constraints = this->get_constraints ();
        
        rep_mgr_->set_ranklist_constraints (constraints.in ());

        DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM::stop_failover_unit (%C): ")
                      ACE_TEXT("plan was shutdown sucessfully.\n"),
                      fou_id));

        ACE_Time_Value tv;
        timer_.elapsed_time (tv);
        TFouShutdownTime sample = {tv.msec (), start, ACE_OS::gettimeofday (), fou_id};

        {
          ACE_Guard <ACE_Thread_Mutex> guard (history_lock_);
          history_.push_back (sample);
        }
      }
    catch (const CORBA::Exception & ex)
      {
        DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                                   (LM_ERROR, DLINFO
                                    ACE_TEXT("FCM::stop_failover_unit caught %n\n")
                      ACE_TEXT("      Most likely a complete node manager crashed.\n"),
                      ex._info ().c_str ()));
      }
  }

  void
  FaultCorrelationManager_Impl::app_failure (
    const char * host,
    const ::FLARE::ApplicationList & applications)
  {
    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM: app_failure ()\n")));

    ACE_Time_Value tv = ACE_OS::gettimeofday ();

    {
      ACE_Guard <ACE_Thread_Mutex> guard (app_failure_lock_);

      for (CORBA::ULong i = 0; i < applications.length (); ++i)
        {
          FailureInfo fi = {host, applications[i].in ()};
          
          notification_queue_.push (fi);
        }
    }

    new_notification_.signal ();

    TFouShutdownTime sample = {0, tv, ACE_Time_Value (), host};

    {
      ACE_Guard <ACE_Thread_Mutex> guard (history_lock_);
      history_.push_back (sample);
    }
  }

  void
  FaultCorrelationManager_Impl::app_failure_i (const char * host,
                                               const char * application)
  {
    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Impl::app_failure_i (%s, %s)\n"),
                  host,
                  application));

    TObjectIdMap node;
    if (nodes_.find (host,
                     node) != 0)
      {
        DANCE_DEBUG (DANCE_LOG_WARNING, (LM_WARNING, DLINFO
                      ACE_TEXT("FCM::app_failure (%C, %C): node '%C' not found.\n"),
                      application,
                      host,
                      host));
        return;
      }

    ACE_CString component_id = node[application];

    if (component_id.length () == 0)
      {
        DANCE_DEBUG (DANCE_LOG_WARNING, (LM_WARNING, DLINFO
                      ACE_TEXT("FCM::app_failure (%C, %C): ")
                      ACE_TEXT("application '%C' on '%C' not found.\n"),
                      application,
                      host,
                      application,
                      host));
        return;
      }

    ACE_CString plan_id;
    if (instances_.find (component_id,
                         plan_id) != 0)
      {
        DANCE_DEBUG (DANCE_LOG_WARNING, (LM_WARNING, DLINFO
                      ACE_TEXT("FCM::app_failure (%C, %C): ")
                      ACE_TEXT("plan for component '%C' not found.\n"),
                      application,
                      host,
                      component_id.c_str ()));
        return;
      }

    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM::app_failure (%C, %C): ")
                  ACE_TEXT("caused by component '%C' in plan '%C'.\n"),
                  application,
                  host,
                  component_id.c_str (),
                  plan_id.c_str ()));

    this->stop_failover_unit (plan_id.c_str ());
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

    this->add_constraints (plan);

    RankListConstraints_var constraints = this->get_constraints ();

    rep_mgr_->set_ranklist_constraints (constraints.in ());

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

    this->process_deployment_plan (plan, false);

    exec_mgr_->destroyManager (manager);
  }

  void
  FaultCorrelationManager_Impl::shutdown (void)
  {
    exec_mgr_->shutdown ();
  }

  void
  FaultCorrelationManager_Impl::process_deployment_plan (
    const Deployment::DeploymentPlan &  plan,
    bool deploy)
  {
    // add all found component instances to the map
    const Deployment::InstanceDeploymentDescription id;
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      { 
        if (deploy)
          {
            // add component with the plan id it belongs to
            instances_.bind (plan.instance[i].name.in (), 
                             plan.UUID.in ());

            DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM: instance[%C] -> plan[%C]\n"),
                          plan.instance[i].name.in (), 
                          plan.UUID.in ()));
          }
        else
          {
            // remove component entry
            instances_.unbind (plan.instance[i].name.in ());

            DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM: instance[%C] removed\n"),
                          plan.instance[i].name.in ()));

          }

        // find object_id property if existing
        CORBA::String_var object_id (
          this->get_property (CIAO::Deployment::OBJECT_ID,
                              plan.instance[i].configProperty));

        if (object_id.in () == 0)
          object_id = plan.instance[i].name.in ();

        TObjectIdMap oidmap;
        if (deploy)
          {
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

            DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM: node[%C] -> oid[%C] -> instance[%C]\n"),
                          plan.instance[i].node.in (), 
                          object_id.in (),
                          plan.instance[i].name.in ()));
          }
        else
          {
            // remove node map entry
            if (nodes_.find (plan.instance[i].node.in (),
                             oidmap) == 0)
              {
                // the new component to exisiting node map
                oidmap.erase (object_id.in ());

                DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM: node[%C] -> oid[%C] removed.\n"),
                              plan.instance[i].node.in (), 
                              object_id.in ()));

                if (oidmap.size () == 0)
                  {
                    nodes_.unbind (plan.instance[i].node.in ());

                    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FCM: node[%C] removed.\n"),
                                  plan.instance[i].node.in ()));
                  }
              }            
          }
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

  void 
  FaultCorrelationManager_Impl::add_constraints (const Deployment::DeploymentPlan & plan)
  {
    // add all found component instances to the map
    const Deployment::InstanceDeploymentDescription id;
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      { 
        // find object_id property if existing
        CORBA::String_var object_id (
          this->get_property (CIAO::Deployment::OBJECT_ID,
                              plan.instance[i].configProperty));

        if (object_id.in () == 0)
          object_id = plan.instance[i].name.in ();

        RANKLIST_CONSTRAINT constr;
        if (constraints_.find (object_id.in (),
                               constr) == 0)
          {
            // add to already existing constraint
            constr.push_back (plan.instance[i].node.in ());
            constraints_.rebind (object_id.in (),
                                 constr);
          }
        else
          {
            // create new list
            constr.push_back (plan.instance[i].node.in ());
            constraints_.bind (object_id.in (),
                               constr);
          }
      }
  }

  void 
  FaultCorrelationManager_Impl::remove_constraints (const Deployment::DeploymentPlan & plan)
  {
    // add all found component instances to the map
    const Deployment::InstanceDeploymentDescription id;
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      { 
        // find object_id property if existing
        CORBA::String_var object_id (
          this->get_property (CIAO::Deployment::OBJECT_ID,
                              plan.instance[i].configProperty));

        if (object_id.in () == 0)
          object_id = plan.instance[i].name.in ();

        RANKLIST_CONSTRAINT constr;
        if (constraints_.find (object_id.in (),
                               constr) == 0)
          {
            for (RANKLIST_CONSTRAINT::iterator it = constr.begin ();
                 it != constr.end ();
                 ++it)
              {
                if (it->compare (plan.instance[i].node.in ()) == 0)
                  {
                    constr.erase (it);

                    if (constr.size () == 0)
                      constraints_.unbind (object_id.in ());

                    break;
                  }
              }
          }
      }
  }

  RankListConstraints *
  FaultCorrelationManager_Impl::get_constraints (void)
  {
    RankListConstraints_var constraints = new RankListConstraints ();
    constraints->length (constraints_.current_size ());
    CORBA::ULong index = 0;

    for (RANKLIST_CONSTRAINT_MAP::iterator it = constraints_.begin ();
         it != constraints_.end ();
         ++it)
      {
        RankListConstraint_var constr (new RankListConstraint ());
        constr->object_id = it->key ().c_str ();
        constr->hosts.length (it->item ().size ());
        CORBA::ULong cindex = 0;

        for (RANKLIST_CONSTRAINT::iterator cit = it->item ().begin ();
             cit != it->item ().end ();
             ++cit)
          {
            constr->hosts[cindex++] = cit->c_str ();
          }

        constraints[index++] = constr;
      }

    return constraints._retn ();
  }

};

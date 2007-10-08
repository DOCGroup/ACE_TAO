// $Id$
#include "DomainDataManager.h"

#include "tools/Config_Handlers/DD_Handler.h"
#include "tools/Config_Handlers/DnC_Dump.h"
#include "ciao/CIAO_common.h"
#include <orbsvcs/CosNamingC.h>
#include "utils/Exceptions.h"


namespace CIAO
{
    DomainDataManager::DomainDataManager (CORBA::ORB_ptr orb,
                                          const char *dat_file,
                                          const char *domain_file,
                                          ::Deployment::DeploymentPlan& plan)

      : orb_ (CORBA::ORB::_duplicate (orb)),
        deployment_config_ (orb_.in()),
        delay_ (utils::Timer ("delay")),
        plan_ (plan),
        dat_file_ (dat_file),
        condition_ (condition_mutex_)

    {
      CIAO::Config_Handlers::DD_Handler dd (domain_file);
      ::Deployment::Domain* dmn = dd.domain_idl ();
      current_domain_ = *dmn;
      initial_domain_ = current_domain_;

      // set up the profile timers for each node
      for (CORBA::ULong i = 0; i < this->current_domain_.node.length (); ++i)
        {
          this->node_timers_ [this->current_domain_.node[i].name.in ()] =
            new utils::Timer (this->current_domain_.node[i].name.in ());
        }
      if (this->call_all_node_managers () != 0)
        {

          throw new utils::Initialization_Exception ();
        }
      // Wait for all the monitors to upload their obj. refs
      sleep (10);
      this->get_monitor_obj_ref ();

    }

  DomainDataManager::~DomainDataManager ()
  {}


    int
    DomainDataManager::call_all_node_managers ()
    {
      if (this->deployment_config_.init (this->dat_file_.c_str ()) == -1 )
        {
          ACE_ERROR ((LM_ERROR,
                      "TM::DomainDataManager.cpp:"
                      "call_all_node_managers(): "
                      "ERROR while trying to initialize after reading "
                      "node details DAT file \n"));
          return -1;
        }

      for (CORBA::ULong i = 0; i < this->initial_domain_.node.length (); ++i)
        {
          ::Deployment::NodeManager_var node_manager;
          try
            {
              node_manager =
                deployment_config_.get_node_manager (this->initial_domain_.node[i].name.in ());
            }
          catch (CORBA::Exception& ex)
            {
              std::string msg = "TM::DomainDataManager.cpp:call_all_node_managers(): "
                                "Error trying to contact nodemanager ";
              msg += this->initial_domain_.node[i].name.in ();
              msg += "\n";
              ACE_PRINT_EXCEPTION (ex, msg.c_str ());
              return -1;
            }


          if (!CORBA::is_nil (node_manager.in ()))
            {
              Deployment::Logger_ptr log =
                Deployment::Logger::_nil ();
              ::Deployment::Domain sub_domain;
              sub_domain.UUID = CORBA::string_dup("Node-Level-domain");
              sub_domain.label = CORBA::string_dup("Node-level-domain");
              sub_domain.sharedResource.length(0);
              sub_domain.interconnect.length(0);
              sub_domain.bridge.length(0);
              sub_domain.infoProperty.length(0);
              sub_domain.node.length (1);
              sub_domain.node[0] = initial_domain_.node[i];
              try
                {
                  node_manager->joinDomain (sub_domain, NULL, log);
                }
              catch (CORBA::Exception& ex)
                {
                  std::string msg = "TM::DomainDataManager.cpp:call_all_node_managers(): "
                                    "Error in invoking join domaintrying on ";
                  msg += this->initial_domain_.node[i].name.in ();
                  msg += "\n";
                  ACE_PRINT_EXCEPTION (ex, msg.c_str ());
                  return -1;
                }
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "TM::DomainDataManager.cpp:"
                          "call_all_node_managers(): "
                          "Object ref. for %s is nil!\n",
                          this->initial_domain_.node[i].name.in ()));
              return -1;
            }
        }
      return 0;
    }


    int
    DomainDataManager::update_domain (const ::CORBA::StringSeq &,
                                      const ::Deployment::Domain & domainSubset,
                                      ::Deployment::DomainUpdateKind /*update_kind*/)
    {
      //check the type of update ..
      return this->update_dynamic (domainSubset);

      //   switch (update_kind)
      //     {
      //       case ::Deployment::UpdateAll:
      //       case ::Deployment::UpdateDynamic:
      //         this->update_dynamic (domainSubset);
      //         break;

      //       case ::Deployment::Add:
      // 	ex_occur = true;
      //         break;
      //       default:
      //         return 1;
      //     }
    }

    int
    DomainDataManager::update_dynamic (const ::Deployment::Domain &domainSubset)
    {
      this->node_info_map_ [domainSubset.node[0].name.in ()] = domainSubset.node[0];
      this->node_timers_[domainSubset.node[0].name.in ()]->stop ();

      // Acquire the mutex before making any modifications as multiple
      // threads might try to do this simultaneously.
      this->mutex_.acquire ();
      CORBA::ULong temp_count = --this->response_count_;
      this->mutex_.release ();

      //      ACE_DEBUG ((LM_DEBUG, "Waiting for %u replies to come in.\n", temp_count));
      if (temp_count == 0)
        {
          // all responses have come in .... now timestamp the event
          this->delay_.stop ();
          this->delay_.dump ();
          this->condition_.signal ();

          for (std::map<std::string, utils::Timer*>::iterator itr = this->node_timers_.begin ();
               itr != this->node_timers_.end ();
               itr++)
            {
              (*itr).second->dump ();

            }
        }

      //       CORBA::Double load;
      //       domainSubset.node[0].resource[0].property[0].value >>= load;
      //       ACE_DEBUG ((LM_DEBUG, "The current load is %f\n", load));
      //     CORBA::ULong response_time;
      //     domainSubset.node[0].qos_seq[0].value >>= response_time;
      //     ACE_DEBUG ((LM_DEBUG, "The response time is %u\n", response_time));

      return 0;
    }

  void
    CIAO::DomainDataManager::stop_monitors ()
    {

      for (CORBA::ULong i = 0; i < this->initial_domain_.node.length (); i++)
          {
            ::Deployment::NodeManager_var node_manager;
            try
              {
                node_manager =
                  this->deployment_config_.get_node_manager
                  (this->initial_domain_.node[i].name.in ());
              }
            catch (CORBA::Exception& ex)
              {
                std::string msg = "TM::DomainDataManager.cpp:call_all_node_managers(): "
                "Error trying to contact nodemanager ";
                msg += this->initial_domain_.node[i].name.in ();
                msg += "\n";
                ACE_PRINT_EXCEPTION (ex, msg.c_str ());
                continue;
              }


            if (!CORBA::is_nil (node_manager.in ()))
              {
                try
                  {
                    node_manager->leaveDomain ();
                  }
                catch (CORBA::Exception& ex)
                  {
                    std::string msg = "TM::DomainDataManager.cpp:call_all_node_managers(): "
                    "Error in invoking leavedomain on ";
                    msg += this->initial_domain_.node[i].name.in ();
                    msg += "\n";
                    ACE_PRINT_EXCEPTION (ex, msg.c_str ());
                  }
              }
          }
      return;
    }


    int
    CIAO::DomainDataManager::get_monitor_obj_ref ()
    {
      try
        {

          // here we have to extract the monitors from the NS
          CORBA::Object_var naming_context_object =
            this->orb_->resolve_initial_references ("NameService");

          CosNaming::NamingContext_var naming_context =
            CosNaming::NamingContext::_narrow (naming_context_object.in ());

          for (CORBA::ULong i = 0; i < this->initial_domain_.node.length (); ++i)
            {
              CosNaming::Name name (2);
              name.length (2);
              name[0].id = CORBA::string_dup ("Node_Monitor");
              name[1].id = CORBA::string_dup (initial_domain_.node[i].name);
              CORBA::Object_var object =
                naming_context->resolve (name);
              this->node_monitors_.push_back (Onl_Monitor::NM_Monitor::_narrow (object.in ()));
            }
          return 0;
        }
      catch (CORBA::Exception &ex)
        {

          ACE_PRINT_EXCEPTION (ex, "TM::DomainDataManager.cpp:get_monitor_obj_ref(): "
                               "Exception caught.");
          return -1;
        }
    }


  std::map<std::string, ::Deployment::Node>
  CIAO::DomainDataManager::get_all_data
  (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler)
    {
      this->response_count_ = this->current_domain_.node.length ();
      this->delay_.start ();

      for (CORBA::ULong i = 0; i < this->node_monitors_.size (); ++i)
        {

          this->node_timers_[this->current_domain_.node[i].name.in ()]->start ();
          this->node_monitors_[i]->sendc_get_resource_data (handler);
        }
      this->condition_.wait ();
      return this->node_info_map_;

    }


    int
    CIAO::DomainDataManager::
    start_monitor_qos (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler)
    {
      for (CORBA::ULong i = 0; i < this->node_monitors_.size (); ++i)
        {
          this->node_monitors_[i]->sendc_monitor_app_QoS (handler,
                                                          this->plan_);
        }
      return 0;

    }
  }

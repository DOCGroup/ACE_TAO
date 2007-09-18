// $Id$
#include "DomainDataManager.h"

#include "tools/Config_Handlers/DD_Handler.h"
#include "tools/Config_Handlers/DnC_Dump.h"
#include "ciao/CIAO_common.h"

#include "Profile_Code.h"

#include <orbsvcs/CosNamingC.h>

const char * domain_file_name = "Domain.cdd";

CIAO::DomainDataManager* CIAO::DomainDataManager::global_data_manager_ = 0;

CIAO::DomainDataManager *
CIAO::DomainDataManager::create (CORBA::ORB_ptr orb,
                                 ::Deployment::TargetManager_ptr target,
                                 const char* dat_file)
{
  if (global_data_manager_ == 0)
    {
      global_data_manager_ = new DomainDataManager (orb , target, dat_file);
    }
  return global_data_manager_;
}

// Returns the pointer to the static variable
CIAO::DomainDataManager*
CIAO::DomainDataManager::get_data_manager ()
{
  return global_data_manager_;
}


void
CIAO::DomainDataManager::delete_data_manger ()
{
  if (global_data_manager_)
    delete global_data_manager_;
}

int CIAO::DomainDataManager::update_domain (
                             const ::CORBA::StringSeq &,
                             const ::Deployment::Domain & domainSubset,
                             ::Deployment::DomainUpdateKind update_kind)
{
  // Update the subset of the domain which the above
  // parameter corresponds to

  //  ACE_DEBUG ((LM_DEBUG, "[TM] Update domain called\n"));

  //check the type of update ..

  this->update_dynamic (domainSubset);

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
  return 0;

}

int
CIAO::DomainDataManager::update_dynamic (const ::Deployment::Domain
                                         &domainSubset)
{
  CORBA::ULong const size = current_domain_.node.length ();
  this->node_info_map_ [domainSubset.node[0].name.in ()] = domainSubset.node[0];

  int temp_count;

  this->mutex_.acquire ();
  this->response_count_++;
  temp_count = this->response_count_;
  this->mutex_.release ();

  if (temp_count == current_domain_.node.length ())
    {
      // all responses have come in .... now time stamp the event
      this->profile_->stop ();
      this->profile_->dump ();
      for (std::map<std::string, Profile_Code*>::iterator itr = this->profile_nodes_.begin ();
           itr != this->profile_nodes_.end ();
           itr++)
        {
          (*itr).second->dump ();

        }
      this->got_all_response_ = true;

    }

//       CORBA::Double load;
//       domainSubset.node[0].resource[0].property[0].value >>= load;
//       ACE_DEBUG ((LM_DEBUG, "The current load is %f\n", load));


  profile_nodes_[domainSubset.node[0].name.in ()]->stop ();


//     CORBA::ULong response_time;

//     domainSubset.node[0].qos_seq[0].value >>= response_time;

//     ACE_DEBUG ((LM_DEBUG, "The response time is %u\n", response_time));

  return 0;
}

CIAO::DomainDataManager::
DomainDataManager (CORBA::ORB_ptr orb,
                   ::Deployment::TargetManager_ptr target,
		   const char* dat_file)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    deployment_config_ (orb_.in()),
    dat_file_ (dat_file),
    profile_ (new Profile_Code ("Tst"))
{
  CIAO::Config_Handlers::DD_Handler dd (domain_file_name);
  ::Deployment::Domain* dmn = dd.domain_idl ();

  if (CIAO::debug_level () > 9)
    ::Deployment::DnC_Dump::dump (*dmn);

  current_domain_ = *dmn;
  initial_domain_ = current_domain_;

  //  update_node_status ();

  // set up the profile timers for each node

  for (int i = 0;i < current_domain_.node.length ();i++)
    {
      profile_nodes_[current_domain_.node[i].name.in ()] =
	new Profile_Code (current_domain_.node[i].name.in ());
    }

  call_all_node_managers ();
}

int CIAO::DomainDataManager::readin_domain_data ()
{
  // here read in Domain data ...
  //
  return 0;
}

int CIAO::DomainDataManager::call_all_node_managers ()
{
  if ( this->deployment_config_.init (this->dat_file_.c_str ()) == -1 )
    {
      ACE_ERROR ((LM_ERROR,
                  "TargetM (%P|%t) DomainDataManager.cpp -"
                  "CIAO::DomainDataManager::call_all_node_managers -"
                  "ERROR while trying to initialize after reading "
                  "node details DAT file \n"));
      return 0;
    }

  CORBA::ULong const length = initial_domain_.node.length ();

  for (CORBA::ULong i=0;i < length;i++)
    {

      ::Deployment::NodeManager_var node_manager;

      try
        {
          node_manager =
            deployment_config_.get_node_manager
            (initial_domain_.node[i].name.in ());
        }
      catch (CORBA::Exception&)
        {
          ACE_ERROR ((LM_ERROR, "DANCE::TM (%P|%t) DomainDataManager.cpp: "
                      "Error trying to contact NodeManager %s\n",
                      initial_domain_.node[i].name.in ()));
          continue;
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
	      ACE_DEBUG ((LM_DEBUG, "CAlling Join DOmain\n"));
              node_manager->joinDomain (sub_domain,
                                        NULL,
                                        log);
            }
          catch (CORBA::Exception& ex)
            {
              ACE_ERROR ((LM_ERROR , "TM::Error in calling Join Domain==\n"));
              ex._tao_print_exception (
                "Exception caught in ""DomainDataManager::joinDomain");
            }
        }
    }
  return 0;

}


void CIAO::DomainDataManager::stop_monitors ()
{

  CORBA::ULong length = initial_domain_.node.length ();

  for (CORBA::ULong i=0;i < length;i++)
    {
      ::Deployment::NodeManager_var node_manager;

      try
        {
          node_manager =
            deployment_config_.get_node_manager
            (initial_domain_.node[i].name.in ());
        }
      catch (CORBA::Exception&)
        {
          ACE_ERROR ((LM_ERROR, "DANCE::TM (%P|%t) DomainDataManager.cpp: "
                      "Error in get Node Manager from Deployment Config %s\n",
                      initial_domain_.node[i].name.in ()));
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
              ACE_ERROR ((LM_ERROR , "TM::Error in calling Leave Domain\n"));
              ex._tao_print_exception (
                "Exception caught in ""DomainDataManager::leaveDomain");
            }
        }
    }
  return;

}


int CIAO::DomainDataManager::get_monitor_obj_ref ()
{
  // here we have to extract the monitors from the NS
  CORBA::Object_var naming_context_object =
    orb_->resolve_initial_references ("NameService");

  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_context_object.in ());

  for (CORBA::ULong i = 0;i < initial_domain_.node.length ();i++)
  {
    CosNaming::Name name (2);
    name.length (2);
    name[0].id = CORBA::string_dup ("Node_Monitor");
    name[1].id = CORBA::string_dup (initial_domain_.node[i].name);

    CORBA::Object_var object =
      naming_context->resolve (name);
    node_monitors_.push_back (Onl_Monitor::NM_Monitor::_narrow (object.in ()));
  }
  return 0;

}

void
CIAO::DomainDataManager::get_all_data (Onl_Monitor::AMI_NM_MonitorHandler_ptr
                                       handler)
{
  this->got_all_response_ = false;
  this->mutex_.acquire ();
  this->response_count_ = 0;
  this->mutex_.release ();


  this->profile_->start ();

  for (int i = 0;i < node_monitors_.size ();i++)
  {

    profile_nodes_[current_domain_.node[i].name.in ()]->start ();
    node_monitors_[i]->sendc_get_resource_data (handler);
  }
}

bool CIAO::DomainDataManager::got_all_response ()
{
  return got_all_response_;
}

int
CIAO::DomainDataManager::
start_monitor_qos (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler,
                   ::Deployment::DeploymentPlan& plan)
{
  for (int i = 0;i < node_monitors_.size ();i++)
  {
    node_monitors_[i]->sendc_monitor_app_QoS (handler, plan);
  }
  return 0;

}

void
CIAO::DomainDataManager::
write_snapshot ()
{

  if (ex_occur)
    return;
  std::ofstream out ("Snapshot", std::ios::app);

  out << "----------------------------------------------------"
      << std::endl;

  for (CORBA::ULong i = 0;
       i < this->current_domain_.node.length ();
       i++)
    {
      out << "Node"
          << "\t"
          << this->current_domain_.node[i].name.in ()
          << std::endl;

      CORBA::ULong response_time;

      current_domain_.node[i].qos_seq[0].value >>= response_time;

      CORBA::Double cpu_load;
      CORBA::Double na_load;

      for (CORBA::ULong j =0;
           j < current_domain_.node[i].resource.length ();
           j++)
        {
          if (!strcmp (current_domain_.node[0].resource[j].name.in (), "NA_Monitor"))
            current_domain_.node[i].resource[j].property[0].value >>= na_load;

          if (!strcmp (current_domain_.node[0].resource[j].name.in (), "Processor"))
            current_domain_.node[i].resource[j].property[0].value >>= cpu_load;
        }

      out << cpu_load
          << "\t"
          << na_load
          << "\t"
          << response_time
          << std::endl;
    }

  out.close ();
}

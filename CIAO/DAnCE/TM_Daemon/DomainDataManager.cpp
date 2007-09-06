// $Id$
#include "DomainDataManager.h"

#include "tools/Config_Handlers/DD_Handler.h"
#include "tools/Config_Handlers/DnC_Dump.h"
#include "ciao/CIAO_common.h"

#include "Profile_Code.h"

#include <orbsvcs/CosNamingC.h>

#include <fstream>

const char * domain_file_name = "Domain.cdd";

CIAO::DomainDataManager* CIAO::DomainDataManager::global_data_manager_ = 0;

CIAO::DomainDataManager * CIAO::DomainDataManager::create (CORBA::ORB_ptr orb,
                            ::Deployment::TargetManager_ptr target,
			    const char* dat_file
                            )
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

  ACE_DEBUG ((LM_DEBUG, "THe update domain called\n"));

  //check the type of update ..

  ex_occur = false;

  switch (update_kind)
    {
      case ::Deployment::UpdateAll:
      case ::Deployment::UpdateDynamic:
        break;
      case ::Deployment::Add:
	ex_occur = true;
        //add_to_domain (domainSubset);
        break;
      case ::Deployment::Delete:
        delete_from_domain (domainSubset);
        break;
      default:
        break;
    }

  CORBA::ULong const size = current_domain_.node.length ();

  if (ex_occur == false)
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock, 0);

    Profile_Code prfTagg ("Tagg");

    prfTagg.start ();
    
    CORBA::ULong i;
    for (i=0;i < size;i++)
      {
	if (!ACE_OS::strcmp (domainSubset.node[0].name ,
			     current_domain_.node[i].name))
	  {
	    // found a match
	    // for now overwrite the entire Node info ...
	    // but later , this has to be changed to overwrite
	    // only the specific part ...
	    current_domain_.node[i] = domainSubset.node[0];
	    
	    CORBA::Double load;
	    current_domain_.node[i].resource[0].property[0].value >>= load;
	    ACE_DEBUG ((LM_DEBUG, "The current load is %f\n", load));
	    break; // finished job ...break
	  }
      }
    if (i == size)
      {
	// thus the node is new .. add it to current_domain_
	// later change it ...
	current_domain_.node.length (size+1);
	current_domain_.node[size]=domainSubset.node[0];
      }
    prfTagg.stop ();
    
  }


  profile_nodes_[domainSubset.node[0].name.in ()]->stop ();

  this->response_count_++;

  if (this->response_count_ == current_domain_.node.length ())
    {
      this->got_all_response_ = true;

      // all responses have come in .... now time stamp the event
      this->profile_->stop ();

      // now delete the object then file will be closed ..
      delete this->profile_;
    }

	if (ex_occur = true)
		return 0;

  CORBA::ULong response_time;

  domainSubset.node[0].qos_seq[0].value >>= response_time;

  ACE_DEBUG ((LM_DEBUG, "The response time is %u\n", response_time));

/*
  CORBA::Double cpu_load;
  CORBA::Double na_load;

  for (CORBA::ULong i =0;
       i < domainSubset.node[0].resource.length ();
       i++)
    {
      if (!strcmp (domainSubset.node[0].resource[i].name.in (), "NA_Monitor"))
        domainSubset.node[0].resource[i].property[0].value >>= na_load;

      if (!strcmp (domainSubset.node[0].resource[i].name.in (), "Processor"))
        domainSubset.node[0].resource[i].property[0].value >>= cpu_load;
    }
*/
//   std::ofstream out (domainSubset.node[0].name.in (), std::ios::app);
//   out << cpu_load
//       << "\t"
//       << na_load
//       << "\t"
//       << response_time
//       << std::endl;

//   out.close ();

  return 0;
}

CIAO::DomainDataManager::
DomainDataManager (CORBA::ORB_ptr orb,
                   ::Deployment::TargetManager_ptr target,
		   const char* dat_file)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    deployment_config_ (orb_.in()),
    target_mgr_ (::Deployment::TargetManager::_duplicate(target)),
    dat_file_ (dat_file)
{
  CIAO::Config_Handlers::DD_Handler dd (domain_file_name);
  ::Deployment::Domain* dmn = dd.domain_idl ();

  if (CIAO::debug_level () > 9)
    ::Deployment::DnC_Dump::dump (*dmn);

  current_domain_ = *dmn;
  initial_domain_ = current_domain_;

  // initialize the provisioning domain
  provisioned_data_ = initial_domain_;

  update_node_status ();

  // set up the profile timers for each node 

  for (int i = 0;i < current_domain_.node.length ();i++)
    {
      profile_nodes_[current_domain_.node[i].name.in ()] =
	new Profile_Code (current_domain_.node[i].name.in ());		
    }



  call_all_node_managers ();
}

::Deployment::Domain* CIAO::DomainDataManager::get_current_domain ()
{
  return new ::Deployment::Domain (provisioned_data_);
}

::Deployment::Domain* CIAO::DomainDataManager::get_initial_domain ()
{
  return new ::Deployment::Domain (initial_domain_);
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
                                        target_mgr_.in (),
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


void CIAO::DomainDataManager
::commitResources (
                   const ::Deployment::DeploymentPlan & plan)
{
  // commit the resources
  // parse into the plan and commit resources ...

  // set the action value
  current_action_ = commit;

  // temporary created to guard against exceptions
  ::Deployment::Domain temp_provisioned_data =
      provisioned_data_;

  for (CORBA::ULong i = 0;i < plan.instance.length ();i++)
    {
      for (CORBA::ULong j = 0;j < temp_provisioned_data.node.length ();j++)
        {
          if (!ACE_OS::strcmp (plan.instance[i].node.in () ,
                       temp_provisioned_data.node[j].name.in ()))
          {
            try {
              match_requirement_resource (
                                plan.instance[i].deployedResource,
                                temp_provisioned_data.node[j].resource);
            }
            catch (::Deployment::ResourceCommitmentFailure& ex)
              {
                // catch the exception and add parameters
                throw ex;
              }
          }
        }
    }

  // here commit the commitresources
  provisioned_data_ = temp_provisioned_data;
}


void CIAO::DomainDataManager::
releaseResources (
                  const ::Deployment::DeploymentPlan& plan)
{
  // release the resources


  // set the action value
  current_action_ = release;

  for (CORBA::ULong i = 0;i < plan.instance.length ();i++)
    {
      for (CORBA::ULong j = 0;j < provisioned_data_.node.length ();j++)
        {
          if (!ACE_OS::strcmp (plan.instance[i].node.in () ,
                               provisioned_data_.node[j].name.in ()))
          {
            match_requirement_resource (
                                        plan.instance[i].deployedResource,
                                        provisioned_data_.node[j].resource);

          }
        }
    }

}


void CIAO::DomainDataManager::
match_requirement_resource (
      ::Deployment::InstanceResourceDeploymentDescriptions deployed,
      ::Deployment::Resources & available
         )
{
  // here match the deployed to the available

  for (CORBA::ULong i = 0;i < deployed.length ();i++)
    {
      // for each deployed resource ....search the corresponding
      // available resource
      for (CORBA::ULong j = 0;j < available.length ();j++)
        {
          if (!ACE_OS::strcmp (deployed[i].requirementName, available[j].name))
            {
              if (CIAO::debug_level () > 9)
              // search for the resourcename in the resourceType
              for (CORBA::ULong k = 0;k < available[j].resourceType.length ();k++)
                {
                  if (!ACE_OS::strcmp (deployed[i].resourceName,
                               available[j].resourceType[k]))
                    {
                      try {
                        match_properties (deployed[i].property,
                                          available[j].property);
                      }
                      catch (::Deployment::ResourceCommitmentFailure& ex)
                        {
                          // catch the exception and add parameters
                          throw ex;
                        }
                    }
                }
            }
        }
    }

}

void CIAO::DomainDataManager::
match_properties (
      ::Deployment::Properties deployed,
      ::Deployment::SatisfierProperties & available)
{
  bool property_found;

  for (CORBA::ULong i = 0;i < deployed.length ();i++)
    {
      property_found = false;

      for (CORBA::ULong j = 0;j < available.length ();j++)
      {
        if (!ACE_OS::strcmp (deployed[i].name , available[j].name))
          {
            // check kind here ....and then subtract ....
            // accordingly , ..this is complex ... better to write
            // some specialised algo
            // for now assuming Capacity ....
            // and tk_double ....

              commit_release_resource (deployed[i] , available[j]);
              property_found = true;
          }
      } // internal for ....

      // check if property was found or not
      if (property_found == false)
      {
        // throw an error since property was not found in the Resource
        ::Deployment::ResourceCommitmentFailure failure;

        failure.reason = CORBA::string_dup ("Property Not Found\n");
        failure.propertyName = CORBA::string_dup (deployed[i].name);
        failure.propertyValue.length (0);

        throw failure;
      }
    } // outside for ...
}

void CIAO::DomainDataManager::commit_release_resource (
           ::Deployment::Property & deployed,
           ::Deployment::SatisfierProperty & available)
{
  if (current_action_ == commit)
    {

      CORBA::Long required_d;

      if ((deployed.value >>= required_d) == false)
        ACE_ERROR ((LM_ERROR, "Failed to extract required amount\n"));

      CORBA::Long available_d;

      if ((available.value >>= available_d) == false)
        ACE_ERROR ((LM_ERROR, "failed to extract available amount\n"));

      if (available_d >= required_d)
        {
          available_d = available_d - required_d;

          available.value <<= available_d;
        }
      else
        {
          ::Deployment::ResourceCommitmentFailure failure;

          failure.reason = CORBA::string_dup ("Insufficient resources!");
          failure.propertyName = CORBA::string_dup (available.name);
          failure.propertyValue.length (1);
          failure.propertyValue[0] = available.value;

          throw failure;
        }
    }
  else
    {
      //must be release
      // @todo check return value of >>=
      CORBA::Long required_d;
      deployed.value >>= required_d;
      CORBA::Long available_d;
      available.value >>= available_d;

      available_d = available_d + required_d;

      // Should we check for bin > 100 ??????

      available.value <<= available_d;
    }
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

int CIAO::DomainDataManager::add_to_domain (
    const ::Deployment::Domain& domain)
{
  // here add the domain to the Domain
  // right now use only a node

  // got to take care of the fact , that a node can be added ,
  // while it is still in the domain

  //iterate through the supplied domain
  //for each node
  //  find it in the pristine domain
  //  and copy it back to the provisioned_domain

  for (CORBA::ULong i = 0;i < domain.node.length ();i++)
  {
      //find in the pristine domain
      ::Deployment::Node a_node;

      if (!this->find_in_initial_domain (domain.node[i].name.in (),
                                        a_node))
        continue; // dont know this node

      //check if already present
      if (!this->find_in_provisioned_domain (domain.node[i].name.in (),
                                             a_node))
        {
          // add the node to the domain ...
          provisioned_data_.node.length (provisioned_data_.node.length () + 1);
          provisioned_data_.node[provisioned_data_.node.length () - 1] =
            a_node;
        }
  }

  return 0;
}

bool CIAO::DomainDataManager::
find_in_initial_domain (const char* node_name,
                        ::Deployment::Node& node)
{
  for (CORBA::ULong i =0;
      i < this->initial_domain_.node.length ();
      i++)
  {
    if (ACE_OS::strcmp (node_name, this->initial_domain_.node[i].name.in ()) == 0)
    {
      node = this->initial_domain_.node[i];
      return true;
    }
  }

  // not found the node , return a node with an empty name
  return false;
}


bool CIAO::DomainDataManager::
find_in_provisioned_domain (const char* node_name,
                        ::Deployment::Node& node)
{
  for (CORBA::ULong i =0;
      i < this->provisioned_data_.node.length ();
      i++)
  {
    if (ACE_OS::strcmp (node_name, this->provisioned_data_.node[i].name.in ()) == 0)
    {
      node = this->provisioned_data_.node[i];
      return true;
    }
  }

  // not found the node , return a node with an empty name
  return false;
}

int CIAO::DomainDataManager::delete_from_domain (
    const ::Deployment::Domain& domain)
{
  // validate input
  if (domain.node.length () == 0)
    return 1;

  if (domain.node.length () >
      this->provisioned_data_.node.length ())
    return 0;

  //algo : parse through the provisioned_data
  // for each node , find in the deleted domain list
  // if not found add it to the updated nodes list

  ::Deployment::Nodes updated_nodes;
  bool found = false;

  for (CORBA::ULong j = 0;
       j < this->provisioned_data_.node.length ();
       j++)
    {
      found = false;

      for (CORBA::ULong i = 0;i < domain.node.length ();i++)
        {
          if (ACE_OS::strcmp (domain.node[i].name.in (),
                      this->provisioned_data_.node[j].name.in ()) == 0)
            {
              found = true;
              break; // found the node
            }
        }
      if (found)
        continue;

      // not found in the deleted list

      // update the length of the list
      updated_nodes.length (updated_nodes.length () + 1);

      // copy the node info
      updated_nodes[updated_nodes.length () - 1] =
        this->provisioned_data_.node[j];

    } // for provisioned_data

  // here update the provisioned data
  this->provisioned_data_.node = updated_nodes;

  return 1;
}

int CIAO::DomainDataManager::intimate_planner (
    const ::Deployment::Domain& domain)
{
 // use the connection with the planner and get a reference to the planner
 // make a call top the planner
  Deployment::Domain d = domain;
  return 0;

}

bool CIAO::DomainDataManager::update_node_status ()
{
  // update the node status here ...
  return 0;
}

void CIAO::DomainDataManager::commitResourceAllocation (
          const ::Deployment::ResourceAllocations & resources)
{
  // commit the resources
  // parse into the plan and commit resources ...

  // set the action value
  current_action_ = commit;

  this->commit_release_RA (resources);
}

void CIAO::DomainDataManager::releaseResourceAllocation (
    const ::Deployment::ResourceAllocations & resources)
{
  // set the action value
  current_action_ = release;

  this->commit_release_RA (resources);
}


int CIAO::DomainDataManager::
commit_release_RA (const ::Deployment::ResourceAllocations& resources)
{
  // temporary used to guard against exceptions
  temp_provisioned_data_ = provisioned_data_;


  for (CORBA::ULong i = 0;i < resources.length ();i++)
  {
    try
    {
      ::Deployment::Resource& res = find_resource (resources[i]);

      match_properties (resources[i].property , res.property);
    }
    catch (::Deployment::ResourceCommitmentFailure& ex)
    {
      // catch the exception and add parameters
      ACE_ERROR ((LM_ERROR, "Caught the Exception in releaseResourceAllocation\n"));
      ex.index = i;
      throw ex;
    }
  }

  // here commit the commitresources
  provisioned_data_ = temp_provisioned_data_;

  return 0;
}

::Deployment::Resource&
CIAO::DomainDataManager::find_resource (
    const ::Deployment::ResourceAllocation& resource)
{
  // for now search the resource in the Node sequence; Later need
  // to add it to the Bridges and Interconnects too according to the
  // spec
    for (CORBA::ULong j = 0;j < this->temp_provisioned_data_.node.length ();j++)
    {
      if (!ACE_OS::strcmp (resource.elementName.in () ,
            this->temp_provisioned_data_.node[j].name.in ()))
      {
        for (CORBA::ULong k =0;
              k < this->temp_provisioned_data_.node[j].resource.length ();
              k++)
        {
          if (!ACE_OS::strcmp (this->temp_provisioned_data_.node[j].resource[k].name.in (),
                resource.resourceName.in ()))
            return this->temp_provisioned_data_.node[j].resource[k];//resource found here, return
        }

        // resource not found
        throw ::Deployment::ResourceCommitmentFailure ().reason = CORBA::string_dup ("Resource Not Found\n");
      }
    }
    throw ::Deployment::ResourceCommitmentFailure ().reason = CORBA::string_dup ("Resource Not Found\n");
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
}

void CIAO::DomainDataManager::get_all_data (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler)
{
  this->got_all_response_ = false;
  this->response_count_ = 0;

  Profile_Code profiler ("Tout");

  this->profile_ = new Profile_Code ("Tst");

  this->profile_->start ();



  for (int i = 0;i < node_monitors_.size ();i++)
  {
    profiler.start ();
   
    profile_nodes_[current_domain_.node[i].name.in ()]->start ();
    node_monitors_[i]->sendc_get_resource_data (handler);
    
    profiler.stop ();
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

// $Id$
//===============================================================
/**
 * @file DomainDataManager.cpp
 *
 * @brief Maintains the Domain Information
 *
 * It contains the entire Domain information.Both the
 * initial domain as well as the current available domain.
 *
 * @author Nilabja Roy nilabjar@dre.vanderbilt.edu
 */
//===============================================================
#include "DomainDataManager.h"

#include "Config_Handlers/DD_Handler.h"
#include "Config_Handlers/DnC_Dump.h"
#include "ciao/CIAO_common.h"

const char * domain_file_name = "Domain.cdd";

CIAO::DomainDataManager* CIAO::DomainDataManager::global_data_manager_ = 0;

CIAO::DomainDataManager * CIAO::DomainDataManager::create (CORBA::ORB_ptr orb,
                            ::Deployment::TargetManager_ptr target
                            )
{
  if (global_data_manager_ == 0)
    {
      global_data_manager_ = new DomainDataManager (orb , target);
    }
  return global_data_manager_;
}


CIAO::DomainDataManager* CIAO::DomainDataManager::get_data_manager ()
{
  return global_data_manager_;
}


void CIAO::DomainDataManager::delete_data_manger ()
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


  //check the type of update ..

  switch (update_kind)
    {
      case ::Deployment::UpdateAll:
      case ::Deployment::UpdateAvailable:
        break;
      case ::Deployment::Add:
        add_to_domain (domainSubset);
        break;
      case ::Deployment::Delete:
        delete_from_domain (domainSubset);
        break;
      default:
        break;
    }

  int size = current_domain_.node.length ();

  int i;
  for (i=0;i < size;i++)
    {
      if (!strcmp (domainSubset.node[0].name ,
                   current_domain_.node[i].name))
        {
          // found a match
          // for now overwrite the entire Node info ...
          // but later , this has to be changed to overwrite
          // only the specific part ...
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG , "TM::Changed the cpu Value\n"));
            }
          current_domain_.node[i] = domainSubset.node[0];
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
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG ,
          "TM::Inside The update Domain of Manager\n"));
    }
  return 0;
}

CIAO::DomainDataManager::
DomainDataManager (CORBA::ORB_ptr orb,
                   ::Deployment::TargetManager_ptr target)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    deployment_config_ (orb_.in()),
    target_mgr_ (::Deployment::TargetManager::_duplicate(target))
{
  //  ACE_DEBUG((LM_DEBUG , "Calling DD_HANDLER\n"));
  CIAO::Config_Handlers::DD_Handler dd (domain_file_name);
  //  ACE_DEBUG((LM_DEBUG , "After DD_HANDLER Constructor\n"));
  ::Deployment::Domain* dmn = dd.domain_idl ();
  //  ACE_DEBUG((LM_DEBUG , "After DD_HANDLER domain_idl\n"));

  if (CIAO::debug_level () > 9)
    ::Deployment::DnC_Dump::dump (*dmn);

  current_domain_ = *dmn;
  initial_domain_ = current_domain_;

  // initialize the provisioning domain
  provisioned_data_ = initial_domain_;

  update_node_status ();

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
  if ( this->deployment_config_.init ("NodeDetails.dat") == -1 )
    {
      ACE_ERROR ((LM_ERROR,
                  "TargetM (%P|%t) DomainDataManager.cpp -"
                  "CIAO::DomainDataManager::call_all_node_managers -"
                  "ERROR while trying to initialize after reading "
                  "node details DAT file \n"));
      return 0;
    }

  int length = initial_domain_.node.length ();
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG, "Number of nodes in domain.cdd is : %d\n", length));
    }

  for (int i=0;i < length;i++)
    {

      ::Deployment::NodeManager_var node_manager;


      try
        {
          node_manager =
            deployment_config_.get_node_manager
            (initial_domain_.node[i].name.in ());
        }
      catch (CORBA::Exception& ex)
        {
          ACE_ERROR ((LM_ERROR, "DANCE::TM (%P|%t) DomainDataManager.cpp: "
                      "Error trying to contact NodeManager %s\n",
                      initial_domain_.node[i].name.in ()));
          continue;
        }


      if (!CORBA::is_nil (node_manager.in ()))
        {
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG, "Trying to contact nodemanager on %s\n",
                          initial_domain_.node[i].name.in ()));
            }
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
              node_manager->joinDomain (sub_domain,
                                        target_mgr_.in (),
                                        log);
            }
          catch (CORBA::Exception& ex)
            {
              ACE_DEBUG ((LM_DEBUG , "TM::Error in calling Join Domain==\n"));
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in "
                                   "DomainDataManager::joinDomain");
            }
        }
    }
  return 0;

}

CIAO::Host_NodeManager_seq * CIAO::DomainDataManager::get_node_managers ()
{
  ::CIAO::Host_NodeManager_seq* node_mgr_seq =
      new ::CIAO::Host_NodeManager_seq ();
  node_mgr_seq->length (initial_domain_.node.length ());
  for (unsigned int i=0;i < initial_domain_.node.length ();i++)
    {
      (*node_mgr_seq)[i].host_ =
        CORBA::string_dup (initial_domain_.node[i].name);
      ::Deployment::NodeManager_var node_manager =
          deployment_config_.get_node_manager (initial_domain_.node[i].name);
      //      if (node_manager.in () != 0)
        {
          (*node_mgr_seq)[i].node_mgr_ = ::CIAO::NodeManagerDaemon::_narrow (node_manager.in ());
        }
    }
  return node_mgr_seq;
}

CIAO::Host_Infos*  CIAO::DomainDataManager::get_cpu_info ()
{
  CIAO::Host_Infos* host_info_seq = new CIAO::Host_Infos ();
  host_info_seq->length (current_domain_.node.length ());

  if (CIAO::debug_level () > 9)
    ACE_DEBUG ((LM_DEBUG , "TM:: The node length is [%d]",
                current_domain_.node.length ()));

  for (unsigned int i=0;i < current_domain_.node.length ();i++)
    {
      (*host_info_seq)[i].hostname =
        CORBA::string_dup (current_domain_.node[i].name);
      //      ACE_DEBUG ((LM_DEBUG , "The resource length is [%d]",
      //          current_domain_.node[i].resource.length ()));

      for (unsigned int j = 0;j < current_domain_.node[i].resource.length ();j++)
        {
          if (!strcmp(
                      current_domain_.node[i].resource[j].name,
                      "Processor"))
            {
              current_domain_.node[i].resource[j].property[0].value
                >>= (*host_info_seq)[i].cpu_util;
              CORBA::Double d;
              current_domain_.node[i].resource[j].property[0].value
                >>= d;
              //      ACE_DEBUG ((LM_DEBUG, "TM::The current cpu util is [%f]\n", d));
            }
        }
    }

  if (CIAO::debug_level () > 9)
    ACE_DEBUG ((LM_DEBUG , "TM::Returning from get_cpu_info"));

  return host_info_seq;
}

CORBA::Long CIAO::DomainDataManager::get_pid (ACE_CString cmp)
{
  CORBA::Long pid;

  // This is really ineffiecient this is O(n) ; searching all the nodes
  // all the resources for a particular component.
  // It needs to be stored in some other data structure

  for (unsigned int i=0;i < current_domain_.node.length ();i++)
    {
      if (CIAO::debug_level () > 9)
        ACE_DEBUG ((LM_DEBUG , "TM::The resource length is [%d]",
                    current_domain_.node[i].resource.length ()));

      for (unsigned int j = 0;j < current_domain_.node[i].resource.length ();j++)
        {
          // The resource
          if (!strcmp(
                      current_domain_.node[i].resource[j].name,
                      "Component") &&
              ACE_CString (current_domain_.node[i].resource[j].property[0].name) ==
              cmp)
            {
              current_domain_.node[i].resource[j].property[0].value
                >>= pid;
              if (CIAO::debug_level () > 9)
                ACE_DEBUG ((LM_DEBUG,
                            "TM::getpid::The current pid is [%d]\n", pid));

            }
        } // resources
    }// nodes

  return pid;
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

  for (unsigned int i = 0;i < plan.instance.length ();i++)
    {
      for (unsigned int j = 0;j < temp_provisioned_data.node.length ();j++)
        {
          if (!strcmp (plan.instance[i].node.in () ,
                       temp_provisioned_data.node[j].name.in ()))
          {
            if (CIAO::debug_level () > 9)
              ACE_DEBUG ((LM_DEBUG ,
                  "TM::commitResource::Host name matched\n"));
            try {
              match_requirement_resource (
                                          plan.instance[i].deployedResource,
                                          temp_provisioned_data.node[j].resource);
            }
            catch (::Deployment::ResourceNotAvailable& ex)
              {
                // catch the exception and add parameters
                ex.elementName =
                  CORBA::string_dup (temp_provisioned_data.node[j].name);

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

  for (unsigned int i = 0;i < plan.instance.length ();i++)
    {
      for (unsigned int j = 0;j < provisioned_data_.node.length ();j++)
        {
          if (!strcmp (plan.instance[i].node.in () ,
                       provisioned_data_.node[j].name.in ()))
          {
            if (CIAO::debug_level () > 9)
              ACE_DEBUG ((LM_DEBUG ,
                  "TM::commitResource::Host name matched\n"));

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

  for (unsigned int i = 0;i < deployed.length ();i++)
    {
      // for each deployed resource ....search the corresponding
      // available resource
      for (unsigned int j = 0;j < available.length ();j++)
        {
          if (!strcmp (deployed[i].requirementName, available[j].name))
            {
              if (CIAO::debug_level () > 9)
                ACE_DEBUG ((LM_DEBUG ,
                    "TM::commitResource::Requirement name matched\n"));
              // search for the resourcename in the resourceType
              for (unsigned int k = 0;k < available[j].resourceType.length ();k++)
                {
                  if (!strcmp (deployed[i].resourceName,
                               available[j].resourceType[k]))
                    {
                      if (CIAO::debug_level () > 9)
                        ACE_DEBUG ((LM_DEBUG ,
                           "TM::commitResource::Resource name matched\n"));

                      try {
                        match_properties (deployed[i].property,
                                          available[j].property);
                      }
                      catch (::Deployment::ResourceNotAvailable& ex)
                        {
                          // catch the exception and add parameters
                          ex.resourceType =
                            CORBA::string_dup (available[j].resourceType[k]);
                          ex.resourceName =
                            CORBA::string_dup (available[j].name);
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
  for (unsigned int i = 0;i < deployed.length ();i++)
    {
      for (unsigned int j = 0;j < available.length ();j++)
        if (!strcmp (deployed[i].name , available[j].name))
          {
            // check kind here ....and then subtract ....
            // accordingly , ..this is complex ... better to write
            // some specialised algo
            // for now assuming Capacity ....
            // and tk_double ....
            if (CIAO::debug_level () > 9)
              ACE_DEBUG ((LM_DEBUG ,
                          "TM::commitResource::Property name matched\n"));

              commit_release_resource (deployed[i] , available[j]);

          }
    }
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
          if (CIAO::debug_level () > 9)
            ACE_DEBUG ((LM_DEBUG, "TM::The available is [%f]",
                        available_d));

          available.value <<= available_d;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Insufficient resources! Available: %d, Required %d\n",
                      available_d, required_d));
          throw ::Deployment::ResourceNotAvailable ("",
                                                    "",
                                                    deployed.name.in (),
                                                    "",
                                                    "");
        }


    }
  else
    {
      //must be release
      CORBA::Long required_d;
      deployed.value >>= required_d;
      CORBA::Long available_d;
      available.value >>= available_d;

      available_d = available_d + required_d;


      // Should we check for bin > 100 ??????

      if (CIAO::debug_level () > 9)
        ACE_DEBUG ((LM_DEBUG, "TM::The available is [%f]",
                    available_d));

      available.value <<= available_d;
    }
}

void CIAO::DomainDataManager::stop_monitors ()
{

  int length = initial_domain_.node.length ();
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG, "Number of nodes in domain.cdd is : %d\n", length));
    }

  for (int i=0;i < length;i++)
    {

      ::Deployment::NodeManager_var node_manager;


      try
        {
          node_manager =
            deployment_config_.get_node_manager
            (initial_domain_.node[i].name.in ());
        }
      catch (CORBA::Exception& ex)
        {
          ACE_ERROR ((LM_ERROR, "DANCE::TM (%P|%t) DomainDataManager.cpp: "
                      "Error in get Node Manager from Deployment Config %s\n",
                      initial_domain_.node[i].name.in ()));
          continue;
        }


      if (!CORBA::is_nil (node_manager.in ()))
        {
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG, "Trying to contact nodemanager on %s\n",
                          initial_domain_.node[i].name.in ()));
            }
          try
            {
              node_manager->leaveDomain ();
            }
          catch (CORBA::Exception& ex)
            {
              ACE_DEBUG ((LM_DEBUG , "TM::Error in calling Leave Domain\n"));
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in "
                                   "DomainDataManager::leaveDomain");
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

//  ::Deployment::DnC_Dump::dump (this->provisioned_data_);

  ACE_DEBUG ((LM_DEBUG, "TM::Node Up Message Processed\n"));

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
    if (strcmp (node_name, this->initial_domain_.node[i].name.in ()) == 0)
    {
      node = this->initial_domain_.node[i];
      return true;
    }
  }

  // not found the node , retunr a node with an empty name
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
    if (strcmp (node_name, this->provisioned_data_.node[i].name.in ()) == 0)
    {
      node = this->provisioned_data_.node[i];
      return true;
    }
  }

  // not found the node , retunr a node with an empty name
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
  bool found = 0;


  for (CORBA::ULong j = 0;
       j < this->provisioned_data_.node.length ();
       j++)
    {
      found = 0;

      for (CORBA::ULong i = 0;i < domain.node.length ();i++)
        {
          if (strcmp (domain.node[i].name.in (),
                      this->provisioned_data_.node[j].name.in ()) == 0)
            {
              found = 1;
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

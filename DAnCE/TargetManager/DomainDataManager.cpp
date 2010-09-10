// $Id$
#include "DomainDataManager.h"
#include "Deployment/Deployment_NodeManagerC.h"

#include "Config_Handlers/XML_File_Intf.h"
#include "DAnCE/Logger/Log_Macros.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

void
DAnCE::DomainDataManager::init (CORBA::ORB_ptr orb,
                                ::Deployment::TargetManager_ptr target,
                                const ACE_TCHAR *domain_name)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::init");

  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->target_mgr_ = ::Deployment::TargetManager::_duplicate(target);

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("DAnCE::DomainDataManager::init - ")
                ACE_TEXT ("Parsing initial domain from file %s\n"),
                domain_name));
  
  DAnCE::Config_Handlers::XML_File_Intf intf (domain_name);
  ::Deployment::Domain* dmn = intf.release_domain ();

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE::DomainDataManager::init - ")
                ACE_TEXT ("Initial domain successfully parsed\n")));
#ifdef GEN_OSTREAM_OPS
  std::ostringstream _stream;
  _stream << *dmn << std::endl;

  DANCE_DEBUG (9, (LM_TRACE, DLINFO "DAnCE::DomainDataManager::init - "
                "Contents of Domain: %C\n",
                _stream.str ().c_str ()));
#endif

  current_domain_ = *dmn;
  initial_domain_ = current_domain_;

  // initialize the provisioning domain
  provisioned_data_ = initial_domain_;

  update_node_status ();

  call_all_node_managers ();
}

int DAnCE::DomainDataManager::update_domain (const ::CORBA::StringSeq &,
                                             const ::Deployment::Domain & domainSubset,
                                             ::Deployment::DomainUpdateKind update_kind)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::update_domain");

  // Update the subset of the domain which the above
  // parameter corresponds to

  // Check the type of update ..
  switch (update_kind)
    {
      case ::Deployment::UpdateAll:
      case ::Deployment::UpdateDynamic:
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

  CORBA::ULong const size = current_domain_.node.length ();

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
  return 0;
}

::Deployment::Domain* DAnCE::DomainDataManager::get_current_domain ()
{
  DANCE_TRACE ("DAnCE::DomainDataManager::get_current_domain");

  return new ::Deployment::Domain (provisioned_data_);
}

::Deployment::Domain* DAnCE::DomainDataManager::get_initial_domain ()
{
  DANCE_TRACE ("DAnCE::DomainDataManager::get_initial_domain");

  return new ::Deployment::Domain (initial_domain_);
}

int DAnCE::DomainDataManager::readin_domain_data ()
{
  DANCE_TRACE ("DAnCE::DomainDataManager::readin_domain_data");

  // here read in Domain data ...
  //
  return 0;
}

int DAnCE::DomainDataManager::call_all_node_managers ()
{
  DANCE_TRACE ("DAnCE::DomainDataManager::call_all_node_managers");

/*  if ( this->deployment_config_.init ("NodeDetails.dat") == -1 )
    {
      DANCE_ERROR (1, (LM_ERROR,
                  "TargetM (%P|%t) DomainDataManager.cpp -"
                  "DAnCE::DomainDataManager::call_all_node_managers -"
                  "ERROR while trying to initialize after reading "
                  "node details DAT file\n"));
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
          DANCE_ERROR (1, (LM_ERROR, "DANCE::TM (%P|%t) DomainDataManager.cpp: "
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
              node_manager->joinDomain (sub_domain,
                                        target_mgr_.in (),
                                        log);
            }
          catch (CORBA::Exception& ex)
            {
              DANCE_ERROR (1, (LM_ERROR , "TM::Error in calling Join Domain==\n"));
              ex._tao_print_exception (
                "Exception caught in ""DomainDataManager::joinDomain");
            }
        }
    }
*/
  return 0;

}


::Deployment::ResourceCommitmentManager_ptr DAnCE::DomainDataManager
::commitResources (const ::Deployment::ResourceAllocations &)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::commitResources");

/*
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
  */
  return 0;
}


void DAnCE::DomainDataManager::
releaseResources (const ::Deployment::ResourceCommitmentManager_ptr)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::releaseResources");

  // release the resources
/*

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
*/
}


void DAnCE::DomainDataManager::match_requirement_resource (
      ::Deployment::InstanceResourceDeploymentDescriptions deployed,
      ::Deployment::Resources & available)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::match_requirement_resource");

  // here match the deployed to the available

  for (CORBA::ULong i = 0;i < deployed.length ();i++)
    {
      // for each deployed resource ....search the corresponding
      // available resource
      for (CORBA::ULong j = 0;j < available.length ();j++)
        {
          if (!ACE_OS::strcmp (deployed[i].requirementName, available[j].name))
            {
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

void DAnCE::DomainDataManager::match_properties (
      ::Deployment::Properties deployed,
      ::Deployment::SatisfierProperties & available)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::match_properties");

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

void DAnCE::DomainDataManager::commit_release_resource (
           ::Deployment::Property & deployed,
           ::Deployment::SatisfierProperty & available)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::commit_release_resource");

  if (current_action_ == commit)
    {

      CORBA::Long required_d;

      if ((deployed.value >>= required_d) == false)
        DANCE_ERROR (1, (LM_ERROR, "Failed to extract required amount\n"));

      CORBA::Long available_d;

      if ((available.value >>= available_d) == false)
        DANCE_ERROR (1, (LM_ERROR, "failed to extract available amount\n"));

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

int DAnCE::DomainDataManager::add_to_domain (
    const ::Deployment::Domain& domain)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::add_to_domain");

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

bool DAnCE::DomainDataManager::find_in_initial_domain (const char* node_name,
                        ::Deployment::Node& node)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::find_in_initial_domain");

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


bool DAnCE::DomainDataManager::find_in_provisioned_domain (const char* node_name,
                        ::Deployment::Node& node)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::find_in_provisioned_domain");

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

int DAnCE::DomainDataManager::delete_from_domain (
    const ::Deployment::Domain& domain)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::delete_from_domain");

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

int DAnCE::DomainDataManager::intimate_planner (
    const ::Deployment::Domain& domain)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::intimate_planner");

 // use the connection with the planner and get a reference to the planner
 // make a call top the planner
  Deployment::Domain d = domain;
  return 0;

}

bool DAnCE::DomainDataManager::update_node_status ()
{
  DANCE_TRACE ("DAnCE::DomainDataManager::update_node_status");

  // update the node status here ...
  return 0;
}

void DAnCE::DomainDataManager::commitResourceAllocation (
          const ::Deployment::ResourceAllocations & resources)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::commitResourceAllocation");

  // commit the resources
  // parse into the plan and commit resources ...

  // set the action value
  current_action_ = commit;

  this->commit_release_RA (resources);
}

void DAnCE::DomainDataManager::releaseResourceAllocation (
    const ::Deployment::ResourceAllocations & resources)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::releaseResourceAllocation");

  // set the action value
  current_action_ = release;

  this->commit_release_RA (resources);
}


int DAnCE::DomainDataManager::commit_release_RA (const ::Deployment::ResourceAllocations& resources)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::commit_release_RA");

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
      DANCE_ERROR (1, (LM_ERROR, "Caught the Exception in releaseResourceAllocation\n"));
      ex.index = i;
      throw ex;
    }
  }

  // here commit the commitresources
  provisioned_data_ = temp_provisioned_data_;

  return 0;
}

::Deployment::Resource&
DAnCE::DomainDataManager::find_resource (
    const ::Deployment::ResourceAllocation& resource)
{
  DANCE_TRACE ("DAnCE::DomainDataManager::find_resource");

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

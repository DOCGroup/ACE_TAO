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

int DomainDataManager::update_domain (const ::CORBA::StringSeq &,
                                      const ::Deployment::Domain & domainSubset,
                                      ::Deployment::DomainUpdateKind )
{
  // Update the subset of the domain which the above
  // parameter corresponds to

  // for now consider only nodes
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG , "Inside The update Domain of Manager\n"));
      ACE_DEBUG ((LM_DEBUG , "the length of domain is [%d]",
                  current_domain_.node.length ()));
      ACE_DEBUG ((LM_DEBUG , "domainSubsetNode is \n" ));
    }
  int size = current_domain_.node.length ();
  int i;
  for (i=0;i < size;i++)
    {
      if (!strcmp (domainSubset.node[0].name , current_domain_.node[i].name))
        {
          // found a match
          // for now overwrite the entire Node info ...
          // but later , this has to be changed to overwrite
          // only the specific part ...
          if (CIAO::debug_level () > 9)
            {
              ACE_DEBUG ((LM_DEBUG , "Changed the memory Value\n"));
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
      ACE_DEBUG ((LM_DEBUG , "Inside The update Domain of Manager\n"));
    }
  return 0;
}

DomainDataManager::
DomainDataManager (CORBA::ORB_ptr orb,
                   ::Deployment::TargetManager_ptr target)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    deployment_config_ (orb_.in()),
    target_mgr_ (::Deployment::TargetManager::_duplicate(target))
{
  CIAO::Config_Handlers::DD_Handler dd (domain_file_name);
  ::Deployment::Domain* dmn = dd.domain_idl ();
  ::Deployment::DnC_Dump::dump (*dmn);
  current_domain_ = *dmn;
  initial_domain_ = current_domain_;

  call_all_node_managers ();
}

::Deployment::Domain* DomainDataManager::get_current_domain ()
{
  ::Deployment::Domain * retval = new ::Deployment::Domain (current_domain_);
  return retval;
}

::Deployment::Domain* DomainDataManager::get_initial_domain ()
{
  ::Deployment::Domain * retval = new ::Deployment::Domain (initial_domain_);
  return retval;
}

int DomainDataManager::readin_domain_data ()
{
  // here read in Domain data ...
  //
  return 0;
}

int DomainDataManager::call_all_node_managers ()
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
  for (int i=0;i < length;i++)
    {
      ::Deployment::NodeManager_var node_manager =
          deployment_config_.get_node_manager (initial_domain_.node[i].name);
      if (node_manager.in () != 0)
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
              node_manager->joinDomain (sub_domain , target_mgr_.in (), log);
            }
          catch (CORBA::Exception& ex)
            {
              ACE_DEBUG ((LM_DEBUG , "Error in calling Join Domain==\n"));
            }
        }
    }
  return 0;

}

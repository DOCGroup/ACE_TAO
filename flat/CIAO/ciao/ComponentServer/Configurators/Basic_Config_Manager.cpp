// $Id$

#include "Basic_Config_Manager.h"
#include "ciao/CIAO_common.h"
#include "ace/SString.h"
#include "tao/SystemException.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "tao/DiffServPolicy/Server_Network_Priority_Policy.h"

namespace CIAO
{
  namespace Deployment
  {
    
    void
    Basic_Config_Manager::init (CORBA::ORB_ptr orb)
    {
      this->orb_ = CORBA::ORB::_duplicate (orb);
    }

    int
    Basic_Config_Manager::pre_orb_initialize (void)
    {
      return 0;
    }

    int
    Basic_Config_Manager::post_orb_initialize (CORBA::ORB_ptr)
    {
      return 0;
    }

    void
    Basic_Config_Manager::init_resources
    (const ServerResource &server_resource)
    {
      ACE_DEBUG ((LM_DEBUG,"Basic_Config_Manager::init_resources\n"));

      if (CORBA::is_nil (this->orb_.in()))
	{
	  ACE_ERROR ((LM_ERROR,
		      "Basic_Config_Manager has not been properly initialized\n"));
	  throw CORBA::INTERNAL ();
	}

      const PolicySets &sets = server_resource.orb_config.policy_set;
      for (CORBA::ULong i = 0; i < sets.length (); ++i)
	{
	  CORBA::ULong np = sets[i].policies.length ();
	  if (np == 0)
	    continue;

	  CORBA::PolicyList_var policy_list = new CORBA::PolicyList (np);
	  policy_list->length (np);
	  CORBA::ULong index = 0;
	  CORBA::ULong array_index = np;

	  // Create a list of policies
	  for (CORBA::ULong pc = 0; pc < np; ++pc)
	    {
	      CORBA::Policy_var temp_policy =
		this->create_single_policy (sets[i].policies[pc]);

	      if (CORBA::is_nil (temp_policy.in ()))
		{
		  array_index = array_index - 1;
		  policy_list->length (array_index);
		}
	      else
		{
		  policy_list[index] = temp_policy;
		  index = index + 1;
		}
	    }

	  // Bind the policy list to the name.  The bind operation should
	  // surrender the ownership of the newly created PolicyList
	  // sequence to the map.
	  if (array_index != 0)
	    {
	      if (this->policy_map_.bind (sets[i].Id.in (),
					  policy_list) != 0)
		{
		  ACE_ERROR ((LM_ERROR,
			      "Error binding Policy_Set with name: %s\n",
			      sets[i].Id.in ()));
		  throw CORBA::INTERNAL ();
		}
	      else
		{
		  ACE_DEBUG ((LM_DEBUG,
			      "Basic_Config_Manager::init_resource"
			      " added policy set : %s with %d policies\n",
			      sets[i].Id.in (), array_index));
		}
	    }
	  else
	    {
	      ACE_DEBUG ((LM_DEBUG,
			  "Basic_Config_Manager::init_resource"
			  " added policy set : %s with %d policies\n",
			  sets[i].Id.in (), array_index));
	    }
	}
    }

    bool
    Basic_Config_Manager::policy_exists (const char *name)
    {
      if (name == 0)
	{
	  ACE_ERROR ((LM_ERROR,
		      "Invalid name string found in "
		      "Basic_Config_Manager::policy_exists\n"));
	  throw CORBA::INTERNAL ();
	}

      POLICY_MAP::ENTRY *entry = 0;

      if (this->policy_map_.find (name, entry) != 0)
	{
	  return false;
	}

      return true;
    }

    CORBA::PolicyList *
    Basic_Config_Manager::find_policies_by_name (const char *name)
    {
      if (name == 0)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Invalid name string found in find_policies_by_name\n"));
	  throw CORBA::INTERNAL ();
	}

      POLICY_MAP::ENTRY *entry = 0;

      CORBA::PolicyList_var retv;

      if (this->policy_map_.find (name, entry) != 0)
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Unable to find a PolicyList named %s\n",
		      name));
	  retv = 0;
	}
      else
	{
	  retv = new CORBA::PolicyList (entry->int_id_.in ());
	}
      return retv._retn ();
    }

    CORBA::Policy_ptr
    Basic_Config_Manager::create_single_policy
    (const PolicyDef &policy_def)
    {
      CORBA::Policy_var retv;

      switch (policy_def._d ())
	{
	case TAO::NETWORK_PRIORITY_TYPE:
	  {
	    const NWPriorityModelPolicyDef &nw_tmp
	      = policy_def.NWPriorityModelDef ();
	    retv = this->orb_->_create_policy (TAO::NETWORK_PRIORITY_TYPE);

	    TAO::NetworkPriorityPolicy_var nw_priority =
	      TAO::NetworkPriorityPolicy::_narrow (retv.in ());

	    nw_priority->network_priority_model (
						 (TAO::NetworkPriorityModel) nw_tmp.nw_priority_model);

	    nw_priority->request_diffserv_codepoint (
						     (TAO::DiffservCodepoint)  nw_tmp.request_dscp);

	    nw_priority->reply_diffserv_codepoint (
						   (TAO::DiffservCodepoint)  nw_tmp.reply_dscp);

	    retv = nw_priority._retn ();

	    if (! CORBA::is_nil (retv.in ()))
	      ACE_DEBUG ((LM_DEBUG,
			  "NAConfigManager::Create NetworkPriority policy: %d\n",
			  nw_tmp.nw_priority_model));
	  }
	  break;

	case TAO::CLIENT_NETWORK_PRIORITY_TYPE:
	  {
	    const CNWPriorityModelPolicyDef &cnw_tmp
	      = policy_def.CNWPriorityModelDef ();
	    retv = this->orb_->_create_policy (TAO::CLIENT_NETWORK_PRIORITY_TYPE);

	    TAO::NetworkPriorityPolicy_var cnw_priority =
	      TAO::NetworkPriorityPolicy::_narrow (retv.in ());

	    cnw_priority->request_diffserv_codepoint (
						      (TAO::DiffservCodepoint) cnw_tmp.request_dscp);

	    cnw_priority->reply_diffserv_codepoint (
						    (TAO::DiffservCodepoint) cnw_tmp.reply_dscp);

	    retv = cnw_priority._retn ();

	    if (! CORBA::is_nil (retv.in ()))
	      ACE_DEBUG ((LM_DEBUG,
			  "NAConfigManager::Create NetworkPriority policy \n"));
	  }
	  break;

	default:
	  retv = 0;
	}

      return retv._retn ();
    }

  }
}

using CIAO::Deployment::Config_Manager;
using CIAO::Deployment::Basic_Config_Manager;

extern "C" Basic_Config_Manager_Export Config_Manager *create_na_config_manager (void);

Config_Manager *
create_na_config_manager (void)
{
  Basic_Config_Manager *config;
  ACE_NEW_RETURN (config, Basic_Config_Manager, 0);
  return config;
}

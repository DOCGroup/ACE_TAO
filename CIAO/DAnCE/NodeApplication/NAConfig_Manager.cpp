// $Id$

#include "NAConfig_Manager.h"
#include "ciao/CIAO_common.h"
#include "ace/SString.h"
#include "tao/TAO_Network_Priority_PolicyC.h"
#include "tao/Network_Priority_Policy.h"
#include "tao/Client_Network_Priority_Policy.h"

void
CIAO::NAResource_Config_Manager::init (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}

int
CIAO::NAResource_Config_Manager::pre_orb_initialize (void)
{
  return 0;
}

int
CIAO::NAResource_Config_Manager::post_orb_initialize (CORBA::ORB_ptr)
{
  return 0;
}

void
CIAO::NAResource_Config_Manager::init_resources
(const CIAO::DAnCE::ServerResource &server_resource
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,"NAResource_Config_Manager::init_resources\n"));

  if (CORBA::is_nil (this->orb_.in()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "NAResource_Config_Manager has not been properly initialized\n"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  const CIAO::DAnCE::PolicySets &sets = server_resource.orb_config.policy_set;
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
            this->create_single_policy (sets[i].policies[pc]
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (temp_policy == 0)
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
              ACE_DEBUG ((LM_DEBUG,
                          "Error binding Policy_Set with name: %s\n",
                          sets[i].Id.in ()));
              ACE_THROW (CORBA::INTERNAL ());
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "NAResource_Config_Manager::init_resource"
                          " added policy set : %s with %d policies\n", 
                          sets[i].Id.in (), array_index));
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "NAResource_Config_Manager::init_resource"
                      " added policy set : %s with %d policies\n",
                      sets[i].Id.in (), array_index));
        }
    }
}

bool
CIAO::NAResource_Config_Manager::policy_exists (const char *name
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid name string found in "
                  "CIAO::NAResource_Config_Manager::policy_exists\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  POLICY_MAP::ENTRY *entry = 0;

  if (this->policy_map_.find (name, entry) != 0)
    {
      return false;
    }

  return true;
}

CORBA::PolicyList *
CIAO::NAResource_Config_Manager::find_policies_by_name (const char *name
                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid name string found in find_policies_by_name\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  POLICY_MAP::ENTRY *entry = 0;

  CORBA::PolicyList_var retv;

  if (this->policy_map_.find (name, entry) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Unable to find a PolicyList named %s\n",
                  name));
      retv = 0;
      // ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }
  else
    {
      retv = new CORBA::PolicyList (entry->int_id_.in ());
    }
  return retv._retn ();
}

CORBA::Policy_ptr
CIAO::NAResource_Config_Manager::create_single_policy
(const CIAO::DAnCE::PolicyDef &policy_def
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Policy_var retv;

  switch (policy_def._d ())
    {
    case TAO::NETWORK_PRIORITY_TYPE:
      {
        const CIAO::DAnCE::NWPriorityModelPolicyDef &nw_tmp
          = policy_def.NWPriorityModelDef ();
        retv = this->orb_->create_network_priority_policy (
           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        TAO::NetworkPriorityPolicy_var nw_priority =
          TAO::NetworkPriorityPolicy::_narrow (retv.in ()
                 ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        nw_priority->set_network_priority_model (
          (TAO::NetworkPriorityModel) nw_tmp.nw_priority_model
          ACE_ENV_ARG_PARAMETER);

        nw_priority->set_request_diffserv_codepoint (
          (TAO::DiffservCodepoint)  nw_tmp.request_dscp
          ACE_ENV_ARG_PARAMETER);

        nw_priority->set_reply_diffserv_codepoint (
          (TAO::DiffservCodepoint)  nw_tmp.reply_dscp
          ACE_ENV_ARG_PARAMETER);

        retv = nw_priority._retn ();

        if (! CORBA::is_nil (retv.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "NAConfigManager::Create NetworkPriority policy: %d\n",
                    nw_tmp.nw_priority_model));
      }
      break;

    case TAO::CLIENT_NETWORK_PRIORITY_TYPE:
      {
        const CIAO::DAnCE::CNWPriorityModelPolicyDef &cnw_tmp
          = policy_def.CNWPriorityModelDef ();
        retv = this->orb_->create_client_network_priority_policy (
           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        TAO::NetworkPriorityPolicy_var cnw_priority =
          TAO::NetworkPriorityPolicy::_narrow (retv.in ()
                 ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        cnw_priority->set_request_diffserv_codepoint (
          (TAO::DiffservCodepoint)  cnw_tmp.request_dscp
          ACE_ENV_ARG_PARAMETER);

        cnw_priority->set_reply_diffserv_codepoint (
          (TAO::DiffservCodepoint)  cnw_tmp.reply_dscp
          ACE_ENV_ARG_PARAMETER);

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

extern "C" CIAO_NA_Configurator_Export CIAO::Config_Manager *create_na_config_manager (void);

CIAO::Config_Manager *
create_na_config_manager (void)
{
  CIAO::NAResource_Config_Manager *config;
  ACE_NEW_RETURN (config, CIAO::NAResource_Config_Manager, 0);
  return config;
}

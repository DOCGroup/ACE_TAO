// $Id$

#include "RTConfig_Manager.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
# include "RTConfig_Manager.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::RTResource_Config_Manager::init
(const CIAO::RTConfiguration::RTORB_Resources_Info &info
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong i;

  // Creating and binding name with RT Threadpool
  for (i = 0; i < info.tp_configs.length (); ++i)
    {
      RTCORBA::ThreadpoolId thr_id =
        this->rtorb_->create_threadpool
        (info.tp_configs[i].stacksize,
         info.tp_configs[i].static_threads,
         info.tp_configs[i].dynamic_threads,
         info.tp_configs[i].default_priority,
         info.tp_configs[i].allow_request_buffering,
         info.tp_configs[i].max_buffered_requests,
         info.tp_configs[i].max_request_buffer_size
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;                // Simply pass back the exception here
                                // for now.  We need to have a better way
                                // to handle execption here.
      if (this->threadpool_map_.bind (info.tp_configs[i].name.in (),
                                      thr_id) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Error binding thread pool name: %s to map when initializing RTComponentServer resources.\n", info.tp_configs[i].name.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }
    }

  for (i = 0; i < info.tpl_configs.length (); ++i)
    {
      RTCORBA::ThreadpoolId thr_id =
        this->rtorb_->create_threadpool_with_lanes
        (info.tpl_configs[i].stacksize,
         info.tpl_configs[i].lanes,
         info.tpl_configs[i].allow_borrowing,
         info.tpl_configs[i].allow_request_buffering,
         info.tpl_configs[i].max_buffered_requests,
         info.tpl_configs[i].max_request_buffer_size
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;                // Simply pass back the exception here
                                // for now.  We need to have a better way
                                // to handle execption here.

      if (this->threadpool_map_.bind (info.tpl_configs[i].name.in (),
                                      thr_id) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Error binding thread pool name: %s to map when initializing RTComponentServer resources.\n", info.tpl_configs[i].name.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }
    }
}

void
CIAO::RTResource_Config_Manager::fini (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TP_MAP::ITERATOR iter = this->threadpool_map_.begin ();
  TP_MAP::ITERATOR end = this->threadpool_map_.end ();

  for (; iter != end; ++iter)
    {
      this->rtorb_->destroy_threadpool ((*iter).int_id_
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

}

RTCORBA::ThreadpoolId
CIAO::RTResource_Config_Manager::find_threadpool_by_name (const char *name
                                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid name string found in \"find_threadpool_by_name\"\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }


  RTCORBA::ThreadpoolId ret_id;    // return id

  if (this->threadpool_map_.find (name, ret_id) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Unable to find a threadpool named %s\n",
                  name));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return ret_id;
}

/*

RTCORBA::PriorityBands *
CIAO::RTResource_Config_Manager::find_priority_bands_by_name (const char *name
                                                              ACE_ENV_ARG_DECL)
  // @@ THROW SPEC?
{
}
*/

void
CIAO::RTPolicy_Set_Manager::init (const CIAO::RTConfiguration::Policy_Sets &sets
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0; i < sets.length (); ++i)
    {
      CORBA::ULong np = sets[i].configs.length ();
      if (np == 0)
        continue;

      CORBA::PolicyList_var policy_list = new CORBA::PolicyList (np);
      policy_list->length (np);

      // Create a list of policies
      for (CORBA::ULong pc = 0; pc < np; ++pc)
        {
          policy_list[pc] = this->create_single_policy (sets[i].configs[pc]
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      // Bind the policy list to the name.  The bind operation should
      // surrender the ownership of the newly created PolicyList
      // sequence to the map.
      if (this->policy_map_.bind (sets[i].name.in (),
                                  policy_list) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error binding Policy_Set with name: %s\n",
                      sets[i].name.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }
    }
}

void
CIAO::RTPolicy_Set_Manager::fini (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // We are keeping the PolicyList in var variables in the map.
  // There's no need to release them explicitly.
}

CORBA::PolicyList *
CIAO::RTPolicy_Set_Manager::find_policies_by_name (const char *name
                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid name string found in \"find_policies_by_name\"\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }


  POLICY_MAP::ENTRY *entry;

  if (this->policy_map_.find (name, entry) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Unable to find a PolicyList named %s\n",
                  name));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  // duplicate the sequence PolicyList.
  CORBA::PolicyList_var retv = new CORBA::PolicyList (entry->int_id_);
  return retv._retn ();
}

CORBA::Policy_ptr
CIAO::RTPolicy_Set_Manager::create_single_policy
(const CIAO::RTConfiguration::Policy_Config &policy_config
 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Policy_var retv;

  switch (policy_config.type)
    {
    case RTCORBA::PRIORITY_MODEL_POLICY_TYPE:
      CIAO::RTConfiguration::Priority_Model_Config *tmp;

      if (policy_config.configuration >>= tmp)
        {
          retv = this->rtorb_->create_priority_model_policy (tmp->model,
                                                             tmp->default_priority
                                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Error extracting PriorityModelPolicy configuration\n"));
          ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
        }
      break;

    case RTCORBA::THREADPOOL_POLICY_TYPE:
      retv = this->orb_->create_policy (RTCORBA::THREADPOOL_POLICY_TYPE,
                                        policy_config.configuration
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      break;

    default:
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid policy type - RTPolicy_Set_Manager::create_single_policy\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return retv._retn ();
}

// $Id$

#include "RTConfig_Manager.h"
#include "ciao/CIAO_common.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
# include "RTConfig_Manager.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::RTResource_Config_Manager::init (RTCORBA::RTORB_ptr rtorb)
{
  this->rtorb_ = RTCORBA::RTORB::_duplicate (rtorb);
}

void
CIAO::RTResource_Config_Manager::print_resources
(const CIAO::DAnCE::ServerResource &server_resource
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  const CIAO::DAnCE::ORBResource &orb_resource
    = server_resource.orb_config.orb_resources[0];

  CORBA::ULong i;

  // Creating and binding name with RT Threadpool
  for (i = 0; i < orb_resource.threadpool_list.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "-RTResource_Config_Manager::print_resource threadpool: %s\n"
                  "\tStack size: %d\n"
                  "\tStatic: %d\n"
                  "\tDynamic %d\n"
                  "\tDefault Prio: %d\n"
                  "\tBuffering %s\n"
                  "\tMax Buffered: %d\n"
                  "\tBuffer Size %d\n",
                  orb_resource.threadpool_list[i].Id.in (),
                  orb_resource.threadpool_list[i].stacksize,
                  orb_resource.threadpool_list[i].static_threads,
                  orb_resource.threadpool_list[i].dynamic_threads,
                  orb_resource.threadpool_list[i].default_priority,
                  (orb_resource.threadpool_list[i].allow_request_buffering ? "Y" : "N"),
                  orb_resource.threadpool_list[i].max_buffered_requests,
                  orb_resource.threadpool_list[i].max_request_buffer_size));
    }

  for (i = 0; i < orb_resource.threadpool_with_lanes_list.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "-RTResource_Config_Manager::print_resource threadpool_with %d, lanes: %s\n",
                  orb_resource.threadpool_with_lanes_list[i].threadpool_lanes.length (),
                  orb_resource.threadpool_with_lanes_list[i].Id.in ()));

      for (CORBA::ULong l = 0;
           l < orb_resource.threadpool_with_lanes_list[i].threadpool_lanes.length ();
           ++l)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\tLane %d:\n"
                      "\t\tPriority: %d\n"
                      "\t\tStatic: %d\n"
                      "\t\tDynamic: %d\n",
                      l,
                      orb_resource.threadpool_with_lanes_list[i].threadpool_lanes[l].lane_priority,
                      orb_resource.threadpool_with_lanes_list[i].threadpool_lanes[l].static_threads,
                      orb_resource.threadpool_with_lanes_list[i].threadpool_lanes[l].dynamic_threads));
                      }
          ACE_DEBUG ((LM_DEBUG,
                  "\tStack size: %d\n"
                  "\tBorrowing: %s\n"
                  "\tBuffering %s\n"
                  "\tMax Buffered: %d\n"
                  "\tBuffer Size %d\n",
                  orb_resource.threadpool_with_lanes_list[i].stacksize,
                  (orb_resource.threadpool_with_lanes_list[i].allow_borrowing ? "Y" : "N"),
                  (orb_resource.threadpool_with_lanes_list[i].allow_request_buffering ? "Y" : "N"),
                  orb_resource.threadpool_with_lanes_list[i].max_buffered_requests,
                  orb_resource.threadpool_with_lanes_list[i].max_request_buffer_size));
    }

  for (i = 0; i < orb_resource.connection_bands_list.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "-RTResource_Config_Manager::print_resource connection %d bands: %s\n",
                  orb_resource.connection_bands_list[i].bands.length (),
                  orb_resource.connection_bands_list[i].Id.in ()));

      for (CORBA::ULong l = 0;
           l < orb_resource.connection_bands_list[i].bands.length ();
           ++l)
        {
      ACE_DEBUG ((LM_DEBUG,
                  "\tBand %d Prio: %d -- %d\n",
                  l,
                  orb_resource.connection_bands_list[i].bands[l].low,
                  orb_resource.connection_bands_list[i].bands[l].high));
        }
    }
}

void
CIAO::RTResource_Config_Manager::init_resources
(const CIAO::DAnCE::ServerResource &server_resource
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "RTResource_Config_Manager::init_resource.\n"));

  if (CORBA::is_nil (this->rtorb_.in()))
    {
      ACE_ERROR ((LM_ERROR,
                  "RTResource_Config_Manager has not been properly initialized.\n"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  this->print_resources (server_resource);
  //  return;

  const CIAO::DAnCE::ORBResource &orb_resource
    = server_resource.orb_config.orb_resources[0];

  CORBA::ULong i;

  // Creating and binding name with RT Threadpool
  for (i = 0; i < orb_resource.threadpool_list.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "RTResource_Config_Manager::init_resource threadpool: %s\n",
                  orb_resource.threadpool_list[i].Id.in ()));
      RTCORBA::ThreadpoolId thr_id =
        this->rtorb_->create_threadpool
        (orb_resource.threadpool_list[i].stacksize,
         orb_resource.threadpool_list[i].static_threads,
         orb_resource.threadpool_list[i].dynamic_threads,
         orb_resource.threadpool_list[i].default_priority,
         orb_resource.threadpool_list[i].allow_request_buffering,
         orb_resource.threadpool_list[i].max_buffered_requests,
         orb_resource.threadpool_list[i].max_request_buffer_size
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;                // Simply pass back the exception here
                                // for now.  We need to have a better way
                                // to handle execption here.
      if (this->threadpool_map_.bind (orb_resource.threadpool_list[i].Id.in (),
                                      thr_id) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error binding thread pool Id: %s to map when initializing RTNodeApplication resources.\n", orb_resource.threadpool_list[i].Id.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RTResource_Config_Manager::init_resource added thread pool: %s\n",
                      orb_resource.threadpool_list[i].Id.in ()));
        }
    }

  for (i = 0; i < orb_resource.threadpool_with_lanes_list.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "RTResource_Config_Manager::init_resource threadpool_with %d, lanes: %s\n",
                  orb_resource.threadpool_with_lanes_list[i].threadpool_lanes.length (),
                  orb_resource.threadpool_with_lanes_list[i].Id.in ()));

      // Duplicate RTCORBA lanes
      RTCORBA::ThreadpoolLanes lanes
        (orb_resource.threadpool_with_lanes_list[i].threadpool_lanes.length ());
      lanes.length (orb_resource.threadpool_with_lanes_list[i].threadpool_lanes.length ());

      for (CORBA::ULong l = 0;
           l < orb_resource.threadpool_with_lanes_list[i].threadpool_lanes.length ();
           ++l)
        {
          lanes[l].lane_priority
            = orb_resource.threadpool_with_lanes_list[i].threadpool_lanes[l].lane_priority;
          lanes[l].static_threads
            = orb_resource.threadpool_with_lanes_list[i].threadpool_lanes[l].static_threads;
          lanes[l].dynamic_threads
            = orb_resource.threadpool_with_lanes_list[i].threadpool_lanes[l].dynamic_threads;
        }

      RTCORBA::ThreadpoolId thr_id =
        this->rtorb_->create_threadpool_with_lanes
        (orb_resource.threadpool_with_lanes_list[i].stacksize,
         lanes,
         orb_resource.threadpool_with_lanes_list[i].allow_borrowing,
         orb_resource.threadpool_with_lanes_list[i].allow_request_buffering,
         orb_resource.threadpool_with_lanes_list[i].max_buffered_requests,
         orb_resource.threadpool_with_lanes_list[i].max_request_buffer_size
         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;                // Simply pass back the exception here
                                // for now.  We need to have a better way
                                // to handle execption here.

      if (this->threadpool_map_.bind (orb_resource.threadpool_with_lanes_list[i].Id.in (),
                                      thr_id) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error binding thread pool name: %s to map when "
                      "initializing RTNodeApplication resources.\n",
                      orb_resource.threadpool_with_lanes_list[i].Id.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }

      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RTResource_Config_Manager::init_resource added thread pool with lanes: %s\n",
                      orb_resource.threadpool_with_lanes_list[i].Id.in ()));
        }
    }

  for (i = 0; i < orb_resource.connection_bands_list.length (); ++i)
    {
      RTCORBA::PriorityBands
        safebands(orb_resource.connection_bands_list[i].bands.length ());
      safebands.length (orb_resource.connection_bands_list[i].bands.length ());

      for (CORBA::ULong l = 0;
           l < orb_resource.connection_bands_list[i].bands.length ();
           ++l)
        {
          safebands[l].low = orb_resource.connection_bands_list[i].bands[l].low;
          safebands[l].high = orb_resource.connection_bands_list[i].bands[l].high;
        }

      if (this->priority_bands_map_.bind (orb_resource.connection_bands_list[i].Id.in (),
                                          safebands) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error binding priority bands name: %s to map "
                      "when initializing RTNodeApplication resources.\n",
                      orb_resource.connection_bands_list[i].Id.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RTResource_Config_Manager::init_resource added connection bands: %s\n",
                      orb_resource.connection_bands_list[i].Id.in ()));
        }
    }

  // now initialize the policy set part of it.
  // server_resource.orb_config.policy_set
  const CIAO::DAnCE::PolicySets &sets = server_resource.orb_config.policy_set;
  for (CORBA::ULong i = 0; i < sets.length (); ++i)
    {
      CORBA::ULong const np = sets[i].policies.length ();
      if (np == 0)
        continue;

      CORBA::PolicyList_var policy_list = new CORBA::PolicyList (np);
      policy_list->length (np);

      // Create a list of policies
      for (CORBA::ULong pc = 0; pc < np; ++pc)
        {
          policy_list[pc] = this->create_single_policy (sets[i].policies[pc]
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      // Bind the policy list to the name.  The bind operation should
      // surrender the ownership of the newly created PolicyList
      // sequence to the map.
      if (this->policy_map_.bind (sets[i].Id.in (),
                                  policy_list) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error binding Policy_Set with name: %s\n",
                      sets[i].Id.in ()));
          ACE_THROW (CORBA::INTERNAL ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RTResource_Config_Manager::init_resource added policy set: %s\n",
                      sets[i].Id.in ()));
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
      ACE_ERROR ((LM_ERROR,
                  "Invalid name string found in \"find_threadpool_by_name\"\n"));
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
    }


  RTCORBA::ThreadpoolId ret_id = 0;    // return id

  if (this->threadpool_map_.find (name, ret_id) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to find a threadpool named %s\n",
                  name));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return ret_id;
}

RTCORBA::PriorityBands *
CIAO::RTResource_Config_Manager::find_priority_bands_by_name (const char *name
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Invalid name string found in \"find_priority_bands_by_name\"\n"));
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
    }

  PB_MAP::ENTRY *entry = 0;

  if (this->priority_bands_map_.find (name, entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to find a connection bands named %s\n",
                  name));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  RTCORBA::PriorityBands_var retv = new RTCORBA::PriorityBands;
  (*retv.ptr ()) = (*entry->int_id_.ptr ());
  return retv._retn ();
}

CORBA::PolicyList *
CIAO::RTResource_Config_Manager::find_policies_by_name (const char *name
                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (name == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Invalid name string found in \"find_policies_by_name\"\n"));
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
    }


  ACE_DEBUG ((LM_DEBUG, "RTResource_Config_Manager::find_policies_by_name: %s\n",
              name));

  POLICY_MAP::ENTRY *entry = 0;

  if (this->policy_map_.find (name, entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to find a PolicyList named %s\n",
                  name));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  // duplicate the sequence PolicyList.
  CORBA::PolicyList_var retv =
    new CORBA::PolicyList (entry->int_id_.in ());

  ACE_DEBUG ((LM_DEBUG, "RTResource_Config_Manager::find_policies_by_name ok.\n"));

  return retv._retn ();
}

CORBA::Policy_ptr
CIAO::RTResource_Config_Manager::create_single_policy
(const CIAO::DAnCE::PolicyDef &policy_def
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //  if (CIAO::debug_level () > 9)
    ACE_DEBUG ((LM_DEBUG,
                "RTResource_Config_Manager::create_single_policy: %d\n",
                policy_def._d ()));

  CORBA::Policy_var retv;

  switch (policy_def._d ())
    {
    case RTCORBA::PRIORITY_MODEL_POLICY_TYPE:
      {
        const CIAO::DAnCE::PriorityModelPolicyDef &tmp
          = policy_def.PriorityModelDef ();

        retv = this->rtorb_->create_priority_model_policy ((RTCORBA::PriorityModel) tmp.priority_model,
                                                           tmp.server_priority
                                                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if (! CORBA::is_nil (retv.in ()))
          ACE_DEBUG ((LM_DEBUG,
                      "Create PriorityModel policy: %d - %d\n",
                      tmp.priority_model, tmp.server_priority));
      }
      break;

    case RTCORBA::THREADPOOL_POLICY_TYPE:
      {
        RTCORBA::ThreadpoolId tpid =
          this->find_threadpool_by_name (policy_def.ThreadpoolDef().Id.in ()
                                         ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        retv = this->rtorb_->create_threadpool_policy (tpid
                                                       ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if (! CORBA::is_nil (retv.in ()))
          ACE_DEBUG ((LM_DEBUG,
                      "Create Threadpool policy: %s, TPid: %d\n",
                      policy_def.ThreadpoolDef().Id.in (), tpid));
      }
      break;

    case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE:
      {
        RTCORBA::PriorityBands_var bands =
          this->find_priority_bands_by_name (policy_def.PriorityBandedConnectionDef().Id.in ()
                                             ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        retv =
          this->rtorb_->create_priority_banded_connection_policy (bands.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if (! CORBA::is_nil (retv.in ()))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Created Banded Connection policy: %s\n",
                        policy_def.PriorityBandedConnectionDef().Id.in ()));
          }
      }
      break;

    default:
      ACE_ERROR ((LM_ERROR,
                  "Invalid policy type - RTPolicy_Set_Manager::create_single_policy\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return retv._retn ();
}


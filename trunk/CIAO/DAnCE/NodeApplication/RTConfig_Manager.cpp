// $Id$

#include "RTConfig_Manager.h"
#include "ciao/CIAO_common.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
# include "RTConfig_Manager.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::RTResource_Config_Manager::init (CORBA::ORB_ptr orb)
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("RTORB");
  this->rtorb_ = RTCORBA::RTORB::_narrow (object.in ());
}

int
CIAO::RTResource_Config_Manager::pre_orb_initialize (void)
{
  return 0;
}

int
CIAO::RTResource_Config_Manager::post_orb_initialize (CORBA::ORB_ptr)
{
  return 0;
}

void
CIAO::RTResource_Config_Manager::print_resources
(const CIAO::DAnCE::ServerResource &server_resource)
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
(const CIAO::DAnCE::ServerResource &server_resource)
{
  ACE_DEBUG ((LM_DEBUG,
              "RTResource_Config_Manager::init_resource.\n"));

  if (CORBA::is_nil (this->rtorb_.in()))
    {
      ACE_ERROR ((LM_ERROR,
                  "RTResource_Config_Manager has not been properly initialized.\n"));
      throw CORBA::INTERNAL ();
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
         orb_resource.threadpool_list[i].max_request_buffer_size);
                      // Simply pass back the exception here
                                // for now.  We need to have a better way
                                // to handle execption here.
      if (this->threadpool_map_.bind (orb_resource.threadpool_list[i].Id.in (),
                                      thr_id) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error binding thread pool Id: %s to map when initializing RTNodeApplication resources.\n", orb_resource.threadpool_list[i].Id.in ()));
          throw CORBA::INTERNAL ();
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
         orb_resource.threadpool_with_lanes_list[i].max_request_buffer_size);
                      // Simply pass back the exception here
                                // for now.  We need to have a better way
                                // to handle execption here.

      if (this->threadpool_map_.bind (orb_resource.threadpool_with_lanes_list[i].Id.in (),
                                      thr_id) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error binding thread pool name: %s to map when "
                      "initializing RTNodeApplication resources.\n",
                      orb_resource.threadpool_with_lanes_list[i].Id.in ()));
          throw CORBA::INTERNAL ();
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
          throw CORBA::INTERNAL ();
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
      CORBA::ULong index = 0;
      CORBA::ULong array_index = np;

      // Create a list of policies
      for (CORBA::ULong pc = 0; pc < np; ++pc)
        {
          CORBA::Policy_var temp_policy = 
            this->create_single_policy (sets[i].policies[pc]);
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
              throw CORBA::INTERNAL (); 
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "RTResource_Config_Manager::init_resource "
                          "added policy set: %s with %d policies\n",
                          sets[i].Id.in (), array_index));
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "RTResource_Config_Manager::init_resource "
                      "added policy set: %s with %d policies\n",
                      sets[i].Id.in (), array_index));
        }
    }
}

void
CIAO::RTResource_Config_Manager::fini ()
{
  TP_MAP::ITERATOR iter = this->threadpool_map_.begin ();
  TP_MAP::ITERATOR end = this->threadpool_map_.end ();

  for (; iter != end; ++iter)
    {
      this->rtorb_->destroy_threadpool ((*iter).int_id_);
    }

}

RTCORBA::ThreadpoolId
CIAO::RTResource_Config_Manager::find_threadpool_by_name (const char *name)
{
  if (name == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Invalid name string found in \"find_threadpool_by_name\"\n"));
      throw CORBA::BAD_PARAM ();
    }


  RTCORBA::ThreadpoolId ret_id = 0;    // return id

  if (this->threadpool_map_.find (name, ret_id) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to find a threadpool named %s\n",
                  name));
      throw CORBA::INTERNAL ();
    }

  return ret_id;
}

RTCORBA::PriorityBands *
CIAO::RTResource_Config_Manager::find_priority_bands_by_name (const char *name)
{
  if (name == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Invalid name string found in \"find_priority_bands_by_name\"\n"));
      throw CORBA::BAD_PARAM ();
    }

  PB_MAP::ENTRY *entry = 0;

  if (this->priority_bands_map_.find (name, entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to find a connection bands named %s\n",
                  name));
      throw CORBA::INTERNAL ();
    }

  RTCORBA::PriorityBands_var retv = new RTCORBA::PriorityBands;
  (*retv.ptr ()) = (*entry->int_id_.ptr ());
  return retv._retn ();
}

bool
CIAO::RTResource_Config_Manager::policy_exists (const char *name)
{
  if (name == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid name string found in "
                  "CIAO::NAResource_Config_Manager::policy_exists\n"));
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
CIAO::RTResource_Config_Manager::find_policies_by_name (const char *name)
{
  if (name == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Invalid name string found in \"find_policies_by_name\"\n"));
      throw CORBA::BAD_PARAM ();
    }


  ACE_DEBUG ((LM_DEBUG, "RTResource_Config_Manager::find_policies_by_name: %s\n",
              name));

  POLICY_MAP::ENTRY *entry = 0;

  if (this->policy_map_.find (name, entry) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to find a PolicyList named %s\n",
                  name));
      throw CORBA::INTERNAL ();
    }

  // duplicate the sequence PolicyList.
  CORBA::PolicyList_var retv =
    new CORBA::PolicyList (entry->int_id_.in ());

  ACE_DEBUG ((LM_DEBUG, "RTResource_Config_Manager::find_policies_by_name ok.\n"));

  return retv._retn ();
}

CORBA::Policy_ptr
CIAO::RTResource_Config_Manager::create_single_policy
(const CIAO::DAnCE::PolicyDef &policy_def)
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
                                                           tmp.server_priority);

        if (! CORBA::is_nil (retv.in ()))
          ACE_DEBUG ((LM_DEBUG,
                      "Create PriorityModel policy: %d - %d\n",
                      tmp.priority_model, tmp.server_priority));
      }
      break;

    case RTCORBA::THREADPOOL_POLICY_TYPE:
      {
        RTCORBA::ThreadpoolId tpid =
          this->find_threadpool_by_name (policy_def.ThreadpoolDef().Id.in ());

        retv = this->rtorb_->create_threadpool_policy (tpid);

        if (! CORBA::is_nil (retv.in ()))
          ACE_DEBUG ((LM_DEBUG,
                      "Create Threadpool policy: %s, TPid: %d\n",
                      policy_def.ThreadpoolDef().Id.in (), tpid));
      }
      break;

    case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE:
      {
        RTCORBA::PriorityBands_var bands =
          this->find_priority_bands_by_name (policy_def.PriorityBandedConnectionDef().Id.in ());

        retv =
          this->rtorb_->create_priority_banded_connection_policy (bands.in ());

        if (! CORBA::is_nil (retv.in ()))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Created Banded Connection policy: %s\n",
                        policy_def.PriorityBandedConnectionDef().Id.in ()));
          }
      }
      break;

    default:
      retv = 0;
    }

  return retv._retn ();
}

extern "C" CIAO_RTNA_Configurator_Export CIAO::Config_Manager
*create_rt_config_manager (void);

CIAO::Config_Manager *
create_rt_config_manager (void)
{
  CIAO::RTResource_Config_Manager *config;
  ACE_NEW_RETURN (config, CIAO::RTResource_Config_Manager, 0);
  return config;
}

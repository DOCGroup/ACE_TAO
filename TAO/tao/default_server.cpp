// $Id$

#include "tao/default_server.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_client.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, default_server, "$Id$")

TAO_Default_Server_Strategy_Factory::TAO_Default_Server_Strategy_Factory (void)
  : activate_server_connections_ (0),
    thread_flags_ (THR_BOUND),
    poa_lock_type_ (TAO_THREAD_LOCK),
    poa_mgr_lock_type_ (TAO_THREAD_LOCK),
    event_loop_lock_type_ (TAO_NULL_LOCK),
    cached_connector_lock_type_ (TAO_THREAD_LOCK)
{
}

TAO_Default_Server_Strategy_Factory::~TAO_Default_Server_Strategy_Factory (void)
{
  // Perform appropriate cleanup.
}

int
TAO_Default_Server_Strategy_Factory::enable_poa_locking (void)
{
  switch (this->poa_lock_type_)
    {
    case TAO_NULL_LOCK:
      return 0;
    case TAO_THREAD_LOCK:
    default:
      return 1;
    }
}

int
TAO_Default_Server_Strategy_Factory::activate_server_connections (void)
{
  return this->activate_server_connections_;
}

int
TAO_Default_Server_Strategy_Factory::server_connection_thread_flags (void)
{
  return this->thread_flags_;
}

int
TAO_Default_Server_Strategy_Factory::server_connection_thread_count (void)
{
  return 1;
}

ACE_Lock *
TAO_Default_Server_Strategy_Factory::create_servant_lock (void)
{
  ACE_Lock *the_lock = 0;

#if defined (ACE_HAS_THREADS)
  if (this->activate_server_connections ())
      ACE_NEW_RETURN (the_lock,
                      ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex> (),
                      0);
  else
#endif /* ACE_HAS_THREADS */
      ACE_NEW_RETURN (the_lock,
                      ACE_Lock_Adapter<ACE_Null_Mutex> (),
                      0);

  return the_lock;
}

ACE_Lock *
TAO_Default_Server_Strategy_Factory::create_event_loop_lock (void)
{
  ACE_Lock *the_lock = 0;

  if (this->event_loop_lock_type_ == TAO_NULL_LOCK)
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                    0);
  else
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<ACE_SYNCH_RECURSIVE_MUTEX> (),
                    0);

  return the_lock;
}

ACE_Lock *
TAO_Default_Server_Strategy_Factory::create_cached_connector_lock (void)
{
  ACE_Lock *the_lock = 0;

  if (this->cached_connector_lock_type_ == TAO_NULL_LOCK)
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                    0);
  else
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<ACE_SYNCH_MUTEX> (),
                    0);

  return the_lock;
}

// Evil macros b/c I'm lazy!
#define TAO_BEGINCHECK  if (0)
#define TAO_CHECKANDSET(sym) else if (ACE_OS::strcmp (flag, #sym) == 0) ACE_SET_BITS (this->thread_flags_, sym)
#define TAO_ENDCHECK

void
TAO_Default_Server_Strategy_Factory::tokenize (char *flag_string)
{
  char *lasts = 0;

  for (char *flag = ACE_OS::strtok_r (flag_string, "|", &lasts);
       flag != 0;
       flag = ACE_OS::strtok_r (0, "|", &lasts))
    {
      TAO_BEGINCHECK;
      TAO_CHECKANDSET (THR_DETACHED);
      TAO_CHECKANDSET (THR_BOUND);
      TAO_CHECKANDSET (THR_NEW_LWP);
      TAO_CHECKANDSET (THR_SUSPENDED);
#if !defined (ACE_WIN32)
      TAO_CHECKANDSET (THR_DAEMON);
#endif /* ACE_WIN32 */
      TAO_ENDCHECK;
    }
}

int
TAO_Default_Server_Strategy_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Server_Strategy_Factory::open (TAO_ORB_Core* orb_core)
{
  return 0;
}

int
TAO_Default_Server_Strategy_Factory::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");

  int curarg;

  for (curarg = 0; curarg < argc && argv[curarg]; curarg++)
    if (ACE_OS::strcmp (argv[curarg], "-ORBconcurrency") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "reactive") == 0)
              this->activate_server_connections_ = 0;
            else if (ACE_OS::strcasecmp (name, "thread-per-connection") == 0)
              this->activate_server_connections_ = 1;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBtablesize") == 0 ||
             ACE_OS::strcmp (argv[curarg], "-ORBactiveobjectmapsize") == 0)
      {
        curarg++;
        if (curarg < argc)
          this->active_object_map_creation_parameters_.active_object_map_size_ = ACE_OS::strtoul (argv[curarg], 0, 10);
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBpoamapsize") == 0)
      {
        curarg++;
        if (curarg < argc)
          this->active_object_map_creation_parameters_.poa_map_size_ = ACE_OS::strtoul (argv[curarg], 0, 10);
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBactivehintinids") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *value = argv[curarg];

            this->active_object_map_creation_parameters_.use_active_hint_in_ids_ = ACE_OS::atoi (value);
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBactivehintinpoanames") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *value = argv[curarg];

            this->active_object_map_creation_parameters_.use_active_hint_in_poa_names_ = ACE_OS::atoi (value);
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBallowreactivationofsystemids") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *value = argv[curarg];

            this->active_object_map_creation_parameters_.allow_reactivation_of_system_ids_ = ACE_OS::atoi (value);
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBuseridpolicydemuxstrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            // Active demux not supported with user id policy
            if (ACE_OS::strcasecmp (name, "dynamic") == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_user_id_policy_ = TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name, "linear") == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_user_id_policy_ = TAO_LINEAR;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBsystemidpolicydemuxstrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "dynamic") == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_system_id_policy_ = TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name, "linear") == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_system_id_policy_ = TAO_LINEAR;
            else if (ACE_OS::strcasecmp (name, "active") == 0)
              this->active_object_map_creation_parameters_.object_lookup_strategy_for_system_id_policy_ = TAO_ACTIVE_DEMUX;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBpersistentidpolicydemuxstrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            // Active demux not supported with user id policy
            if (ACE_OS::strcasecmp (name, "dynamic") == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_persistent_id_policy_ = TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name, "linear") == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_persistent_id_policy_ = TAO_LINEAR;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBtransientidpolicydemuxstrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "dynamic") == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_transient_id_policy_ = TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name, "linear") == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_transient_id_policy_ = TAO_LINEAR;
            else if (ACE_OS::strcasecmp (name, "active") == 0)
              this->active_object_map_creation_parameters_.poa_lookup_strategy_for_transient_id_policy_ = TAO_ACTIVE_DEMUX;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBuniqueidpolicyreversedemuxstrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "dynamic") == 0)
              this->active_object_map_creation_parameters_.reverse_object_lookup_strategy_for_unique_id_policy_ = TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name, "linear") == 0)
              this->active_object_map_creation_parameters_.reverse_object_lookup_strategy_for_unique_id_policy_ = TAO_LINEAR;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBdemuxstrategy") == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Warning: -ORBdemuxstrategy is deprecated.  Please use "
                    "-ORBsystemidpolicydemuxstrategy or -ORBuseridpolicydemuxstrategy instead.\n"));
        curarg++;
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBpoalock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "thread") == 0)
              this->poa_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name, "null") == 0)
              this->poa_lock_type_ = TAO_NULL_LOCK;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBeventlock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "thread") == 0)
              this->poa_mgr_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name, "null") == 0)
              this->poa_mgr_lock_type_ = TAO_NULL_LOCK;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBconnectorlock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "thread") == 0)
              this->cached_connector_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name, "null") == 0)
              this->cached_connector_lock_type_ = TAO_NULL_LOCK;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBthreadflags") == 0)
      {
        curarg++;

        if (curarg < argc)
          this->tokenize (argv[curarg]);
      }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_STATIC_SVC_DEFINE (TAO_Default_Server_Strategy_Factory,
                       ASYS_TEXT ("Server_Strategy_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Server_Strategy_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Server_Strategy_Factory)

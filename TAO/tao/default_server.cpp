// ============================================================================
//
// = FILENAME
//     default_server.cpp
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#include "tao/default_server.h"
#include "tao/orb_core.h"

template <class SH>
TAO_Reactive_Strategy<SH>::TAO_Reactive_Strategy (void)
{}

template <class SH>
TAO_Reactive_Strategy<SH>::~TAO_Reactive_Strategy (void)
{}

template <class SH> int
TAO_Reactive_Strategy<SH>::activate_svc_handler (SH *svc_handler, void *arg)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::activate_svc_handler");

  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();

  int result = 0;

  if (r == 0)
    result = -1;

  // Register with the Reactor with the appropriate <mask>.
  else if (r->register_handler (svc_handler, this->mask_) == -1)
    result = -1;

  // If the implementation of the reactor uses event associations
  else if (r->uses_event_associations ())
    {
      // If we don't have non-block on, it won't work with
      // WFMO_Reactor
      // This maybe too harsh
      // if (!ACE_BIT_ENABLED (this->flags_, ACE_NONBLOCK))
      // goto failure;
      if (svc_handler->open ((void *) this) != -1)
        return 0;
      else
        result = -1;
    }
  else
    // Call up to our parent to do the SVC_HANDLER initialization.
    return this->inherited::activate_svc_handler (svc_handler, arg);

  if (result == -1)
    svc_handler->close (0);

  return result;
}


TAO_Default_Server_Strategy_Factory::TAO_Default_Server_Strategy_Factory (void)
  : thread_flags_ (0),
    object_table_size_ (TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE),
    object_lookup_strategy_ (TAO_DYNAMIC_HASH),
    concurrency_strategy_ (0),
    poa_lock_type_ (TAO_THREAD_LOCK),
    poa_mgr_lock_type_ (TAO_THREAD_LOCK)
{
}

TAO_Default_Server_Strategy_Factory::~TAO_Default_Server_Strategy_Factory (void)
{
  // Perform appropriate cleanup.
}

TAO_Default_Server_Strategy_Factory::CONCURRENCY_STRATEGY *
TAO_Default_Server_Strategy_Factory::concurrency_strategy (void)
{
  return this->concurrency_strategy_;
}

ACE_Lock *
TAO_Default_Server_Strategy_Factory::create_poa_lock (void)
{
  ACE_Lock *thelock = 0;

  switch (this->poa_lock_type_)
    {
    case TAO_THREAD_LOCK:
      ACE_NEW_RETURN (thelock,
                      ACE_Lock_Adapter<ACE_Thread_Mutex> (),
                      0);
      break;
    case TAO_NULL_LOCK:
      ACE_NEW_RETURN (thelock,
                      ACE_Lock_Adapter<ACE_Null_Mutex> (),
                      0);
      break;
    }
  
  return thelock;// Just to make sure we return something
}

ACE_Lock *
TAO_Default_Server_Strategy_Factory::create_poa_mgr_lock (void)
{
  ACE_Lock *thelock = 0;

  switch (this->poa_mgr_lock_type_)
    {
    case TAO_THREAD_LOCK:
      ACE_NEW_RETURN (thelock,
                      ACE_Lock_Adapter<ACE_Thread_Mutex> (),
                      0);
      break;
    case TAO_NULL_LOCK:
      ACE_NEW_RETURN (thelock,
                      ACE_Lock_Adapter<ACE_Null_Mutex> (),
                      0);
      break;
    }
  
  return thelock;// Just to make sure we return something
}

TAO_Object_Table_Impl *
TAO_Default_Server_Strategy_Factory::create_object_table (void)
{
  // Create the appropriate-sized object table based on passed
  // arguments.
  TAO_Object_Table_Impl *objtable = 0;

  switch (this->object_lookup_strategy_)
    {
    case TAO_LINEAR:
      ACE_NEW_RETURN (objtable,
		      TAO_Linear_ObjTable (this->object_table_size_),
		      0);
      break;
      // Don't do this one right now until we determine how to deal
      // with its reliance on a global singleton.
    case TAO_USER_DEFINED:
      // it is assumed that the user would have used the hooks to
      // supply a user-defined instance of the object table
      //
      // @@ Note that the usage below doesn't really fit very well
      // now.  We need for the userdef stuff to provide a creation
      // hook--IF we decide to keep the whole demultiplexing strategy
      // creation the way it is.  IMHO, the way that userdef stuff
      // should be done is to create the User_Server_Strategy_Factory
      // and just link it in.  The default server would only encompass
      // the strategies that are "shipped", so to speak. --cjc
      objtable = TAO_ORB_Core_instance()->oa_params()->userdef_lookup_strategy ();
      break;
    case TAO_ACTIVE_DEMUX:
      ACE_NEW_RETURN (objtable,
		      TAO_Active_Demux_ObjTable (this->object_table_size_),
		      0);
      break;
    case TAO_DYNAMIC_HASH:
    case TAO_NONE:
    default:
      ACE_NEW_RETURN (objtable,
		      TAO_Dynamic_Hash_ObjTable (this->object_table_size_),
		      0);
      break;
    }

  return objtable;
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
TAO_Default_Server_Strategy_Factory::open (void)
{
  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();

  if (reactive_strategy_.open (orb_core->reactor ()) == 0
      && threaded_strategy_.open (orb_core->thr_mgr (),
                                  this->thread_flags_) == 0)
    return 0;
  else
    return -1;
}

int
TAO_Default_Server_Strategy_Factory::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");

  int curarg;
  for (curarg = 0; curarg < argc && argv[curarg]; curarg++)
    {
      if (ACE_OS::strcmp (argv[curarg], "-ORBconcurrency") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *name = argv[curarg];

              if (ACE_OS::strcasecmp (name, "reactive") == 0)
                this->concurrency_strategy_ = &reactive_strategy_;
              else if (ACE_OS::strcasecmp (name, "thread-per-connection") == 0)
                this->concurrency_strategy_ = &threaded_strategy_;
            }
        }
      else if (ACE_OS::strcmp (argv[curarg], "-ORBtablesize") == 0)
        {
          curarg++;
          if (curarg < argc)
            this->object_table_size_ = ACE_OS::strtoul (argv[curarg], 0, 10);
        }
      else if (ACE_OS::strcmp (argv[curarg], "-ORBdemuxstrategy") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *name = argv[curarg];

              if (ACE_OS::strcasecmp (name, "dynamic") == 0)
                this->object_lookup_strategy_ = TAO_DYNAMIC_HASH;
              else if (ACE_OS::strcasecmp (name, "linear") == 0)
                this->object_lookup_strategy_ = TAO_LINEAR;
              else if (ACE_OS::strcasecmp (name, "active") == 0)
                this->object_lookup_strategy_ = TAO_ACTIVE_DEMUX;
              else if (ACE_OS::strcasecmp (name, "user") == 0)
                this->object_lookup_strategy_ = TAO_USER_DEFINED;
            }
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
      else if (ACE_OS::strcmp (argv[curarg], "-ORBpoamgrlock") == 0)
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
      else if (ACE_OS::strcmp (argv[curarg], "-ORBthreadflags") == 0)
        {
          curarg++;

          if (curarg < argc)
            this->tokenize (argv[curarg]);
        }
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Reactive_Strategy<TAO_Server_Connection_Handler>;
template class ACE_Reactive_Strategy<TAO_Server_Connection_Handler>;
template class ACE_Thread_Strategy<TAO_Server_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Reactive_Strategy<TAO_Server_Connection_Handler>
#pragma instantiate ACE_Reactive_Strategy<TAO_Server_Connection_Handler>
#pragma instantiate ACE_Thread_Strategy<TAO_Server_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_FACTORY_DEFINE (TAO, TAO_Default_Server_Strategy_Factory)

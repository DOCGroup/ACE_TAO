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

#define ACE_BUILD_SVC_DLL
#if 0
#include "tao/default_server.h"
#endif /* 0 */

#include "tao/corba.h"

TAO_Default_Server_Strategy_Factory::TAO_Default_Server_Strategy_Factory (void)
  : thread_flags_ (0),
    object_table_size_ (SERVER_OBJECT_TABLE_SIZE),
    concurrency_strategy_ (0),
    objtable_ (0)
{}

TAO_Default_Server_Strategy_Factory::~TAO_Default_Server_Strategy_Factory (void)
{
  // Perform appropriate cleanup.
  delete this->objtable_;
}

TAO_Default_Server_Strategy_Factory::CONCURRENCY_STRATEGY *
TAO_Default_Server_Strategy_Factory::concurrency_strategy (void)
{
  return this->concurrency_strategy_;
}

TAO_Object_Table *
TAO_Default_Server_Strategy_Factory::object_lookup_strategy (void)
{
  return this->objtable_;
}

// Evil macros b/c I'm lazy!
#define TAO_BEGINCHECK  if (0)
#define TAO_CHECKANDSET(sym) else if (ACE_OS::strcmp (flag, #sym) == 0) ACE_SET_BITS (this->thread_flags_, sym)
#define TAO_ENDCHECK

void
TAO_Default_Server_Strategy_Factory::tokenize (char *flag_string)
{
  // @@ Danger!  strtok not re-entrant...need to find a re-entrant version!
  // @@ Chris, please see ACE_OS::strtok_r ().  There are some examples of
  // how to use this in JAWS.

  for (char *flag = ACE_OS::strtok (flag_string, "|");
       flag != 0;
       flag = ACE_OS::strtok (0, "|"))
    {
      TAO_BEGINCHECK;
      TAO_CHECKANDSET (THR_DETACHED);
      TAO_CHECKANDSET (THR_BOUND);
      TAO_CHECKANDSET (THR_NEW_LWP);
      TAO_CHECKANDSET (THR_SUSPENDED);
      TAO_CHECKANDSET (THR_DAEMON);
      TAO_ENDCHECK;
    }
}

int
TAO_Default_Server_Strategy_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Server_Strategy_Factory::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");

  ACE_Get_Opt get_opt (argc, argv, "t:s:RTL:", 0);

  TAO_Demux_Strategy strat = TAO_NONE;

  // @@ Chris, I think this code should use the same option format
  // that is used by CORBA_ORB_init().  Can you please work with Andy
  // on this?

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 't':
          {
            char *temp = get_opt.optarg;
            this->tokenize (temp);
          }
          break;

        case 's':
          this->object_table_size_ = ACE_OS::strtoul (get_opt.optarg, NULL, 10);
          break;

        case 'R':
          // Set Reactive concurrency strategy
          this->concurrency_strategy_ = &reactive_strategy_;
          break;

        case 'T':
          // Set thread-per-connection concurrency strategy
          this->concurrency_strategy_ = &threaded_strategy_;
          break;

        case 'L':
          {
            char *name = get_opt.optarg;

	    // @@ Chris, why do we use "L" for "Demuxing strategy?"
	    // Also, please make sure that you document all of these
	    // options!

            if (ACE_OS::strcasecmp (name, "dynamic") == 0)
              strat = TAO_DYNAMIC_HASH;
            else if (ACE_OS::strcasecmp (name, "linear") == 0)
              strat = TAO_LINEAR;
            else if (ACE_OS::strcasecmp (name, "active") == 0)
              strat = TAO_ACTIVE_DEMUX;
            else if (ACE_OS::strcasecmp (name, "user") == 0)
              strat = TAO_USER_DEFINED;
          }
          break;
        }
    }

  // Create the appropriate-sized object table based on passed
  // arguments.
  switch (strat)
    {
    case TAO_LINEAR:
      ACE_NEW_RETURN (this->objtable_,
		      TAO_Linear_ObjTable (this->object_table_size_),
		      -1);
      break;
      // Don't do this one right now until we determine how to deal
      // with its reliance on a global singleton.
    case TAO_USER_DEFINED:
      // it is assumed that the user would have used the hooks to supply a
      // user-defined instance of the object table
      this->objtable_ = TAO_OA_PARAMS::instance ()->userdef_lookup_strategy ();
      break;
    case TAO_ACTIVE_DEMUX:
      ACE_NEW_RETURN (this->objtable_,
		      TAO_Active_Demux_ObjTable (this->object_table_size_),
		      -1);
      break;
    case TAO_DYNAMIC_HASH:
    case TAO_NONE:
    default:
      ACE_NEW_RETURN (this->objtable_,
		      TAO_Dynamic_Hash_ObjTable (this->object_table_size_),
		      -1);
      break;
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Reactive_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Thread_Strategy<TAO_OA_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Reactive_Strategy<TAO_OA_Connection_Handler>
#pragma instantiate ACE_Thread_Strategy<TAO_OA_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_SVC_FACTORY_DEFINE (TAO_Default_Server_Strategy_Factory)


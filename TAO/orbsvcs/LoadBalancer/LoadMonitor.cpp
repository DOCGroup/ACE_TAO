#include "Push_Handler.h"

#include "orbsvcs/LoadBalancing/LB_CPU_Monitor.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"


ACE_RCSID (LoadBalancer,
           LoadMonitor,
           "$Id$")


static const char * location_id = 0;
static const char * location_kind = 0;
static const char * mtype = "CPU";
static const char * mstyle = "PUSH";
static const char * custom_monitor_ior = 0;
static const char * monitor_ior_file = "monitor.ior";
static long push_interval = 15;

void
usage (const ACE_TCHAR * cmd)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:  %s ")
              ACE_TEXT ("-l <location_id> ")
              ACE_TEXT ("-k <location_kind> ")
              ACE_TEXT ("-t <CPU|Disk|Memory|Network> ")
              ACE_TEXT ("-s <PULL|PUSH> ")
              ACE_TEXT ("-i <push_interval> (in seconds,")
              ACE_TEXT (" and requires \"PUSH\" style monitoring) ")
              ACE_TEXT ("-m <custom_monitor_ior>")
              ACE_TEXT (" (overrides \"-t\", \"-l\" and \"-k\") ")
              ACE_TEXT ("-o <ior_output_file> ")
              ACE_TEXT ("-h ")
              ACE_TEXT ("\n\n"),
              cmd));
}

void
parse_args (int argc,
            ACE_TCHAR *argv[]
            ACE_ENV_ARG_DECL)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("l:k:t:s:i:m:o:h"));

  int c = 0;
  const char * s;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'm':
          ::custom_monitor_ior = get_opts.opt_arg ();
          break;

        case 'o':
          ::monitor_ior_file = get_opts.opt_arg ();
          break;

        case 'l':
          ::location_id = get_opts.opt_arg ();
          break;

        case 'k':
          ::location_kind = get_opts.opt_arg ();
          break;

        case 't':
          ::mtype = get_opts.opt_arg ();
          break;

        case 's':
          ::mstyle = get_opts.opt_arg ();
          break;

        case 'i':
          s = get_opts.opt_arg ();
          push_interval = ACE_OS::atoi (s);
          if (push_interval < 1)
            {
              ACE_ERROR ((LM_ERROR,
                           ACE_TEXT ("ERROR: Invalid push interval: %s\n"),
                           s));

              ACE_THROW (CORBA::BAD_PARAM ());
            }
          break;

        case 'h':
          ::usage (argv[0]);
          exit (0);
          break;

        default:
          ::usage (argv[0]);
          ACE_THROW (CORBA::BAD_PARAM ());
        }
    }
}

CosLoadBalancing::LoadMonitor_ptr
get_load_monitor (CORBA::ORB_ptr orb
                  ACE_ENV_ARG_DECL)
{
  if (::custom_monitor_ior != 0)
    {
      CORBA::Object_var obj =
        orb->string_to_object (::custom_monitor_ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosLoadBalancing::LoadMonitor::_nil ());

      return CosLoadBalancing::LoadMonitor::_narrow (obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      // The POA is only needed for the built-in load monitors since
      // they must be activated.

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosLoadBalancing::LoadMonitor::_nil ());

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosLoadBalancing::LoadMonitor::_nil ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosLoadBalancing::LoadMonitor::_nil ());

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosLoadBalancing::LoadMonitor::_nil ());

      if (ACE_OS::strcasecmp (::mtype, "CPU") == 0)
        {
          TAO_LB_CPU_Monitor * monitor = 0;
          ACE_NEW_THROW_EX (monitor,
                            TAO_LB_CPU_Monitor (::location_id,
                                                ::location_kind),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CosLoadBalancing::LoadMonitor::_nil ());

          // Transfer ownership to the POA.
          PortableServer::ServantBase_var s = monitor;

          return monitor->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
        }
      else if (ACE_OS::strcasecmp (::mtype, "Disk") == 0
               || ACE_OS::strcasecmp (::mtype, "Memory") == 0
               || ACE_OS::strcasecmp (::mtype, "Network") == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: \"%s\" load monitor currently ")
                      ACE_TEXT ("unimplemented.\n"),
                      ::mtype));

          ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                            CosLoadBalancing::LoadMonitor::_nil ());
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: Unrecognized built-in load monitor ")
                      ACE_TEXT ("type: <%s>.\n"),
                      ::mtype));

          ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                            CosLoadBalancing::LoadMonitor::_nil ());
        }
    }
}

void
register_load_monitor (CosLoadBalancing::LoadManager_ptr manager,
                       CosLoadBalancing::LoadMonitor_ptr monitor,
                       TAO_LB_Push_Handler * handler,
                       ACE_Reactor * reactor,
                       long & timer_id
                       ACE_ENV_ARG_DECL)
{
  PortableGroup::Location_var location =
    monitor->the_location (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (ACE_OS::strcasecmp (::mstyle, "PULL") == 0)
    {
      manager->register_load_monitor (monitor,
                                      location.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else if (ACE_OS::strcasecmp (::mstyle, "PUSH") == 0)
    {
      ACE_Time_Value interval (::push_interval, 0);
      ACE_Time_Value restart (::push_interval, 0);
      timer_id = reactor->schedule_timer (handler,
                                          0,
                                          interval,
                                          restart);

      if (timer_id == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: Unable to schedule timer for ")
                      ACE_TEXT ("\"PUSH\" style load monitoring.\n")));

          ACE_THROW (CORBA::INTERNAL ());
        }
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: Unrecognized load monitoring ")
                  ACE_TEXT ("style: <%s>.\n"),
                  ::mstyle));

      ACE_THROW (CORBA::BAD_PARAM ());
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // The usual server side boilerplate code.

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check the non-ORB arguments.
      ::parse_args (argc,
                    argv
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadMonitor_var load_monitor =
        ::get_load_monitor (orb.in ()
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::Location_var location =
        load_monitor->the_location (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The "LoadManager" reference should have already been
      // registered with the ORB by its ORBInitializer.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("LoadManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadManager_var load_manager =
        CosLoadBalancing::LoadManager::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This "push" handler will only be used if the load monitor
      // style is "PUSH".
      TAO_LB_Push_Handler push_handler (load_monitor.in (),
                                        location.in (),
                                        load_manager.in ());

      ACE_Reactor * reactor = orb->orb_core ()->reactor ();

      long timer_id = -1;

      ::register_load_monitor (load_manager.in (),
                               load_monitor.in (),
                               &push_handler,
                               reactor,
                               timer_id
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (timer_id != -1 && reactor->cancel_timer (timer_id) == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR:Unable to cancel \"push\" load ")
                      ACE_TEXT ("monitoring timer.\n")));

          // Just keep going.  We're shutting down anyway.
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO Load Monitor");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

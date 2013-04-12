// $Id$

#include "orbsvcs/Log_Macros.h"
#include "Push_Handler.h"
#include "Monitor_Signal_Handler.h"

#include "orbsvcs/LoadBalancing/LB_CPU_Load_Average_Monitor.h"
#include "orbsvcs/LoadBalancing/LB_conf.h"

#include "tao/ORB_Core.h"

#include "ace/Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_strings.h"

static const ACE_TCHAR * location_id = 0;
static const ACE_TCHAR * location_kind = 0;
static const ACE_TCHAR * mtype = ACE_TEXT("CPU");
static const ACE_TCHAR * mstyle = ACE_TEXT("PUSH");
static const ACE_TCHAR * custom_monitor_ior = 0;

// For the sake of consistency, make default push monitoring interval
// the same as the pull monitoring interval.
static long push_interval = TAO_LB_PULL_HANDLER_INTERVAL;

void
usage (const ACE_TCHAR * cmd)
{
  ORBSVCS_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:\n")
              ACE_TEXT ("  %s\n")
              ACE_TEXT ("    -l <location_id>\n")
              ACE_TEXT ("    -k <location_kind>\n")
              ACE_TEXT ("    -t <CPU | Disk | Memory | Network>\n")
              ACE_TEXT ("    -s <PULL | PUSH>\n")
              ACE_TEXT ("    -i <push_interval> (in seconds,")
              ACE_TEXT (" and requires \"PUSH\" style monitoring)\n")
              ACE_TEXT ("    -m <custom_monitor_ior>")
              ACE_TEXT (" (overrides \"-t\", \"-l\" and \"-k\")\n")
              ACE_TEXT ("    -h\n")
              ACE_TEXT ("\n"),
              cmd));
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("l:k:t:s:i:m:h"));

  int c = 0;
  const ACE_TCHAR * s = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'm':
          ::custom_monitor_ior = get_opts.opt_arg ();
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
              ORBSVCS_ERROR ((LM_ERROR,
                           ACE_TEXT ("ERROR: Invalid push interval: %s\n"),
                           s));

              throw CORBA::BAD_PARAM ();
            }
          break;

        case 'h':
          ::usage (argv[0]);
          ACE_OS::exit (0);
          break;

        default:
          ::usage (argv[0]);
          throw CORBA::BAD_PARAM ();
        }
    }
}

#if defined (linux) && defined (ACE_HAS_THREADS)
// Only the main thread can handle signals in Linux.  Run the
// LoadManager in thread other than main().
extern "C"
void *
TAO_LB_run_load_monitor (void * orb_arg)
{
  CORBA::ORB_ptr orb = static_cast<CORBA::ORB_ptr> (orb_arg);

  // Only the main thread should handle signals.
  //
  // @@ This is probably unnecessary since no signals should be
  //    delivered to this thread on Linux.
  ACE_Sig_Guard signal_guard;

  try
    {
      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO Load Monitor");

      return reinterpret_cast<void *> (-1);
    }

  return 0;
}
#endif  /* linux && ACE_HAS_THREADS */


CosLoadBalancing::LoadMonitor_ptr
get_load_monitor (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr root_poa)
{
  if (::custom_monitor_ior != 0)
    {
      CORBA::Object_var obj =
        orb->string_to_object (::custom_monitor_ior);

      return CosLoadBalancing::LoadMonitor::_narrow (obj.in ());
    }
  else
    {
      // The POA is only needed for the built-in load monitors since
      // they must be activated.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (ACE_OS::strcasecmp (::mtype, ACE_TEXT("CPU")) == 0)
        {
          TAO_LB_CPU_Load_Average_Monitor * monitor = 0;
          ACE_NEW_THROW_EX (monitor,
                            TAO_LB_CPU_Load_Average_Monitor (::location_id,
                                                             ::location_kind),
                            CORBA::NO_MEMORY ());

          // Transfer ownership to the POA.
          PortableServer::ServantBase_var s = monitor;

          return monitor->_this ();
        }
      else if (ACE_OS::strcasecmp (::mtype, ACE_TEXT("Disk")) == 0
               || ACE_OS::strcasecmp (::mtype, ACE_TEXT("Memory")) == 0
               || ACE_OS::strcasecmp (::mtype, ACE_TEXT("Network")) == 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: \"%s\" load monitor currently ")
                      ACE_TEXT ("unimplemented.\n"),
                      ::mtype));

          throw CORBA::NO_IMPLEMENT ();
        }
      else
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: Unrecognized built-in load monitor ")
                      ACE_TEXT ("type: <%s>.\n"),
                      ::mtype));

          throw CORBA::BAD_PARAM ();
        }
    }
}

void
register_load_monitor (CosLoadBalancing::LoadManager_ptr manager,
                       CosLoadBalancing::LoadMonitor_ptr monitor,
                       TAO_LB_Push_Handler * handler,
                       ACE_Reactor * reactor,
                       long & timer_id)
{
  if (ACE_OS::strcasecmp (::mstyle, ACE_TEXT("PULL")) == 0)
    {
      PortableGroup::Location_var location =
        monitor->the_location ();

      manager->register_load_monitor (location.in (),
                                      monitor);
    }
  else if (ACE_OS::strcasecmp (::mstyle, ACE_TEXT("PUSH")) == 0)
    {
      ACE_Time_Value interval (::push_interval, 0);
      ACE_Time_Value restart (::push_interval, 0);
      timer_id = reactor->schedule_timer (handler,
                                          0,
                                          interval,
                                          restart);

      if (timer_id == -1)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: Unable to schedule timer for ")
                      ACE_TEXT ("\"PUSH\" style load monitoring.\n")));

          throw CORBA::INTERNAL ();
        }
    }
  else
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: Unrecognized load monitoring ")
                  ACE_TEXT ("style: <%s>.\n"),
                  ::mstyle));

      throw CORBA::BAD_PARAM ();
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // The usual server side boilerplate code.

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Check the non-ORB arguments.
      ::parse_args (argc,
                    argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      CosLoadBalancing::LoadMonitor_var load_monitor =
        ::get_load_monitor (orb.in (),
                            root_poa.in ());

      PortableGroup::Location_var location =
        load_monitor->the_location ();

      // The "LoadManager" reference should have already been
      // registered with the ORB by its ORBInitializer.
      obj = orb->resolve_initial_references ("LoadManager");

      CosLoadBalancing::LoadManager_var load_manager =
        CosLoadBalancing::LoadManager::_narrow (obj.in ());

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
                               timer_id);

#if defined (linux) && defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn (::TAO_LB_run_load_monitor,
                                                  orb.in ()) == -1)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             "ERROR:  Unable to spawn TAO LoadMonitor's "
                             "ORB thread.\n"),
                            -1);
        }

      ACE_Sig_Set sigset;
      sigset.sig_add (SIGINT);
      sigset.sig_add (SIGTERM);

      int signum = -1;

      // Block waiting for the registered signals.
      if (ACE_OS::sigwait (sigset, &signum) == -1)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) %p\n",
                             "ERROR waiting on signal"),
                            -1);
        }

      ACE_ASSERT (signum == SIGINT || signum == SIGTERM);

      // Deregister the LoadMonitor from the LoadManager in the PULL
      // load monitoring case.
      if (timer_id == -1)
        {
          load_manager->remove_load_monitor (location.in ());
        }
#else
      // Activate/register the signal handler that (attempts) to
      // ensure graceful shutdown of the LoadMonitor so that
      // LoadMonitors registered with the LoadManager can be
      // deregistered.
      CosLoadBalancing::LoadManager_ptr tmp;

      if (timer_id == -1)
        tmp = load_manager.in ();   // PULL monitoring
      else
        tmp = CosLoadBalancing::LoadManager::_nil ();  // PUSH
                                                       // monitoring
      TAO_LB_Monitor_Signal_Handler signal_handler (
         orb.in (),
         root_poa.in (),
         tmp,
         location.in ());

      if (signal_handler.activate () != 0)
        return -1;

      // @@ There is a subtle race condition here.  If the signal
      //    handler thread shuts down the ORB before it is run, the
      //    below call to ORB::run() will throw a CORBA::BAD_INV_ORDER
      //    exception.
      orb->run ();

      // Wait for the signal handler thread to finish
      // before the process exits.
      signal_handler.wait ();
#endif  /* linux && ACE_HAS_THREADS */

      if (timer_id != -1 && reactor->cancel_timer (timer_id) == 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: Unable to cancel \"push\" load ")
                      ACE_TEXT ("monitoring timer.\n")));

          // Just keep going.  We're shutting down anyway.
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO Load Monitor");

      return -1;
    }

  return 0;
}

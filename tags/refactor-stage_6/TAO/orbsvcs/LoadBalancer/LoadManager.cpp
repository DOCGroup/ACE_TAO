#include "Signal_Handler.h"

#include "orbsvcs/LoadBalancing/LB_LoadManager.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"

#if defined (linux) && defined (ACE_HAS_THREADS)
# include "ace/Signal.h"
#endif /* linux && ACE_HAS_THREADS */


ACE_RCSID (LoadBalancer,
           LoadBalancer,
           "$Id$")


static const char * lm_ior_file = "lm.ior";

void
usage (const ACE_TCHAR * cmd)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:\n")
              ACE_TEXT ("  %s\n")
              ACE_TEXT ("    -o <ior_output_file>\n")
              ACE_TEXT ("    -s <RoundRobin | Random | LeastLoaded>\n")
              ACE_TEXT ("    -h\n")
              ACE_TEXT ("\n")
              ACE_TEXT (" NOTE: Standard default values will be used ")
              ACE_TEXT ("for \"LeastLoaded\" strategy.\n"),
              cmd));
}

void
parse_args (int argc,
            ACE_TCHAR *argv[],
            int & default_strategy
            ACE_ENV_ARG_DECL)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:s:h"));

  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ::lm_ior_file = get_opts.opt_arg ();
          break;

        case 's':
          if (ACE_OS::strcasecmp (get_opts.opt_arg (),
                                  "RoundRobin") == 0)
            default_strategy = 0;
          else if (ACE_OS::strcasecmp (get_opts.opt_arg (),
                                       "Random") == 0)
            default_strategy = 1;
          else if (ACE_OS::strcasecmp (get_opts.opt_arg (),
                                       "LeastLoaded") == 0)
            default_strategy = 2;
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Unknown strategy, using RoundRobin\n")));
          break;

        case 'h':
          ::usage (argv[0]);
          ACE_OS::exit (0);
          break;

        default:
          ::usage (argv[0]);
          ACE_THROW (CORBA::BAD_PARAM ());
        }
    }
}

#if defined (linux) && defined (ACE_HAS_THREADS)
// Only the main thread can handle signals in Linux.  Run the
// LoadManager in thread other than main().
extern "C"
void *
TAO_LB_run_load_manager (void * orb_arg)
{
  CORBA::ORB_ptr orb = ACE_static_cast (CORBA::ORB_ptr, orb_arg);

  // Only the main thread should handle signals.
  //
  // @@ This is probably unnecessary since no signals should be
  //    delivered to this thread on Linux.
  ACE_Sig_Guard signal_guard;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO Load Manager");

      return ACE_reinterpret_cast (void *, -1);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (ACE_reinterpret_cast (void *, -1));

  return 0;
}
#endif  /* linux && ACE_HAS_THREADS */

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

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // "built-in" strategies are the following:
      //   0 = RoundRobin
      //   1 = Random
      //   2 = LeastLoaded
      int default_strategy = 1;

      // Check the non-ORB arguments.
      ::parse_args (argc,
                    argv,
                    default_strategy
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_LB_LoadManager * lm;
      ACE_NEW_THROW_EX (lm,
                        TAO_LB_LoadManager,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_lm = lm;

      // Initalize the LoadManager servant.
      lm->init (orb->orb_core ()->reactor (),
                orb.in (),
                root_poa.in ()
                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::Properties props (1);
      props.length (1);
      props[0].nam.length (1);
      props[0].nam[0].id =
        CORBA::string_dup ("org.omg.CosLoadBalancing.StrategyInfo");

      CosLoadBalancing::StrategyInfo strategy_info;

      switch (default_strategy)
        {
        case 0:
          strategy_info.name = CORBA::string_dup ("RoundRobin");
          break;
        case 1:
          strategy_info.name = CORBA::string_dup ("Random");
          break;
        case 2:
          strategy_info.name = CORBA::string_dup ("LeastLoaded");
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("ERROR: LoadBalancer internal error.\n")
                            ACE_TEXT ("       Unknown built-in strategy.\n")),
                            -1);
        }

      props[0].val <<= strategy_info;

      lm->set_default_properties (props
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadManager_var load_manager =
        lm->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (load_manager.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      FILE * lm_ior = ACE_OS::fopen (lm_ior_file, "w");
      ACE_OS::fprintf (lm_ior, "%s", str.in ());
      ACE_OS::fclose (lm_ior);

#if defined (linux) && defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn (::TAO_LB_run_load_manager,
                                                  orb.in ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR:  Unable to spawn TAO LoadManager's "
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
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) %p\n",
                             "ERROR waiting on signal"),
                            -1);
        }

      ACE_ASSERT (signum == SIGINT || signum == SIGTERM);
#else
      // Activate/register the signal handler that (attempts) to
      // ensure graceful shutdown of the LoadManager so that remote
      // resources created by the LoadManager can be cleaned up.
      TAO_LB_Signal_Handler signal_handler (orb.in (), root_poa.in ());

      if (signal_handler.activate () != 0)
        return -1;

      // @@ There is a subtle race condition here.  If the signal
      //    handler thread shuts down the ORB before it is run, the
      //    below call to ORB::run() will throw a CORBA::BAD_INV_ORDER
      //    exception.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif  /* linux && ACE_HAS_THREADS */

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
//   ACE_CATCH (PortableGroup::InvalidProperty, ex)
//     {
//       ACE_DEBUG ((LM_DEBUG, "Property ----> %s\n", ex.nam[0].id.in ()));
//     }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO Load Manager");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

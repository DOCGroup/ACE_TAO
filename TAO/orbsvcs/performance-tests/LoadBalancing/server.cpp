// $Id$

#include "Roundtrip.h"
#include "ORBInitializer.h"
#include "RPS_Monitor.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "tao/Strategies/advanced_resource.h"

ACE_RCSID (LoadBalancing,
           server,
           "$Id$")

const char *ior_output_file = "test.ior";

CORBA::Float reject_threshold = 0;
CORBA::Float critical_threshold = 0;
CORBA::Float dampening = 0;
const char * strategy = "Random";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:r:c:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        strategy = get_opts.opt_arg ();
        break;

      case 'r':
        reject_threshold =
          ACE_static_cast (CORBA::Float, ::atof (get_opts.opt_arg ()));
        break;

      case 'c':
        critical_threshold =
          ACE_static_cast (CORBA::Float, ::atof (get_opts.opt_arg ()));
        break;

      case 'd':
        dampening =
          ACE_static_cast (CORBA::Float, ::atof (get_opts.opt_arg ()));
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "  -o <iorfile>\n"
                           "  -s <RoundRobin | Random | LeastLoaded>\n"
                           "  -r <reject threshold>    (LeastLoaded only)\n"
                           "  -c <critical threshold>  (LeastLoaded only)\n"
                           "  -d <damping value>       (LeastLoaded only)\n"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

CORBA::Object_ptr
join_object_group (CORBA::ORB_ptr orb,
                   CosLoadBalancing::LoadManager_ptr lm,
                   const PortableGroup::Location & location
                   ACE_ENV_ARG_PARAMETER)
{
  CORBA::Object_var ns_object =
    orb->resolve_initial_references ("NameService"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CosNaming::NamingContext_var nc =
    CosNaming::NamingContext::_narrow (ns_object.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CosNaming::Name name (1);
  name.length (1);

  name[0].id = "RoundtripObjectGroup";
  name[0].kind = "Object Group";

  CORBA::Object_var group;

  ACE_TRY
    {
      group = nc->resolve (name
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, ex)
    {
      // Object group not created.  Create one.
      const char repository_id[] = "IDL:Test/Roundtrip:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property & property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("omg.org.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      PortableGroup::GenericFactory::FactoryCreationId_var fcid;

      group = lm->create_object (repository_id,
                                 criteria,
                                 fcid.out ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_TRY_EX (foo)
        {
          nc->bind (name,
                    group.in ()
                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (foo);

          PortableGroup::Properties props (1);
          props.length (1);
          props[0].nam.length (1);
          props[0].nam[0].id =
            CORBA::string_dup ("org.omg.CosLoadBalancing.StrategyInfo");

          CosLoadBalancing::StrategyInfo strategy_info;

          strategy_info.name = CORBA::string_dup (strategy);

          if (ACE_OS::strcasecmp (strategy, "LeastLoaded") == 0
              && (reject_threshold != 0
                  || critical_threshold != 0
                  || dampening != 0))
            {
              CORBA::ULong len = 1;

              PortableGroup::Properties & props =
                strategy_info.props;

              if (reject_threshold != 0)
                {
                  const CORBA::ULong i = len - 1;

                  props.length (len++);

                  props[i].nam.length (1);
                  props[i].nam[0].id =
                    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy.LeastLoaded.RejectThreshold");
                  props[i].val <<= reject_threshold;
                }

              if (critical_threshold != 0)
                {
                  const CORBA::ULong i = len - 1;

                  props.length (len++);

                  props[i].nam.length (1);
                  props[i].nam[0].id =
                    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy.LeastLoaded.CriticalThreshold");
                  props[i].val <<= critical_threshold;
                }

              if (dampening != 0)
                {
                  const CORBA::ULong i = len - 1;

                  props.length (len++);

                  props[i].nam.length (1);
                  props[i].nam[0].id =
                    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy.LeastLoaded.Dampening");
                  props[i].val <<= dampening;
                }

            }

          props[0].val <<= strategy_info;

          lm->set_default_properties (props
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ex)
        {
          // Somebody beat us to creating the object group.  Clean up
          // the one we created.
          lm->delete_object (fcid.in ()
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (foo);

          group = nc->resolve (name
                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (foo);
        }
      ACE_ENDTRY;
      ACE_TRY_CHECK;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  Roundtrip * roundtrip_impl;
  ACE_NEW_THROW_EX (roundtrip_impl,
                    Roundtrip (orb),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var owner_transfer (roundtrip_impl);

  Test::Roundtrip_var roundtrip =
    roundtrip_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  group = lm->add_member (group.in (),
                          location,
                          roundtrip.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return group._retn ();
}

int
main (int argc, char *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      ORBInitializer *initializer = 0;
      ACE_NEW_RETURN (initializer,
                      ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var lm_object =
        orb->resolve_initial_references ("LoadManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadManager_var load_manager =
        CosLoadBalancing::LoadManager::_narrow (lm_object.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RPS_Monitor * monitor_servant;
      ACE_NEW_THROW_EX (monitor_servant,
                        RPS_Monitor (initializer->interceptor ()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_monitor_servant (monitor_servant);

      CosLoadBalancing::LoadMonitor_var load_monitor =
        monitor_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::Location_var location =
        load_monitor->the_location (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var roundtrip =
        ::join_object_group (orb.in (),
                             load_manager.in (),
                             location.in ()
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_LB_LoadAlert & alert_servant = initializer->load_alert ();

      CosLoadBalancing::LoadAlert_var load_alert =
        alert_servant._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::String_var ior =
        orb->object_to_string (roundtrip.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      load_manager->register_load_monitor (location.in (),
                                           load_monitor.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      load_manager->register_load_alert (location.in (),
                                         load_alert.in ()
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

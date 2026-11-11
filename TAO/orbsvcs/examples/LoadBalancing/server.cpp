#include "tao/ORBInitializer_Registry.h"
#include "StockFactory.h"
#include "ORBInitializer.h"
#include "RPS_Monitor.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_strings.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("obj.ior")};
  ACE_TCHAR const *strategy {ACE_TEXT ("LeastLoaded")};
  CORBA::Float reject_threshold {};
  CORBA::Float critical_threshold {};
  CORBA::Float dampening {};
  int number {};
};

int parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:n:s:r:c:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        options.strategy = get_opts.opt_arg ();
        break;

      case 'n':
        options.number = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'r':
        options.reject_threshold =
          static_cast<CORBA::Float> (ACE_OS::atof (get_opts.opt_arg ()));
        break;

      case 'c':
        options.critical_threshold =
          static_cast<CORBA::Float> (ACE_OS::atof (get_opts.opt_arg ()));
        break;

      case 'd':
        options.dampening =
          static_cast<CORBA::Float> (ACE_OS::atof (get_opts.opt_arg ()));
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s\n")
                           ACE_TEXT ("  -o <iorfile>\n")
                           ACE_TEXT ("  -s <RoundRobin | Random | LeastLoaded>\n")
                           ACE_TEXT ("  -r <reject threshold>    (LeastLoaded only)\n")
                           ACE_TEXT ("  -c <critical threshold>  (LeastLoaded only)\n")
                           ACE_TEXT ("  -d <damping value>       (LeastLoaded only)\n\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

CORBA::Object_ptr
join_object_group (CORBA::ORB_ptr orb,
                   CosLoadBalancing::LoadManager_ptr lm,
                   PortableGroup::Location const &location,
                   Server_Options const &options)
{
  CORBA::Object_var ns_object =
    orb->resolve_initial_references ("NameService");

  CosNaming::NamingContext_var nc =
    CosNaming::NamingContext::_narrow (ns_object.in ());

  CosNaming::Name name (1);
  name.length (1);

  name[0].id = "StockFactoryObjectGroup";
  name[0].kind = "Object Group";

  CORBA::Object_var group;

  try
    {
      group = nc->resolve (name);
    }
  catch (CosNaming::NamingContext::NotFound const &)
    {
      // Object group not created.  Create one.
      constexpr char repository_id[] = "IDL:Test/StockFactory:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property & property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("omg.org.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue const msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      PortableGroup::GenericFactory::FactoryCreationId_var fcid;

      group = lm->create_object (repository_id,
                                 criteria,
                                 fcid.out ());

      try
        {
          nc->bind (name,
                    group.in ());

          PortableGroup::Properties props (1);
          props.length (1);
          props[0].nam.length (1);
          props[0].nam[0].id =
            CORBA::string_dup ("org.omg.CosLoadBalancing.StrategyInfo");

          CosLoadBalancing::StrategyInfo strategy_info;

          strategy_info.name = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (options.strategy));

          if (ACE_OS::strcasecmp (options.strategy, ACE_TEXT ("LeastLoaded")) == 0
              && (!ACE::is_equal (options.reject_threshold, 0.0f)
                  || !ACE::is_equal (options.critical_threshold, 0.0f)
                  || !ACE::is_equal (options.dampening, 0.0f)))
            {
              CORBA::ULong len = 1;

              PortableGroup::Properties & props =
                strategy_info.props;

              if (!ACE::is_equal (options.reject_threshold, 0.0f))
                {
                  CORBA::ULong const i = len - 1;

                  props.length (len);
                  ++len;

                  props[i].nam.length (1);
                  props[i].nam[0].id =
                    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy.LeastLoaded.RejectThreshold");
                  props[i].val <<= options.reject_threshold;
                }

              if (!ACE::is_equal (options.critical_threshold, 0.0f))
                {
                  CORBA::ULong const i = len - 1;

                  props.length (len);
                  ++len;

                  props[i].nam.length (1);
                  props[i].nam[0].id =
                    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy.LeastLoaded.CriticalThreshold");
                  props[i].val <<= options.critical_threshold;
                }

              if (!ACE::is_equal (options.dampening, 0.0f))
                {
                  CORBA::ULong const i = len - 1;

                  props.length (len);
                  ++len;

                  props[i].nam.length (1);
                  props[i].nam[0].id =
                    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy.LeastLoaded.Dampening");
                  props[i].val <<= options.dampening;
                }

            }

          props[0].val <<= strategy_info;

          lm->set_default_properties (props);
        }
      catch (CosNaming::NamingContext::AlreadyBound const &)
        {
          // Somebody beat us to creating the object group.  Clean up
          // the one we created.
          lm->delete_object (fcid.in ());

          group = nc->resolve (name);
        }
    }

  StockFactory *stockfactory_impl {};
  ACE_NEW_THROW_EX (stockfactory_impl,
                    StockFactory (orb, options.number),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var owner_transfer (stockfactory_impl);

  Test::StockFactory_var stockfactory =
    stockfactory_impl->_this ();

  group = lm->add_member (group.in (),
                          location,
                          stockfactory.in ());

  return group._retn ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;

  try
    {
      ORBInitializer *initializer {};
      ACE_NEW_RETURN (initializer,
                      ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      poa_manager->activate ();

      CORBA::Object_var lm_object =
        orb->resolve_initial_references ("LoadManager");

      CosLoadBalancing::LoadManager_var load_manager =
        CosLoadBalancing::LoadManager::_narrow (lm_object.in ());

      RPS_Monitor *monitor_servant {};
      ACE_NEW_THROW_EX (monitor_servant,
                        RPS_Monitor (initializer->interceptor ()),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var safe_monitor_servant (monitor_servant);

      CosLoadBalancing::LoadMonitor_var load_monitor =
        monitor_servant->_this ();

      PortableGroup::Location_var location =
        load_monitor->the_location ();

      CORBA::Object_var stockfactory =
        ::join_object_group (orb.in (),
                             load_manager.in (),
                             location.in (),
                             options);

      TAO_LB_LoadAlert & alert_servant = initializer->load_alert ();

      CosLoadBalancing::LoadAlert_var load_alert =
        alert_servant._this ();

      CORBA::String_var ior =
        orb->object_to_string (stockfactory.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      load_manager->register_load_monitor (location.in (),
                                           load_monitor.in ());

      load_manager->register_load_alert (location.in (),
                                         load_alert.in ());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) server - event loop finished\n")));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("lb_server exception");
      return 1;
    }

  return 0;
}

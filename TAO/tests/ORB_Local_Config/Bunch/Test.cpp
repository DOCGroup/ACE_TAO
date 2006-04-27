// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/IIOP_Factory.h"
#include "tao/ORBInitializer_Registry_Adapter.h"

#include "ace/Thread_Manager.h"
#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tests, server, "$Id$")


#include "Service_Configuration_Per_ORB.h"

// @brief The "new" interfaces must be compatible with the "old" ones

int
testCompatibility (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testCompatibility");

  // This uses the same default ACE_Service_Repository
  ACE_Service_Gestalt_Test glob;

  // Use the "old" interface
  ACE_ASSERT (0 == ACE_Service_Config::process_directive
                     (ace_svc_desc_TAO_CORBANAME_Parser));
  ACE_ASSERT (0 == ACE_Service_Config::process_directive
                     (ace_svc_desc_TAO_CORBALOC_Parser));

  {
    // This uses the same default ACE_Service_Repository
    ACE_Service_Gestalt_Test one;

    TAO_CORBANAME_Parser* p20 =
      ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance (&one, "CORBANAME_Parser");
    ACE_ASSERT ((p20 != 0));

    TAO_CORBALOC_Parser* p21 =
      ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance (&one, "CORBALOC_Parser");
    ACE_ASSERT ((p21 != 0));

    ACE_DEBUG ((LM_DEBUG, "\tglobal.services_count () -> %d\n",
                one.services_count ()));
    ACE_ASSERT (33 == one.services_count ());

    // Exiting this scope should fini all services ...
  }

  TAO_CORBANAME_Parser* p20 =
    ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance ("CORBANAME_Parser");
  ACE_ASSERT ((p20 == 0));

  TAO_CORBALOC_Parser* p21 =
    ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance ("CORBALOC_Parser");
  ACE_ASSERT ((p21 == 0));



  p20 = ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance (&glob, "CORBANAME_Parser");
  ACE_ASSERT ((p20 == 0));

  p21 =ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance (&glob, "CORBALOC_Parser");
  ACE_ASSERT ((p21 == 0));

  return 0;
}

// @brief Test commandline processing


int
testCommandLineDirectives (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testCommandLineDirectives");

  ACE_ARGV new_argv;
  ACE_ASSERT (new_argv.add (ACE_TEXT ("-f")) != -1
              && new_argv.add (ACE_TEXT ("-S")) != -1
              && new_argv.add (ACE_TEXT ("d1")) != -1
              && new_argv.add (ACE_TEXT ("-S")) != -1
              && new_argv.add (ACE_TEXT ("d2")) != -1);

  ACE_Service_Gestalt_Test g(5);
  ACE_ASSERT (g.parse_args (new_argv.argc (),
                            new_argv.argv ()) != -1
              || errno == ENOENT);

  ACE_DEBUG ((LM_DEBUG, "\tg.command_line_directives_count () -> %d\n",
              g.command_line_directives_count ()));

  ACE_ASSERT (2 == g.command_line_directives_count ());
  return 0;
}



// @brief Loading dynamic services in a local repository

int
testOpenDynamicServices (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testOpenDynamicServices");

  ACE_ARGV new_argv;

  // Process the Service Configurator directives in this test's
  ACE_ASSERT (new_argv.add (ACE_TEXT ("bogus")) != -1
              && new_argv.add (ACE_TEXT ("-f")) != -1
              && new_argv.add (file_Service_Config_Test ()) != -1);

  // We need this scope to make sure that the destructor for the
  // <ACE_Service_Config> gets called.
  ACE_Service_Gestalt_Test daemon(10);

  ACE_ASSERT (daemon.open (new_argv.argc (),
                           new_argv.argv ()) != -1 || errno == ENOENT);

  ACE_DEBUG ((LM_DEBUG,
              "\tdaemon.services_count () -> %d\n",
              daemon.services_count ()));

  ACE_ASSERT (5 == daemon.services_count ());

  // Since the loaded services start their own threads, wait until all of them
  // are done to avoid pulling the rug under their feet.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}





// @brief Try loading the ORBInitializer_Registry locally

int
testORBInitializer_Registry(int, ACE_TCHAR *[])
{
  ACE_TRACE ("testORBInitializer_Registry");

  ACE_Service_Gestalt_Test glob;      // The global service registrations are here
  ACE_Service_Gestalt_Test one (10);  // Localized ones go here

  size_t glob_size = glob.services_count ();
  size_t loca_size = one.services_count ();

  // It is expected to be empty at this point since it is not using the global repo
  ACE_ASSERT (loca_size == 0);

  // Lookup it up.
  TAO::ORBInitializer_Registry_Adapter* oir =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
      (&one, "ORBInitializer_Registry");

#if defined (TAO_AS_STATIC_LIBS)
  ACE_ASSERT ((oir != 0));
#else
  ACE_ASSERT ((oir == 0));
#endif


#if !defined (TAO_AS_STATIC_LIBS)
      // In case we build shared, try to load the PI Client library, in a
      // static build we just can't do this, so don't try it, lower layers
      // output an error then.
  if (oir == 0)
    {
      one.process_directive (
        ACE_DYNAMIC_SERVICE_DIRECTIVE("ORBInitializer_Registry",
                                      "TAO_PI",
                                      "_make_ORBInitializer_Registry",
                                      ""));
      oir =
        ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
          (&one, "ORBInitializer_Registry");
    }
  ACE_ASSERT ((oir != 0));
#endif


  ACE_DEBUG ((LM_DEBUG,
              "Expected %d global static service registrations, found %d\n",
              glob_size,
              glob.services_count ()));

  ACE_ASSERT (glob_size == glob.services_count ());

  ACE_DEBUG ((LM_DEBUG,
              "Expected %d local static service registrations, found %d\n",
              5,
              one.services_count ()));

  // The local repository must have asquired also the static services
  // registered within the dynamic service we just loaded. As of this
  // writing, loading ORBInitializer_Registry causes the registration of
  // four other (static) services. The PolicyFactory_Loader,
  // ClientRequestInterceptor_Adapter_Factory and PICurrent_Loader are
  // registred explicitely, while CodecFactory_Loader - indirectly.
  // Hence the number 5.

  ACE_ASSERT (loca_size != one.services_count ());
  ACE_ASSERT (5 == one.services_count ());

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) try global dynamic service on ORBInitializer_Registry ...\n"));

  // Try to instantiate the dynamic service from the global repository ...
  TAO::ORBInitializer_Registry_Adapter* oir1 =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
      ("ORBInitializer_Registry");

  ACE_ASSERT ((oir1 == 0)); // Right! It should not have been global.

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) try local dynamic service on ORBInitializer_Registry ...\n"));

  // Try to instantiate the dynamic service from the local repository ...
  TAO::ORBInitializer_Registry_Adapter* oir2 =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
      (&one, "ORBInitializer_Registry");

  ACE_ASSERT ((oir2 != 0)); // Right! That's local.

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Explicitely initialize ORBInitializer_Registry"
              " (although Dynamic_Service already did it) ...\n"));

  // ... initialize, but note that without the guard there is nothing to tell
  // the service which gestalt must be used. If init() does static service
  // registrations, those may end up in the wrong (global) gestalt and will
  // be in memory, which may not be mapped by finalization time!
  {
    ACE_Service_Config_Guard guard (&one);
    oir2->init (0,0);
  }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Try dynamic_service on a "
              "dependent static service (CodecFactory_Loader) ...\n"));

  // ... and also one of the dependent static services
  ACE_ASSERT (0 != ACE_Dynamic_Service <ACE_Service_Object>::instance
                      (&one, "CodecFactory_Loader"));


  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Explicitely dynamic_service PolicyFactory_Loader"
              " (although ORBInitializer_Registry already did it) ...\n"));

  one.process_directive
    (ACE_DYNAMIC_SERVICE_DIRECTIVE("PolicyFactory_Loader",
                                   "TAO_PI",
                                   "_make_TAO_PolicyFactory_Loader",
                                   ""));

  ACE_ASSERT (0 != ACE_Dynamic_Service <ACE_Service_Object>::instance
                      (&one, "PolicyFactory_Loader"));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) We're done testing.\n"));
  return 0;
}


// @brief Test the helper components used to implement the temporary
// substitution of the repository currently used as "global" for the
// sake of registering static services, which are dependent on a dynamic
// service

int
testTSSGestalt (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testTSSGestalt");

  ACE_Service_Gestalt_Test one (10);  // Localized ones go here

  ACE_Service_Gestalt *global_instance = ACE_Service_Config::instance ();

  ACE_ASSERT (global_instance == ACE_Service_Config::instance ());
  ACE_ASSERT (global_instance != &one);

  {
    ACE_Service_Config_Guard temporary (&one);

    ACE_Service_Gestalt *global_instance2 = ACE_Service_Config::instance ();

    ACE_ASSERT (global_instance != global_instance2);
    ACE_ASSERT (global_instance2 == &one);
  }

  ACE_ASSERT (global_instance == ACE_Service_Config::instance ());
  ACE_ASSERT (global_instance != &one);

  return 0;
}



// @brief the main driver

int
run_main(int argc, ACE_TCHAR *argv[])
{
  testCompatibility (argc, argv);
  testCommandLineDirectives (argc, argv);
  testOpenDynamicServices (argc, argv);
  testORBInitializer_Registry(argc, argv);
  testTSSGestalt(argc, argv);
  return 0;
}





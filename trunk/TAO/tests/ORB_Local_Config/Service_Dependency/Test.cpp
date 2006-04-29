// $Id$

/// It's a test - we need ACE_ASSERT
#ifdef ACE_NDEBUG
#  undef ACE_NDEBUG
#endif

#include "tao/Codeset_Descriptor_Base.h"
#include "tao/Codeset_Manager_Factory_Base.h"
#include "tao/Codeset_Manager.h"
#include "tao/ORBInitializer_Registry_Adapter.h"

#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "ace/Dynamic_Service_Dependency.h"
#include "ace/Thread_Manager.h"

#include "Service_Configuration_Per_ORB.h"

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
testORBInitializer_Registry (int , ACE_TCHAR *[])
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



// @brief ...


int
testServiceDependency (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testServiceDependency");

  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_DLL) == %d\n", sizeof (ACE_DLL)));
  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_Dynamic_Service_Dependency) == %d\n", sizeof (ACE_Dynamic_Service_Dependency)));

  TAO_Codeset_Manager *codeset_manager = 0;
  ACE_Dynamic_Service_Dependency *pdep = 0;

  {
    /// Start a block to limit the lifespan of a gestalt
    ACE_Service_Gestalt_Test one (10);

    int result = one.process_directive
      (ACE_DYNAMIC_SERVICE_DIRECTIVE("TAO_Codeset",
                                     "TAO_Codeset",
                                     "_make_TAO_Codeset_Manager_Factory",
                                     ""));
    ACE_ASSERT (result == 0);
    ACE_UNUSED_ARG (result);

    TAO_Codeset_Manager_Factory_Base *factory =
      ACE_Dynamic_Service<TAO_Codeset_Manager_Factory_Base>::instance (&one, "TAO_Codeset");
    ACE_ASSERT (factory != 0);

    codeset_manager = factory->create ();
    ACE_ASSERT (codeset_manager != 0);

    ACE_DEBUG ((LM_DEBUG, "Creating dependency ...\n"));

    // [1]
    //
    // Stating that a thing depends on that dynamic service. Why?
    // Read on ...

    pdep = new ACE_Dynamic_Service_Dependency (&one, "TAO_Codeset");

    /// This would ordinarily cause the dynamic services to get unloaded and their DLL's
    /// unmapped ...
  }

  // ... therefore the following code would crash miserably because it needs the
  // ~TAO_Codeset_Manager()'s code, which is in the (unlodaed) DLL's text segment ...
  delete codeset_manager;

  // ... unless of course we used the magic dependency statement, above - [1]
  delete pdep;

  return 0;
}







  int
run_main(int argc, ACE_TCHAR *argv[])
{
  testServiceDependency (argc, argv);
  testOpenDynamicServices (argc, argv);
  testORBInitializer_Registry(argc, argv);
  return 0;
}

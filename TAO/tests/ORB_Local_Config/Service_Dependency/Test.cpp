#include "tao/Codeset_Descriptor_Base.h"
#include "tao/Codeset_Manager_Factory_Base.h"
#include "tao/Codeset_Manager.h"
#include "tao/ORBInitializer_Registry_Adapter.h"

#include "ace/ARGV.h"
#include "ace/Dynamic_Service.h"
#include "ace/Dynamic_Service_Dependency.h"
#include "ace/Thread_Manager.h"
#include "ace/Intrusive_Auto_Ptr.h"

#include "Service_Configuration_Per_ORB.h"

#include <memory>

/// Returns the service configuration filename for the current character
/// encoding. The files all load the same five service objects.
ACE_TCHAR const *
file_Service_Config_Test ()
{
#if defined (ACE_USES_WCHAR)
  // When using full Unicode support, use the version of the Service
  // Configurator file appropriate to the platform.
  // For example, Windows Unicode uses UTF-16.
  //
  //          iconv(1) found on Linux, for example, can
  //          be used to convert between encodings.
  //
  //          Byte ordering is also an issue, so we should be
  //          generating this file on-the-fly from the UTF-8 encoded
  //          file by using functions like iconv(1) or iconv(3).
#  if defined (ACE_WIN32)
  static ACE_TCHAR const svc_conf[] =
    ACE_TEXT ("Service_Config_Test.UTF-16")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  else
  static ACE_TCHAR const svc_conf[] =
    ACE_TEXT ("Service_Config_Test.WCHAR_T")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  endif /* ACE_WIN32 */
#else
    // ASCII (UTF-8) encoded Service Configurator file.
  static ACE_TCHAR const svc_conf[] =
    ACE_TEXT ("Service_Config_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#endif  /* ACE_USES_WCHAR */

  return svc_conf;
}

/// Loads dynamic services into a local repository and checks the service count.
int
testOpenDynamicServices (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testOpenDynamicServices");

  ACE_ARGV new_argv;

  // Build the Service Configurator argument list for this test.
  if (new_argv.add (ACE_TEXT ("bogus")) == -1
      || new_argv.add (ACE_TEXT ("-f")) == -1
      || new_argv.add (file_Service_Config_Test ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Failed to build an argv\n")));
      return -1;
    }

  // The service-config guard keeps an intrusive reference to the current
  // gestalt, so it must be heap-allocated and owned by an intrusive pointer.
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> const daemon {
    new ACE_Service_Gestalt_Test (10)
  };

  if (daemon->open (new_argv.argc (),
                    new_argv.argv ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Unable to open service config\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\tdaemon.services_count () -> %B\n",
              daemon->services_count ()));

  if (daemon->services_count () != 5)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: Expected %d service, but found %B instead\n"),
                  5,
                  daemon->services_count ()));
      return -1;
    }

  // Since the loaded services start their own threads, wait until all
  // of them are done to avoid pulling the rug under their feet.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

/// Tests local loading and dependency registration for the
/// ORBInitializer_Registry service.
int
testORBInitializer_Registry (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testORBInitializer_Registry");

  // for global service registrations
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> const glob {
    new ACE_Service_Gestalt_Test ()
  };

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> const one {
    new ACE_Service_Gestalt_Test (10)
  };

  size_t const glob_size = glob->services_count ();
  size_t const loca_size = one->services_count ();

  // It is expected to be empty at this point since it is not using
  // the global repo
  if (loca_size != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected empty repository\n")));
      return -1;
    }

  // Lookup it up.
  TAO::ORBInitializer_Registry_Adapter *oir =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance (
      one.get (), "ORBInitializer_Registry");

#if defined (TAO_AS_STATIC_LIBS)
  if (oir == nullptr)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find statically linked ORBInitializer_Registry\n")));
      return -1;
    }
#else
  if (oir != nullptr)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Not expected to find ORBInitializer_Registry\n")));
      return -1;
    }
#endif
#if !defined (TAO_AS_STATIC_LIBS)
      // In case we build shared, try to load the PI Client library, in a
      // static build we just can't do this, so don't try it, lower layers
      // output an error then.
  if (oir == nullptr)
    {
      int const result = one->process_directive (
        ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("ORBInitializer_Registry",
                                                 "TAO_PI",
                                                 TAO_VERSION,
                                                 "_make_ORBInitializer_Registry",
                                                 ""));
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to load ORBInitializer_Registry\n")),
                          -1);
      oir =
        ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
        (one.get (), "ORBInitializer_Registry");
    }
  if (oir == nullptr)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find ORBInitializer_Registry\n")));
      return -1;
    }
#endif

  if (glob_size != glob->services_count ())
    {
      ACE_ERROR ((LM_ERROR,
                  "Expected %B local static service registrations, found %B\n",
                  glob_size,
                  glob->services_count ()));
      return -1;
    }

  // The local repository must also have acquired the static services
  // registered within the dynamic service we just loaded. As of this
  // writing, loading ORBInitializer_Registry causes the registration of
  // four other (static) services. The PolicyFactory_Loader,
  // ClientRequestInterceptor_Adapter_Factory and PICurrent_Loader are
  // registered explicitly, while CodecFactory_Loader is registered indirectly.

  if (loca_size == one->services_count ())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find additional services present\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) try global dynamic service"
              " on ORBInitializer_Registry ...\n"));

  // Try to instantiate the dynamic service from the global repository ...
  TAO::ORBInitializer_Registry_Adapter * const oir1 =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance (
      "ORBInitializer_Registry");

  // Right! It should not have been global.
  if (oir1 != nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unexpected to find ORBInitializer_Registry\n")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) try local dynamic service"
              " on ORBInitializer_Registry ...\n"));

  // Try to instantiate the dynamic service from the local repository ...
  TAO::ORBInitializer_Registry_Adapter * const oir2 =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance (
      one.get (), "ORBInitializer_Registry");

  // Right! That's local.
  if (oir2 == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Expected to find ORBInitializer_Registry locally\n")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Explicitly initialize ORBInitializer_Registry"
              " (although Dynamic_Service already did it) ...\n"));

  // ... initialize, but note that without the guard there is nothing to tell
  // the service which gestalt must be used. If init() does static service
  // registrations, those may end up in the wrong (global) gestalt and will
  // be in memory, which may not be mapped by finalization time!
  {
    ACE_Service_Config_Guard guard (one.get ());
    if (oir2->init (0, nullptr) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to initialize ORBInitializer_Registry\n")),
                        -1);
  }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Try dynamic_service on a "
              "dependent static service (CodecFactory_Loader) ...\n"));

  // ... and also one of the dependent static services
  if (ACE_Dynamic_Service<ACE_Service_Object>::instance
      (one.get (), "CodecFactory_Loader") == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: Expected to find CodecFactory_Loader locally\n")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Explicitly dynamic_service PolicyFactory_Loader"
              " (although ORBInitializer_Registry already did it) ...\n"));

  int const policy_factory_result = one->process_directive (
    ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("PolicyFactory_Loader",
                                             "TAO_PI",
                                             TAO_VERSION,
                                             "_make_TAO_PolicyFactory_Loader",
                                             ""));
  if (policy_factory_result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to load PolicyFactory_Loader\n")),
                      -1);

  if (ACE_Dynamic_Service<ACE_Service_Object>::instance
      (one.get (), "PolicyFactory_Loader") == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: Expected to find PolicyFactory_Loader locally\n")),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) We're done testing.\n"));
  return 0;
}

/// Tests lifetime dependencies between a service object and its dynamic
/// library.
int
testServiceDependency (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testServiceDependency");

  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_DLL) == %B\n", sizeof (ACE_DLL)));
  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_Dynamic_Service_Dependency) == %B\n",
              sizeof (ACE_Dynamic_Service_Dependency)));

  std::unique_ptr<TAO_Codeset_Manager> codeset_manager {};
  std::unique_ptr<ACE_Dynamic_Service_Dependency> dependency {};

  {
    // Start a block to limit the lifespan of a gestalt
    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> const one {
      new ACE_Service_Gestalt_Test (10)
    };

    int const result = one->process_directive (
      ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("TAO_Codeset",
                                               "TAO_Codeset",
                                               TAO_VERSION,
                                               "_make_TAO_Codeset_Manager_Factory",
                                               ""));
    if (result != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Expected to register TAO_Codeset successfuly\n")),
                        -1);

    TAO_Codeset_Manager_Factory_Base * const factory =
      ACE_Dynamic_Service<TAO_Codeset_Manager_Factory_Base>::instance (
        one.get (), "TAO_Codeset");

    if (factory == nullptr)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Expected to obtain an instance successfuly\n")),
                        -1);

    codeset_manager.reset (factory->create ());
    if (!codeset_manager)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Factory failed to create the codeset manager\n")),
                        -1);

    ACE_DEBUG ((LM_DEBUG, "Creating dependency ...\n"));

    // [1]
    //
    // Stating that a thing depends on that dynamic service. Why?
    // Read on ...

    dependency = std::make_unique<ACE_Dynamic_Service_Dependency> (
      one.get (), ACE_TEXT ("TAO_Codeset"));

    // This would ordinarily cause the dynamic services to get
    // unloaded and their DLL's unmapped ...
  }

  // ... therefore the following code would crash miserably because it
  // needs the ~TAO_Codeset_Manager()'s code, which is in the
  // (unlodaed) DLL's text segment ...
  codeset_manager.reset ();

  // ... unless of course we used the magic dependency statement,
  // above - [1]
  dependency.reset ();

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (testServiceDependency (argc, argv) != 0
      || testOpenDynamicServices (argc, argv) != 0)
    return -1;

  return testORBInitializer_Registry (argc, argv);
}

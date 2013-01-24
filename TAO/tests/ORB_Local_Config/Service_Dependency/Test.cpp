// $Id$

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

// Return the name of the service configuration file, based on the current ACE
// support for wide characters and unicode
/// Borrowing this from the $ACE_ROOT/test/Service_Config_Test
/// The various config files have the same effect of loading 5
/// new service objects.

const ACE_TCHAR*
file_Service_Config_Test ()
{
#if defined (ACE_USES_WCHAR)
  // When using full Unicode support, use the version of the Service
  // Configurator file appropriate to the platform.
  // For example, Windows Unicode uses UTF-16.
  //
  //          iconv(1) found on Linux and Solaris, for example, can
  //          be used to convert between encodings.
  //
  //          Byte ordering is also an issue, so we should be
  //          generating this file on-the-fly from the UTF-8 encoded
  //          file by using functions like iconv(1) or iconv(3).
#  if defined (ACE_WIN32)
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.UTF-16")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  else
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.WCHAR_T")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  endif /* ACE_WIN32 */
#else
    // ASCII (UTF-8) encoded Service Configurator file.
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#endif  /* ACE_USES_WCHAR */

  return svc_conf;
}


// @brief Loading dynamic services in a local repository

int
testOpenDynamicServices (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testOpenDynamicServices");

  ACE_ARGV new_argv;

  // Process the Service Configurator directives in this test's
  if (false == (new_argv.add (ACE_TEXT ("bogus")) != -1
                && new_argv.add (ACE_TEXT ("-f")) != -1
                && new_argv.add (file_Service_Config_Test ()) != -1))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Failed to build an argv\n")));
      return -1;
    }

  // We need this scope to make sure that the destructor for the
  // <ACE_Service_Config> gets called.
  ACE_Service_Gestalt_Test daemon(10);

  if (daemon.open (new_argv.argc (),
                   new_argv.argv ()) == -1 && errno == ENOENT)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Unable to open service config\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\tdaemon.services_count () -> %d\n",
              daemon.services_count ()));

  if (5 != daemon.services_count ())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Expected %d service, but found %d instead\n"), 5, daemon.services_count ()));
      return -1;
    }

  // Since the loaded services start their own threads, wait until all
  // of them are done to avoid pulling the rug under their feet.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}





// @brief Try loading the ORBInitializer_Registry locally

int
testORBInitializer_Registry (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testORBInitializer_Registry");

  // for global service registrations
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> glob (new ACE_Service_Gestalt_Test ());

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> one (new ACE_Service_Gestalt_Test (10));

  size_t glob_size = glob->services_count ();
  size_t loca_size = one->services_count ();

  // It is expected to be empty at this point since it is not using
  // the global repo
  if (loca_size != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected empty repository\n")));
      return -1;
    }

  // Lookup it up.
  TAO::ORBInitializer_Registry_Adapter* oir =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
    (one.get (), "ORBInitializer_Registry");

#if defined (TAO_AS_STATIC_LIBS)
  if ((oir == 0))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find staticaly linked ORBInitializer_Registry\n")));
      return -1;
    }
#else
  if (oir != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Not expected to find ORBInitializer_Registry\n")));
      return -1;
    }
#endif


#if !defined (TAO_AS_STATIC_LIBS)
      // In case we build shared, try to load the PI Client library, in a
      // static build we just can't do this, so don't try it, lower layers
      // output an error then.
  if (oir == 0)
    {
      one->process_directive (
        ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("ORBInitializer_Registry",
                                      "TAO_PI",
                                      TAO_VERSION,
                                      "_make_ORBInitializer_Registry",
                                      ""));
      oir =
        ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
        (one.get (), "ORBInitializer_Registry");
    }
  if (oir == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find ORBInitializer_Registry\n")));
      return -1;
    }
#endif


  if (glob_size != glob->services_count ())
    {
      ACE_ERROR ((LM_ERROR,
              "Expected %d local static service registrations, found %d\n",
              glob_size,
              glob->services_count ()));
      return -1;
    }

  // The local repository must have asquired also the static services
  // registered within the dynamic service we just loaded. As of this
  // writing, loading ORBInitializer_Registry causes the registration of
  // four other (static) services. The PolicyFactory_Loader,
  // ClientRequestInterceptor_Adapter_Factory and PICurrent_Loader are
  // registred explicitely, while CodecFactory_Loader - indirectly.

  if (loca_size == one->services_count ())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find additional services present\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) try global dynamic service"
              " on ORBInitializer_Registry ...\n"));

  // Try to instantiate the dynamic service from the global repository ...
  TAO::ORBInitializer_Registry_Adapter* oir1 =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
      ("ORBInitializer_Registry");


// Right! It should not have been global.
  if (oir1 != 0)
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT ("Unexpected to find ORBInitializer_Registry\n")),
                     -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) try local dynamic service"
              " on ORBInitializer_Registry ...\n"));

  // Try to instantiate the dynamic service from the local repository ...
  TAO::ORBInitializer_Registry_Adapter* oir2 =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
    (one.get (), "ORBInitializer_Registry");

  // Right! That's local.
  if (oir2 == 0)
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT ("Expected to find ORBInitializer_Registry locally\n")),
                     -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Explicitely initialize ORBInitializer_Registry"
              " (although Dynamic_Service already did it) ...\n"));

  // ... initialize, but note that without the guard there is nothing to tell
  // the service which gestalt must be used. If init() does static service
  // registrations, those may end up in the wrong (global) gestalt and will
  // be in memory, which may not be mapped by finalization time!
  {
    ACE_Service_Config_Guard guard (one.get ());
    oir2->init (0,0);
  }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Try dynamic_service on a "
              "dependent static service (CodecFactory_Loader) ...\n"));

  // ... and also one of the dependent static services
  if (0 == ACE_Dynamic_Service <ACE_Service_Object>::instance
      (one.get (), "CodecFactory_Loader"))
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT ("Error: Expected to find CodecFactory_Loader locally\n")),
                     -1);


  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Explicitely dynamic_service PolicyFactory_Loader"
              " (although ORBInitializer_Registry already did it) ...\n"));

  one->process_directive
    (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("PolicyFactory_Loader",
                                   "TAO_PI",
                                   TAO_VERSION,
                                   "_make_TAO_PolicyFactory_Loader",
                                   ""));


  if (0 == ACE_Dynamic_Service <ACE_Service_Object>::instance
      (one.get (), "PolicyFactory_Loader"))
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT ("Error: Expected to find PolicyFactory_Loader locally\n")),
                     -1);


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
  ACE_DEBUG ((LM_DEBUG, "sizeof (ACE_Dynamic_Service_Dependency) == %d\n",
              sizeof (ACE_Dynamic_Service_Dependency)));

  TAO_Codeset_Manager *codeset_manager = 0;
  ACE_Dynamic_Service_Dependency *pdep = 0;

  {
    // Start a block to limit the lifespan of a gestalt
    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt_Test> one (new ACE_Service_Gestalt_Test (10));

    int result = one->process_directive
      (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("TAO_Codeset",
                                     "TAO_Codeset",
                                     TAO_VERSION,
                                     "_make_TAO_Codeset_Manager_Factory",
                                     ""));
    if (result != 0)
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("Expected to register TAO_Codeset successfuly\n")),
                       -1);

    TAO_Codeset_Manager_Factory_Base *factory =
      ACE_Dynamic_Service<TAO_Codeset_Manager_Factory_Base>::instance
      (one.get (), "TAO_Codeset");

    if (factory == 0)
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("Expected to obtain an instance successfuly\n")),
                       -1);

    codeset_manager = factory->create ();
    if (codeset_manager == 0)

    ACE_DEBUG ((LM_DEBUG, "Creating dependency ...\n"));

    // [1]
    //
    // Stating that a thing depends on that dynamic service. Why?
    // Read on ...

    pdep = new ACE_Dynamic_Service_Dependency (one.get (), ACE_TEXT ("TAO_Codeset"));

    // This would ordinarily cause the dynamic services to get
    // unloaded and their DLL's unmapped ...
  }

  // ... therefore the following code would crash miserably because it
  // needs the ~TAO_Codeset_Manager()'s code, which is in the
  // (unlodaed) DLL's text segment ...
  delete codeset_manager;

  // ... unless of course we used the magic dependency statement,
  // above - [1]
  delete pdep;

  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return
    testServiceDependency (argc, argv)
    && testOpenDynamicServices (argc, argv)
    && testORBInitializer_Registry(argc, argv);
}

// $Id$

#include "Service.h"
#include "ace/Service_Repository.h"
#include "ace/ARGV.h"

ACE_RCSID(EC_Examples, Service, "$Id$");

extern "C"
int
createService (int argc, char* argv[])
{

  if (ACE_Service_Config::process_directive ("dynamic EC_Factory Service_Object * TAO_RTEvent:_make_TAO_EC_Default_Factory() \"-ECDispatching reactive -ECFiltering basic -ECSupplierFiltering per-supplier -ECProxyConsumerLock thread -ECProxySupplierLock thread -ECConsumerControl reactive -ECSupplierControl reactive -ECConsumerControlPeriod 50000 -ECSupplierControlPeriod 50000\"") != 0)
    {

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ERROR: Client unable to process the "
                         "Service Configurator directive"),
                        -1);
    }
  Service service;
  return service.run (argc, argv);
}

Service::Service()
{

}

int
Service::run (int argc, char* argv[])
{
//   int register_with_object_manager = 0;
//   if (TAO_Singleton_Manager::instance ()->init (
//         register_with_object_manager) == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "Service::init -- ORB pre-initialization "
//                        "failed.\n"),
//                       -1);  // No exceptions available yet, so return
//                             // an error

  const ACE_Service_Type* service;
  const ACE_TCHAR* factory = "EC_Factory";

  if (ACE_Service_Repository::instance()->find (factory, &service) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Unable to locate EC_Factory"), -1);

  if (service->type()->init (argc, argv) != 0)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Prepend a "dummy" program name argument to the Service
      // Configurator argument vector.
      int new_argc = argc + 1;

      CORBA::StringSeq new_argv (new_argc);
      new_argv.length (new_argc);

      // Prevent the ORB from opening the Service Configurator file
      // again since the Service Configurator file is already in the
      // process of being opened.
      new_argv[0] = CORBA::string_dup ("dummy");

      // Copy the remaining arguments into the new argument vector.
      for (int i = new_argc - argc, j = 0;
           j < argc;
           ++i, ++j)
        new_argv[i] = CORBA::string_dup (argv[j]);

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (new_argc,
                                            new_argv.get_buffer (),
                                            "Service"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_ = orb.in();

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Naming_Client naming_client;

      // Initialization of the naming service.
      if (naming_client.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name ec_name (1);
      ec_name.length (1);
      ec_name[0].id = CORBA::string_dup ("EventChannel");
      ACE_TRY_CHECK;

      naming_client->bind (ec_name, event_channel.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int done = 0;

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.

      while (!done)
        {
          CORBA::Boolean pending =
            orb->work_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (pending)
            {
              orb->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_TRY
            {
              CORBA::Object_var ec_obj =
                naming_client->resolve (ec_name
                                    ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_CATCH (CosNaming::NamingContext::NotFound, ex)
            {
              ACE_DEBUG ((LM_DEBUG, "EventChannel has been destroyed\n"));
              done = 1;
              break;
            }
          ACE_ENDTRY;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
//   if (TAO_Singleton_Manager::instance ()->fini () == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "Service::fini -- ORB pre-termination failed."),
//                       -1);
  return 0;

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

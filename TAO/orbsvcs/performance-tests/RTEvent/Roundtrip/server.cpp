// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTEC_Initializer.h"
#include "RTServer_Setup.h"

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(TAO_PERF_RTEC_Roundtrip, server, "$Id$")

const char *ior_output_file = "test.ior";
int use_rt_corba = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:r");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'r':
        use_rt_corba = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-r (use RT-CORBA) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  /// Move the test to the real-time class if it is possible.
  RT_Class rt_class;

  ACE_TRY_NEW_ENV
    {
      ORB_Holder orb (argc, argv, ""
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      RTServer_Setup rtserver_setup (use_rt_corba,
                                     orb,
                                     rt_class
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA"
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var ec_poa (root_poa);
      if (use_rt_corba != 0)
        {
          ec_poa = rtserver_setup.poa ();
        }
      Servant_var<TAO_EC_Event_Channel> ec_impl (
              RTEC_Initializer::create (ec_poa.in (),
                                        ec_poa.in (),
                                        rtserver_setup.rtcorba_setup ()
                                        ACE_ENV_ARG_PARAMETER)
              );
      ACE_TRY_CHECK;

      ec_impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var ec_id =
        ec_poa->activate_object (ec_impl.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var ec_object =
        ec_poa->id_to_reference (ec_id.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_object.in ()
                                                      ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the ior to the <ior_output_file>
      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      do {
        ACE_Time_Value tv (1, 0);
        orb->run (tv ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      } while (ec_impl->destroyed () == 0);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ServerORBInitializer.h"
#include "ServerRequest_Interceptor.h"
#include "tao/PI/PI.h"
#include "tao/ORBInitializer_Registry.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file { ACE_TEXT ("test.ior") };
};

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

CORBA::Object_ptr
make_iogr (TAO_IOP::TAO_IOR_Manipulation_ptr iorm,
           char const *domain_id,
           CORBA::ULongLong group_id,
           CORBA::ULong group_version,
           CORBA::Object_ptr ref)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;
  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (1);
  iors.length (1);
  iors [0] = CORBA::Object::_duplicate (ref);

  CORBA::Object_var new_ref =
    iorm->merge_iors (iors);

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = static_cast<CORBA::Octet> (1);
  ft_tag_component.component_version.minor = static_cast<CORBA::Octet> (0);

  // Domain id
  //const char *id = "iogr_testing";
  ft_tag_component.group_domain_id = domain_id;

  // Object group id
  ft_tag_component.object_group_id = group_id;

  // Version
  ft_tag_component.object_group_ref_version = group_version;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean const retval = iorm->set_property (&iogr_prop, new_ref.in ());

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      iorm->set_primary (&iogr_prop, new_ref.in (), new_ref.in ());
    }

  return new_ref._retn ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      PortableServer::POA_var my_poa =
        root_poa->create_POA ("my_poa",
                              poa_manager.in (),
                              policies);

      // Creation of the new POA is over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (), Test::Hello::_nil ()),
                      1);
      PortableServer::ServantBase_var owner (hello_impl);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("server_id");

      my_poa->activate_object_with_id (server_id.in (),
                                       hello_impl);

      CORBA::Object_var hello =
        my_poa->id_to_reference (server_id.in ());

      CORBA::String_var ior =
        orb->object_to_string (hello.in ());

      // Get a ref to the IORManipulation object
      CORBA::Object_var iorm_object =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

      // Narrow
      TAO_IOP::TAO_IOR_Manipulation_var iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

      CORBA::Object_var server = orb->string_to_object (ior.in ());

      CORBA::Object_var iogr = make_iogr (iorm.in (), "Domain_1", 1, 1, server.in ());

      CORBA::String_var iorgr_string =
        orb->object_to_string (iogr.in ());

      {
        File_Ptr output_file (ACE_OS::fopen (options.ior_output_file.c_str (), "w"),
                              ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", iorgr_string.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

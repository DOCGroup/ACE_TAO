// $Id: server.cpp,v 1.1 2006/07/26 12:54:06 sm Exp $

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "ServerORBInitializer2.h"
#include "ServerRequest_Interceptor2.h"
#include "tao/PI/PI.h"
#include "tao/ORBInitializer_Registry.h"
#include "Hello.h"

ACE_RCSID (Hello,
           server,
           "$Id: server.cpp,v 1.1 2006/07/26 12:54:06 sm Exp $")

const char *ior_output_file = "ior.ior";
const char *iogr_output_file = "iogr.ior";
TAO_IOP::TAO_IOR_Manipulation_var iorm = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'p':
        iogr_output_file = get_opts.opt_arg ();
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
  // Indicates sucessful parsing of the command line
  return 0;
}

CORBA::Object_ptr
make_iogr (const char* domain_id, CORBA::ULongLong group_id, CORBA::ULong group_version, CORBA::Object_ptr ref ACE_ENV_ARG_DECL)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;
  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (1);
  iors.length(1);
  iors [0] = CORBA::Object::_duplicate (ref);

  CORBA::Object_var new_ref =
    iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = (CORBA::Octet) 1;
  ft_tag_component.component_version.minor = (CORBA::Octet) 0;

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
  CORBA::Boolean retval = iorm->set_property (&iogr_prop,
                                              new_ref.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  new_ref.in (),
                                  new_ref.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return new_ref._retn ();
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      Server_ORBInitializer2 *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer2,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var my_poa =
        root_poa->create_POA ("my_poa",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of the new POA is over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }


      if (parse_args (argc, argv) != 0)
        return 1;

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (), Test::Hello::_nil ()),
                      1);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("server_id");

      my_poa->activate_object_with_id (server_id.in (),
                                       hello_impl
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var hello =
        my_poa->id_to_reference (server_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (hello.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);


      // Get a ref to the IORManipulation object
      CORBA::Object_var IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow
      iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var iogr = make_iogr ("Domain_1", 1, 1, orb->string_to_object (ior.in ())  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var iorgr_string =
        orb->object_to_string (iogr.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the IOR to the <iogr_output_file>
      output_file = ACE_OS::fopen (iogr_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           iogr_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", iorgr_string.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"

#if TAO_HAS_INTERCEPTORS

#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"
#include "tao/IORManipulation/IORManipulation.h"
#include "tao/ORBInitializer_Registry.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

const ACE_TCHAR *ior_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o IOR\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

void
add_ft_prop (CORBA::ORB_ptr o,
             CORBA::Object_ptr obj1,
             CORBA::Object_ptr obj2)
{
  // Get an object reference for the ORBs IORManipultion object!
  CORBA::Object_var IORM =
    o->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                   0);

  TAO_IOP::TAO_IOR_Manipulation_var iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in());

  FT::TagFTGroupTaggedComponent ft_tag_component;

  // Property values
  // Major and Minor revision numbers
  ft_tag_component.component_version.major = (CORBA::Octet) 1;
  ft_tag_component.component_version.minor = (CORBA::Octet) 0;

  // Domain id
  const char *id = "version_testing";
  ft_tag_component.group_domain_id = id;

  // Object group id
  ft_tag_component.object_group_id =
    (CORBA::ULongLong) 10;

  // Version
  ft_tag_component.object_group_ref_version =
    (CORBA::ULong) 1;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property for object 1
  CORBA::Boolean retval = iorm->set_property (&iogr_prop,
                                              obj1);

  if (retval != 0)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  obj1,
                                  obj1);
    }


  // Set the property for object 2
  // Change the version. That is the point of this test
  // Version
  ft_tag_component.object_group_ref_version = (CORBA::ULong) 5;

  retval = iorm->set_property (&iogr_prop,
                                              obj2);

  if (retval != 0)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  obj2,
                                  obj2);
    }

  return;
}

void test_colocal (ForwardRequestTest::test_ptr server)
{
  CORBA::ULong number = 0;
  for (int i = 1; i <= 25; ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  "CLIENT: Issuing colocal request %d.\n",
                  i));
      CORBA::ULong retval =
         server->number ();
      if (retval != 1 && retval != 317)
        {
          // Not a valid return value, forwarding went wrong and
          // retval is in udefined state
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: colocal invocation returned invalid"
                      " value, location-forwarding must have failed.\n"));
          ACE_OS::abort ();
        }

      number += retval;


      ACE_DEBUG ((LM_INFO,
                  "CLIENT: Number %d .\n",
                  number));
    }

  if (number < 250)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Did not forward to new location\n"));
      ACE_OS::abort ();
    }
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::PolicyList policies;  // Empty policy list.

      // Servant 1
      test_i servant1 (1, orb.in ());
      test_i servant2 (317, orb.in ());

      PortableServer::POA_var first_poa =
        root_poa->create_POA ("first POA",
                              poa_manager.in (),
                              policies);



      PortableServer::ObjectId_var oid1 =
        first_poa->activate_object (&servant1);

      PortableServer::ObjectId_var oid2 =
        first_poa->activate_object (&servant2);

      CORBA::Object_var obj1 =
        first_poa->servant_to_reference (&servant1);

      CORBA::Object_var obj2 =
        first_poa->servant_to_reference (&servant2);

      (void) add_ft_prop (orb.in (),
                          obj1.in (),
                          obj2.in ());

      CORBA::String_var ior =
        orb->object_to_string (obj1.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "ForwardRequestTest::test servant 1: <%s>\n",
                  ior.in ()));

      poa_manager->activate ();

      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var
        server_interceptor = temp_initializer->server_interceptor ();

      ForwardRequestTest::ServerRequestInterceptor_var interceptor =
        ForwardRequestTest::ServerRequestInterceptor::_narrow (
           server_interceptor.in ());

      if (CORBA::is_nil (interceptor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Could not obtain reference to "
                           "server request interceptor.\n"),
                          -1);

      interceptor->forward_references (obj1.in (),
                                       obj2.in ());

      // Run co-local test
        {
          ForwardRequestTest::test_var server =
            ForwardRequestTest::test::_narrow (obj1.in ());
          test_colocal (server.in());

          // Reset interceptor for remote tests
          interceptor->reset ();
        }

      // Write each IOR to a file.

      // IOR 1
      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Run the ORB event loop.
      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}

#else

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  return 0;
}

#endif /* TAO_HAS_INTERCEPTORS */

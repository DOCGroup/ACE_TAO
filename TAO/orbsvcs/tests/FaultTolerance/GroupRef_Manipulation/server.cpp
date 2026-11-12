// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include <cstdio>
#include <cstdlib>
#include <memory>

#if TAO_HAS_INTERCEPTORS

#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"
#include "tao/IORManipulation/IORManipulation.h"
#include "tao/ORBInitializer_Registry.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

struct Server_Options
{
  ACE_TString ior_file {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s -o IOR\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

static bool
write_ior (ACE_TCHAR const *file_name, char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (file_name, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Cannot open output file for writing IOR\n")));
      return false;
    }

  if (std::fprintf (output_file.get (), "%s", ior) < 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")));
      return false;
    }

  return true;
}

static void
add_ft_prop (CORBA::ORB_ptr orb,
             CORBA::Object_ptr obj1,
             CORBA::Object_ptr obj2)
{
  CORBA::Object_var const iorm_object =
    orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION, nullptr);

  TAO_IOP::TAO_IOR_Manipulation_var const iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

  FT::TagFTGroupTaggedComponent ft_tag_component;
  ft_tag_component.component_version.major = 1;
  ft_tag_component.component_version.minor = 0;
  ft_tag_component.group_domain_id = "version_testing";
  ft_tag_component.object_group_id = 10;
  ft_tag_component.object_group_ref_version = 1;

  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  if (iorm->set_property (&iogr_prop, obj1))
    {
      iorm->set_primary (&iogr_prop, obj1, obj1);
    }

  ft_tag_component.object_group_ref_version = 5;

  if (iorm->set_property (&iogr_prop, obj2))
    {
      iorm->set_primary (&iogr_prop, obj2, obj2);
    }
}

static void
test_colocal (ForwardRequestTest::test_ptr server)
{
  CORBA::ULong number {};
  for (CORBA::ULong i {1}; i <= 25; ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  "CLIENT: Issuing colocal request " ACE_UINT32_FORMAT_SPECIFIER ".\n",
                  i));
      CORBA::ULong const retval =
        server->number ();
      if (retval != 1 && retval != 317)
        {
          // Not a valid return value, forwarding went wrong and
          // retval is in udefined state
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: colocal invocation returned invalid"
                      " value, location-forwarding must have failed.\n"));
          std::abort ();
        }

      number += retval;
      ACE_DEBUG ((LM_INFO,
                  "CLIENT: Number " ACE_UINT32_FORMAT_SPECIFIER ".\n",
                  number));
    }

  if (number < 250)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Did not forward to new location\n"));
      std::abort ();
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options {};
      Server_ORBInitializer *initializer {};
      ACE_NEW_RETURN (initializer, Server_ORBInitializer, -1);
      PortableInterceptor::ORBInitializer_var orb_initializer =
        initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      if (parse_args (argc, argv, options) != 0 || options.ior_file.empty ())
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList const policies {};

      test_i servant1 (1, orb.in ());
      test_i servant2 (317, orb.in ());

      PortableServer::POA_var const first_poa =
        root_poa->create_POA ("first POA",
                              poa_manager.in (),
                              policies);

      PortableServer::ObjectId_var const oid1 =
        first_poa->activate_object (&servant1);

      PortableServer::ObjectId_var const oid2 =
        first_poa->activate_object (&servant2);

      CORBA::Object_var const obj1 =
        first_poa->servant_to_reference (&servant1);

      CORBA::Object_var const obj2 =
        first_poa->servant_to_reference (&servant2);

      add_ft_prop (orb.in (),
                   obj1.in (),
                   obj2.in ());

      CORBA::String_var const ior =
        orb->object_to_string (obj1.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "ForwardRequestTest::test servant 1: <%s>\n",
                  ior.in ()));

      poa_manager->activate ();

      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var const
        server_interceptor = initializer->server_interceptor ();

      ForwardRequestTest::ServerRequestInterceptor_var const interceptor =
        ForwardRequestTest::ServerRequestInterceptor::_narrow (
           server_interceptor.in ());

      if (CORBA::is_nil (interceptor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Could not obtain reference to "
                           "server request interceptor.\n"),
                          -1);

      interceptor->forward_references (obj1.in (),
                                       obj2.in ());

      // Exercise the collocated forwarding path before remote requests.
      ForwardRequestTest::test_var server =
        ForwardRequestTest::test::_narrow (obj1.in ());
      test_colocal (server.in ());

      interceptor->reset ();

      if (!write_ior (options.ior_file.c_str (), ior.in ()))
        return 1;

      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}

#else

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}

#endif /* TAO_HAS_INTERCEPTORS */

// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "test_i.h"
#include "ace/OS_NS_stdlib.h"

#if TAO_HAS_INTERCEPTORS

#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"
#include "tao/IORManipulation/IORManipulation.h"
#include "tao/ORBInitializer_Registry.h"

constexpr CORBA::ULong passes_before_forward = 6;

static int
parse_args (int argc, ACE_TCHAR *argv[], bool &direct_collocation)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("d"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        direct_collocation = true;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s -d\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

static void
test_colocal (Collocated_ForwardRequestTest::test_ptr const server)
{
  CORBA::ULong number {};
  for (int i = 1; i <= 10; ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  "CLIENT: Issuing colocal request %d.\n",
                  i));
      CORBA::ULong const retval = server->collocated_call ();
      number += retval;
    }

  if (number != 15)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Did not forward to new location\n"));
      ACE_OS::abort ();
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool direct_collocation {};
  try
    {
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer (passes_before_forward),
                      -1);  // No exceptions yet!
      // The initializer var takes ownership of the newly allocated object.
      PortableInterceptor::ORBInitializer_var const orb_initializer {temp_initializer};

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv, "Server ORB")};

      if (::parse_args (argc, argv, direct_collocation) != 0)
        return -1;

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      test_i servant1 (1);
      test_i servant2 (2);

      PortableServer::ObjectId_var const oid1 {root_poa->activate_object (&servant1)};

      PortableServer::ObjectId_var const oid2 {root_poa->activate_object (&servant2)};

      CORBA::Object_var const obj1 {root_poa->servant_to_reference (&servant1)};

      CORBA::Object_var const obj2 {root_poa->servant_to_reference (&servant2)};

      poa_manager->activate ();

      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var const server_interceptor {
        temp_initializer->server_interceptor ()};

      Collocated_ForwardRequestTest::ServerRequestInterceptor_var const interceptor {
        Collocated_ForwardRequestTest::ServerRequestInterceptor::_narrow (
          server_interceptor.in ())};

      if (CORBA::is_nil (interceptor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Could not obtain reference to "
                           "server request interceptor.\n"),
                          -1);

      if (direct_collocation)
        {
          servant1.forward (obj2.in (), passes_before_forward);
        }
      else
        {
          interceptor->forward (obj2.in ());
        }

      // Run co-local test
      {
        Collocated_ForwardRequestTest::test_var const server =
          Collocated_ForwardRequestTest::test::_narrow (obj1.in ());
        test_colocal (server.in ());
      }

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const &ex)
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

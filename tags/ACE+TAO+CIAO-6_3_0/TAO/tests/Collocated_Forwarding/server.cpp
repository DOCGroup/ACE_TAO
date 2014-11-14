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

const CORBA::ULong passes_before_forward = 6;

bool direct_collocation = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        direct_collocation = true;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-d\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

void test_colocal (Collocated_ForwardRequestTest::test_ptr server)
{
  CORBA::ULong number = 0;
  for (int i = 1; i <= 10; ++i)
    {
      ACE_DEBUG ((LM_INFO,
                  "CLIENT: Issuing colocal request %d.\n",
                  i));
      CORBA::ULong retval =
        server->collocated_call ();

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer (passes_before_forward),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      if (::parse_args (argc, argv) != 0)
        return -1;

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

      test_i servant1 (1, direct_collocation, orb.in ());
      test_i servant2 (2, direct_collocation, orb.in ());

      PortableServer::ObjectId_var oid1 =
        root_poa->activate_object (&servant1);

      PortableServer::ObjectId_var oid2 =
        root_poa->activate_object (&servant2);

      CORBA::Object_var obj1 =
        root_poa->servant_to_reference (&servant1);

      CORBA::Object_var obj2 =
        root_poa->servant_to_reference (&servant2);

      poa_manager->activate ();

      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var
        server_interceptor = temp_initializer->server_interceptor ();

      Collocated_ForwardRequestTest::ServerRequestInterceptor_var interceptor =
        Collocated_ForwardRequestTest::ServerRequestInterceptor::_narrow (
          server_interceptor.in ());

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
        Collocated_ForwardRequestTest::test_var server =
          Collocated_ForwardRequestTest::test::_narrow (obj1.in ());
        test_colocal (server.in ());
      }

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

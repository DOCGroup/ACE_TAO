// -*- C++ -*-
#include "ace/Get_Opt.h"

#include "test_i.h"
#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

ACE_TCHAR const *ior1_file {};
ACE_TCHAR const *ior2_file {};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc != 5)  // foo -o IOR_1 -o IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  int ior_count {1};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':

        if (ior_count == 1)
          {
            ior1_file = get_opts.opt_arg ();
            ++ior_count;
          }
        else if (ior_count == 2)
          {
            ior2_file = get_opts.opt_arg ();
            ++ior_count;
          }

        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o IOR_1 -o IOR_2\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
#if TAO_HAS_INTERCEPTORS == 1
      Server_ORBInitializer *temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var const orb_initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv, "Server ORB"));

      CORBA::Object_var const poa_object (orb->resolve_initial_references ("RootPOA"));

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa (PortableServer::POA::_narrow (poa_object.in ()));

      PortableServer::POAManager_var const poa_manager (root_poa->the_POAManager ());

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::PolicyList const policies;  // Empty policy list.

      // Servant 1
      test_i servant1 (1, orb.in ());

      PortableServer::POA_var const first_poa (
        root_poa->create_POA ("first POA", poa_manager.in (), policies));

      PortableServer::ObjectId_var const oid1 (first_poa->activate_object (&servant1));

      CORBA::Object_var const obj1 (first_poa->servant_to_reference (&servant1));

      CORBA::String_var const ior1 (orb->object_to_string (obj1.in ()));

      ACE_DEBUG ((LM_DEBUG,
                  "ForwardRequestTest::test servant 1: <%C>\n",
                  ior1.in ()));

      // Servant 2
      test_i servant2 (2, orb.in ());

      PortableServer::POA_var const second_poa (
        root_poa->create_POA ("second POA", poa_manager.in (), policies));

      PortableServer::ObjectId_var const oid2 (second_poa->activate_object (&servant2));

      CORBA::Object_var const obj2 (second_poa->servant_to_reference (&servant2));

      CORBA::String_var const ior2 (orb->object_to_string (obj2.in ()));

      ACE_DEBUG ((LM_DEBUG,
                  "ForwardRequestTest::test servant 2: <%C>\n",
                  ior2.in ()));

      poa_manager->activate ();

#if TAO_HAS_INTERCEPTORS == 1
      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var const server_interceptor (
        temp_initializer->server_interceptor ());

      ForwardRequestTest::ServerRequestInterceptor_var const interceptor (
        ForwardRequestTest::ServerRequestInterceptor::_narrow (server_interceptor.in ()));

      if (CORBA::is_nil (interceptor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Could not obtain reference to "
                           "server request interceptor.\n"),
                          -1);

      interceptor->forward_references (obj1.in (),
                                       obj2.in ());
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      // Write each IOR to a file.

      // IOR 1
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior1_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file <%s> for writing "
                             "IOR: %s",
                             ior1_file,
                             ior1.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior1.in ());
      }

      // IOR 2
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior2_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior2.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior2.in ());
      }

      // Run the ORB event loop.
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

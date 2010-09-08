// $Id$

#include "TestS.h"
#include "ace/Get_Opt.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"



const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Test failed - Not regression - unexpected nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (), hello.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      Test::Hello_var me =
        hello_impl->_this ();

      poa_manager->activate ();

      try
        {
          hello->throw_exception ();
        }
      catch (const Test::MyException& my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          ACE_DEBUG ((LM_DEBUG, "Client catches a MyException, as expected. No problem !\n"));
        }

      result = ! hello->call_me_back (me.in ());

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }

  if (result)
    {
      ACE_DEBUG ((LM_ERROR, "Error: REGRESSION identified!!!\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Test passed !!!\n"));
    }
  return result;
}

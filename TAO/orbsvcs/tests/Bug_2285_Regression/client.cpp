#include "TestS.h"
#include "ace/Get_Opt.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"

struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          options.ior = get_opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <ior>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result {};
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (options.ior);

      Test::Hello_var const hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Test failed - Not regression - Unexpected Nil "
                                       "Test::Hello reference <%s>\n"),
                             options.ior),
                            1);
        }

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Test failed - Not regression - unexpected nil RootPOA\n")),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in (), hello.in ()),
                      1);
      // ServantBase_var adopts the initial reference returned by ACE_NEW_RETURN.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      Test::Hello_var const me =
        hello_impl->_this ();

      poa_manager->activate ();

      try
        {
          hello->throw_exception ();
        }
      catch (Test::MyException const &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client catches a MyException, as expected. No problem !\n")));
        }

      result = !hello->call_me_back (me.in ());

      hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Test failed (Not regression) because unexpected exception caught:");
      return 1;
    }

  if (result)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Error: REGRESSION identified!!!\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test passed !!!\n")));
    }
  return result;
}

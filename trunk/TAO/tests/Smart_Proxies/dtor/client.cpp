//$Id$

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "testC.h"
#include "Smart_Proxy_Impl.h"

const char *ior = "file://test.ior";
bool dtor_called;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ior = ACE_OS::strdup (get_opts.opt_arg ());
      break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int main (int argc, char* argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                          argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj = orb->string_to_object (ior);

    // force a scope to see the destruction of the server object
    {
      Test_Smart_Factory *test_factory = 0;
      ACE_NEW_RETURN (test_factory,
                      Test_Smart_Factory,
                      -1);

      ACE_UNUSED_ARG (test_factory);

      Test_var server =
        Test::_narrow(obj.in());

      if (CORBA::is_nil (server.in())) {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil\n",
                           ior),
                          1);
      }

      server->hello(3);

#if (TAO_HAS_MINIMUM_CORBA == 0)
      // Testing the _non_existent function
      ACE_DEBUG ((LM_DEBUG, "Testing _non_existent()\n"));
      CORBA::Boolean ne =
        server->_non_existent();
      if (ne)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Not a Messenger object reference\n"),
                          1);
      else
        ACE_DEBUG ((LM_DEBUG,"Successfully called _non_existent()\n"));
#endif /* TAO_HAS_MINIMUM_CORBA */

      server->shutdown();

      // The following sleep is a hack to make sure the above oneway
      // request gets sent before we exit. Otherwise, at least on
      // Windows XP, the server may not even get the request.
      ACE_Time_Value tv (0, 100000);
      ACE_OS::sleep(tv);
    }
    // here we should get the smart proxy destructor printout
    if (!dtor_called) {
      ACE_ERROR_RETURN((LM_ERROR,
                        "The Smart proxy is not deleted\n"),1);
    }

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Client-side exception:");
    return 1;
  }

  return 0;
}

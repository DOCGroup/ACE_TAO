#include "TestC.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Transport.h"

#include "ace/Get_Opt.h"
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  // Indicates successful parsing of the command line
  return 0;
}

// This function does what TAO does in order to get a transport.
CORBA::Boolean
first_request_flag (CORBA::Object_ptr const obj)
{
  TAO_Stub *const stub = obj->_stubobj ();
  if (stub == nullptr)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "CORBA::Object has stub = 0\n"),
                        1);
    }

  TAO::Profile_Transport_Resolver resolver (obj, stub, true);

  resolver.resolve (nullptr);

  TAO_Transport *const transport = resolver.transport ();
  if (transport == nullptr)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Transport is 0\n"),
                        1);
    }

  return transport->first_request ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result {};
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (ior.c_str ());

      // No remote calls, first request flag is on.
      if (!first_request_flag (tmp.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Test FAILED at the beginning (why?)\n"));
          result = 1;
        }

      // This call must issue LocateRequest message.
      CORBA::PolicyList_var pl {};
      tmp->_validate_connection (pl.out ());

      // There was a LocateRequest call, first request flag
      // must be still on.
      if (!first_request_flag (tmp.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Test FAILED after LocateRequest (bug#3676 is not fixed)!!\n"));
          result = 1;
        }

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      CORBA::String_var const the_string = hello->get_string ();

      // Either _narrow or get_string issued a remote call,
      // first request flag is off.
      if (first_request_flag (tmp.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Test FAILED after normal request (why?)\n"));
          result = 1;
        }

      hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return result;
}

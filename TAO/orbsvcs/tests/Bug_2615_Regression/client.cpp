#include "TestS.h"
#include "ace/Get_Opt.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"

struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT ("file://ior.ior") };
  ACE_TCHAR const *iogr { ACE_TEXT ("file://iogr.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:l:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'l':
        options.iogr = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior> -l <iogr>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool result {};
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      // First perform the test with an IOR
      CORBA::Object_var const ior_object =
        orb->string_to_object (options.ior);

      Test::Hello_var const ior_hello =
        Test::Hello::_narrow (ior_object.in ());

      if (CORBA::is_nil (ior_hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n"),
                             options.ior),
                            1);
        }

      // Check this isn't generating exceptions for any other reason
      ior_hello->ping ();

      if (ior_hello->has_ft_request_service_context ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("ERROR - REGRESSION - Request on a plain IOR has a FT_REQUEST context.\n")));
          result = true;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Request made on a plain IOR has no FT_REQUEST service context. This is OK.\n")));
        }

      // Now repeat the test (for the converse result) with an IOGR
      CORBA::Object_var const iogr_object =
        orb->string_to_object (options.iogr);

      Test::Hello_var const iogr_hello =
        Test::Hello::_narrow (iogr_object.in ());

      if (CORBA::is_nil (iogr_hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n"),
                             options.iogr),
                            1);
        }

      // Check this isn't generating transients for any other reason
      iogr_hello->ping ();

      if (!iogr_hello->has_ft_request_service_context ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("ERROR - REGRESSION - Request made on an IOGR has no FT_REQUEST service context.\n")));
          result = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Request made on an IOGR has a FT_REQUEST service context. This is OK.\n")));
        }

      iogr_hello->shutdown ();

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

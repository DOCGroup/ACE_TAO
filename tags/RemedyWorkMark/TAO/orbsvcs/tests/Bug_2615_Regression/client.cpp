// $Id$

#include "TestS.h"
#include "ace/Get_Opt.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"



const ACE_TCHAR *ior = ACE_TEXT("file://ior.ior");
const ACE_TCHAR *iogr = ACE_TEXT("file://iogr.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'l':
        iogr = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-l <iogr> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
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

      // First perform the test with an IOR
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

      // Check this isn't generating exceptions for any other reason
      hello->ping ();

      if (hello->has_ft_request_service_context ())
        {
          ACE_DEBUG ((LM_ERROR, "ERROR - REGRESSION - Request made on a plain IOR has a FT_REQUEST service context.\n" ));
          result = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Request made on a plain IOR has no FT_REQUEST service context. This is OK.\n" ));
        }

      // Now repeat the test (for the converse result) with an IOGR
      tmp =
        orb->string_to_object(iogr);

      hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                             iogr),
                            1);
        }

      // Check this isn't generating transients for any other reason
      hello->ping ();

      if (! hello->has_ft_request_service_context ())
        {
          ACE_DEBUG ((LM_ERROR, "ERROR - REGRESSION - Request made on an IOGR has no FT_REQUEST service context.\n" ));
          result = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Request made on an IOGR has a FT_REQUEST service context. This is OK.\n" ));
        }

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

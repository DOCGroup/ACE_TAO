// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "TestS.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "Hello.h"

struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT ("file://test.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool result {};
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (options.ior);

      Test::Hello_var hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Test failed - Not regression - Unexpected Nil Test::Hello "
                                       "reference <%s>\n"),
                             options.ior),
                            1);
        }

      // Check this isn't generating transients for any other reason
      hello->ping ();

      try
        {
          hello->throw_location_forward ();

          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("REGRESSION - Test has failed !!!\n")));
          result = 1;
        }
      catch (CORBA::TRANSIENT const &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client catches a TRANSIENT, as expected. No problem!\n")));
        }

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

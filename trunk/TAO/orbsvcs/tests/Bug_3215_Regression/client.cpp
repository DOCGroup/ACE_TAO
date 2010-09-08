// $Id$

// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "TestS.h"
#include "ace/Get_Opt.h"
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);


      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior );


      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () );


      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // Check this isn't generating transients for any other reason
      hello->ping ();

      try
        {
          hello->throw_location_forward ();


          ACE_DEBUG ((LM_ERROR, "REGRESSION - Test has failed !!!\n"));
          result = 1;
        }
      catch (CORBA::TRANSIENT my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          ACE_DEBUG ((LM_DEBUG, "Client catches a TRANSIENT, as expected. No problem !\n"));
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

// $Id$

// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "TestC.h"
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result = 0;
  try
    {
      int one = 1;
      char CLIENT[] = "client"; // Copy const into non-const storage.
      char *argv1[] = { CLIENT, 0 };

      CORBA::ORB_var orb1 =
      CORBA::ORB_init (one, argv1, "orb1" );

      int three = 3;
      char OPTION[] = "-ORBFTSendFullGroupTC"; // Copy const into non-const storage.
      char VALUE[]  = "1"; // Copy const into non-const storage.
      char *argv2[] = { CLIENT, OPTION, VALUE, 0 };
      CORBA::ORB_var orb2;

      try
        {
          orb2 =
            CORBA::ORB_init (three, argv2, "orb2" );
        }
      catch (CORBA::BAD_PARAM bad_param)
        {
          ACE_UNUSED_ARG (bad_param);
          ACE_DEBUG ((LM_ERROR, "Error - REGRESSION - ORB doesn't even accept the config option\n"));
          return 1;
        }


      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb1->string_to_object (ior );

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () );

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      try
        {
          hello->check_normal_context ();
        }
      catch (Test::MyException my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          ACE_DEBUG ((LM_ERROR, "Error - REGRESSION - normal format service context not transmitted\n"));
          result = 1;
        }


      tmp =
        orb2->string_to_object (ior );

      hello =
        Test::Hello::_narrow(tmp.in () );

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Test failed - Not regression - Unexpected Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      try
        {
          hello->check_extended_context ();
        }
      catch (Test::MyException my_ex)
        {
          ACE_UNUSED_ARG (my_ex);
          ACE_DEBUG ((LM_ERROR, "Error - REGRESSION - extended format service context not transmitted\n"));
          result = 1;
        }


      hello->shutdown ();

      orb1->destroy ();
      orb2->destroy ();
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

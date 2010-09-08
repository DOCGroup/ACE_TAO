// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

static const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
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
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      Test_var test = Test::_narrow(tmp.in ());

      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG, "Nil Test reference <%s>\n", ior), 1);
        }

      // Try a few times until we run out of "trys" or we no longer get
      // an exception.  Some times it takes a little while to begin
      // accepting again on AIX.
      for(size_t i = 0; i < 10; i++)
        try
          {
            // This first invocation will actually cause the connection to
            // the server.  Since the server has run out of file handles,
            // it can not accept the new connection.  On AIX, this will
            // receive a CORBA::COMM_FAILURE exception because it doesn't
            // complete in a timely manner.  It does not mean that the test
            // has failed, as long as the server performs the correct
            // function.
            test->simple ();
            break;
          }
        catch (const CORBA::COMM_FAILURE&)
          {
            ACE_OS::sleep (1);
          }

      test->simple ();
      test->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

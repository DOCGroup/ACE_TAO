// $Id$

#include "ace/Get_Opt.h"
#include "TestC.h"
#include "ace/OS_NS_unistd.h"

const char *ior = "file://test.ior";
int num_calls = 10;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        num_calls = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        break;
      }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }

      // Do a couple of calls on the server. If the sever is trying to
      // do something stupid like sending an exception to us, then it
      // won't be able to handle more than 1 request from us.
      Test::Timestamp time = 10;
      for (int i = 0; i < num_calls; i++)
        {
          roundtrip->test_method (time);

          ACE_DEBUG ((LM_DEBUG, "Sent call # %d \n", i));
        }
      orb->destroy();
      // The following sleep is a workaround for a defect in the Windows
      // implementation of sockets (Win XP)
      // When this client exits after writing to a localhost socket
      // Windows discards any data that has not been read by the server.
      // The sleep gives the server time to catch up.  num_calls/2 gives
      // it half a second per request which *really* should be overkill, but
      // it also means the client will terminate before the server actually
      // handles the requests (a good thing).
      // I'm still trying to decide whether this should be a bugzilla entry.
      // wilsond@ociweb.com
      ACE_OS::sleep(num_calls/2);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("");
      return 1;
    }

  return 0;
}

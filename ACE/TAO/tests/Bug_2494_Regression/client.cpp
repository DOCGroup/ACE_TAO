// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_string.h"
#include "testC.h"

ACE_RCSID(Bug_2494_Regression, client, "$Id$")

const char *ior = "file://test.ior";
int nthreads = 5;
int do_shutdown = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-k <ior> "
                           "[-n <nthreads> | -x]"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

namespace
{
  const char *twohundredbytes =
  "12345678901234567890123456789012345678901234567890"
  "12345678901234567890123456789012345678901234567890"
  "12345678901234567890123456789012345678901234567890"
  "12345678901234567890123456789012345678901234567890"
  ;
}

struct Worker : ACE_Task_Base
{

  Worker (Simple_Server_ptr srv)
    : srv_(Simple_Server::_duplicate(srv))
  {
  }

  Simple_Server_var srv_;

  int svc ()
  {
    char* str = CORBA::string_alloc (200*2000 + 1);
    if (!str) return 1;
    str[0] = CORBA::Char('\0');
    for (int i=0; i < 2000; ++i)
      {
        ACE_OS::strcat(str, twohundredbytes);
      }

    while (1)
      {
        try
          {
            const char *ret = srv_->test_method (str);
            ACE_TRY_CHECK;
            if (0 != ACE_OS::strcmp (str, ret)) return 1;
          }
        catch (CORBA::Exception& ex)
          {
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Exception caught: \n%s\n"),
                        ACE_TEXT_CHAR_TO_TCHAR (ex._info ().c_str ())));
            return 1;
          }
      }
    return 0;
  }

};

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (ior);

      Simple_Server_var server = Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior), 1);
        }

      if (do_shutdown)
        {
          server->shutdown ();
        }
      else
        {
          Worker wrk (server.in ());
          wrk.activate (THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED, nthreads);
          wrk.thr_mgr ()->wait ();
        }
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("(%P|%t) Exception caught: \n%s\n"),
                  ACE_TEXT_CHAR_TO_TCHAR (ex._info ().c_str ())));
      return 1;
    }

  return 0;
}

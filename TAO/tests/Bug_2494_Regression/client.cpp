#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_stdlib.h"
#include "testC.h"
#include <cstring>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            ACE_TCHAR const *&ior,
            int &nthreads,
            bool &do_shutdown)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = true;
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
  // Indicates successful parsing of the command line
  return 0;
}

namespace
{
  char const * const two_hundred_bytes =
  "12345678901234567890123456789012345678901234567890"
  "12345678901234567890123456789012345678901234567890"
  "12345678901234567890123456789012345678901234567890"
  "12345678901234567890123456789012345678901234567890"
  ;
}

struct Worker : ACE_Task_Base
{
  explicit Worker (Simple_Server_ptr server)
    : server_ (Simple_Server::_duplicate (server))
  {
  }

  Worker (Worker const &) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker &&) = delete;

  Simple_Server_var const server_;

  int svc () override
  {
    CORBA::String_var str = CORBA::string_alloc (200 * 2000 + 1);
    if (!str.in ())
      return 1;
    str.inout ()[0] = CORBA::Char ('\0');
    for (int i {}; i < 2000; ++i)
      {
        std::strcat (str.inout (), two_hundred_bytes);
      }

    while (true)
      {
        try
          {
            CORBA::String_var const ret = this->server_->test_method (str.in ());
            if (std::strcmp (str.in (), ret.in ()) != 0)
              return 1;
          }
        catch (CORBA::Exception const &ex)
          {
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Exception caught: \n%C\n"),
                        ex._info ().c_str ()));
            return 1;
          }
      }
    return 0;
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  int nthreads {5};
  bool do_shutdown {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior, nthreads, do_shutdown) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (ior);

      Simple_Server_var const server = Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior), 1);
        }

      if (do_shutdown)
        {
          server->shutdown ();
        }
      else
        {
          Worker worker (server.in ());
          if (worker.activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
                               nthreads) != 0)
            ACE_ERROR_RETURN ((LM_ERROR, "Cannot activate client threads\n"), 1);
          worker.thr_mgr ()->wait ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("(%P|%t) ERROR: Exception caught: \n%C\n"),
                  ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/OS_NS_strings.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int client_num = 1;
int report_exception = 1;
bool is_shutdown = false;
int num_threads = 1;
const ACE_TCHAR *test = ACE_TEXT("");
int goal = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:e:g:k:n:st:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        client_num = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'e':
        report_exception = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'g':
        goal = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        num_threads = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 's':
        is_shutdown = true;
        break;
      case 't':
        test = get_opts.opt_arg();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage:  %C ")
                           ACE_TEXT("[-c <client #>] ")
                           ACE_TEXT("[-e (show exception) <0|1>] ")
                           ACE_TEXT("[-k <ior>] ")
                           ACE_TEXT("[-s (shutdownE)] ")
                           ACE_TEXT("[-t <test> <goal>] ")
                           ACE_TEXT("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class ClientTask : public ACE_Task_Base
{
public:
  ClientTask (Test::Hello_ptr h)
  : replies_(0),
    nis_(0),
    exs_(0),
    lock_(),
    target_ (Test::Hello::_duplicate(h))
  {}

  int svc (void);

  int replies_;
  int nis_;
  int exs_;

private:

  ACE_Mutex lock_;
  Test::Hello_var target_;
};

int ClientTask::svc (void)
{
  int ndx = 0;
  {
    ACE_GUARD_RETURN (ACE_Mutex, mon, this->lock_, -1);
    ndx = client_num++;
  }

  for (int i = 5; i > 0; i--)
    {
      try
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT("Client %d calling server\n"),
                     ndx));
          CORBA::String_var the_string = target_->get_string (ndx);
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT("Client %d received return text of: [%s %d].\n"),
                     ndx, the_string.in(), ndx));
          {
            ACE_GUARD_RETURN (ACE_Mutex, mon, this->lock_, -1);
            this->replies_++;
          }
        }
      catch (const CORBA::NO_IMPLEMENT& )
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Client %d caught full queue exception.\n"),
                      ndx));
          {
            ACE_GUARD_RETURN (ACE_Mutex, mon, this->lock_, -1);
            this->nis_++;
          }
        }
      catch (const CORBA::TRANSIENT& )
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Client %d caught transent, retrying.\n"),
                      ndx));
          continue;
        }
      catch (const CORBA::Exception& ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Client %d caught other exception. %s\n"),
                      ndx, ex._name()));

          {
            ACE_GUARD_RETURN (ACE_Mutex, mon, this->lock_, -1);
            this->exs_++;
          }
        }
      break;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Client: Nil Test::Hello reference <%s>\n",
                             ior),
                            -1);
        }

      for (int i = 3; i > 0; i--)
        {
          try
            {
              if (hello->_non_existent())
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT("Client: non_existent returns true\n")),
                                    -1);
                }
              else
                break;
            }
          catch (CORBA::TRANSIENT &)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("Client: got a transient exception with %d tries remaining\n"),
                          i-1));
            }
        }


      if (is_shutdown)
      {
        hello->shutdown();
      }
      else
      {
        ClientTask ct (hello.in());
        ct.activate (THR_NEW_LWP | THR_JOINABLE, num_threads);
        ct.wait ();
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Client: replies = %d, NO_IMPL = %d, other ex = %d\n"),
                    ct.replies_, ct.nis_, ct.exs_));
        if (ct.exs_ > 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Client: ERROR: unexpected exception caught.\n")));
            result = -1;
          }
        else if (ACE_OS::strcasecmp (test, ACE_TEXT ("max_queue")) == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Client: max_queue test, got %d overages, expected %d\n"),
                        ct.nis_, goal));
            if (ct.nis_ != goal)
              {
                result = -1;
              }
          }
      }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (report_exception)
        ex._tao_print_exception (ACE_TEXT("Exception caught:"));
      return -1;
    }

  return result;
}

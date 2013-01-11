// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

class MT_Requestor : public virtual ACE_Task_Base
{
public:
  MT_Requestor (Test::Sleeper_ptr s);

  virtual int svc (void);
private:
  Test::Sleeper_var sleeper_;
};

MT_Requestor::MT_Requestor (Test::Sleeper_ptr s)
  : sleeper_ (Test::Sleeper::_duplicate (s))
{
}

int
MT_Requestor::svc ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %t calling delay\n")));
  try
    {
      this->sleeper_->delay();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %d caught %s\n"),
                  ex._name()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %t done\n")));
  return 0;
}

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int report_exception = 1;
bool is_shutdown = false;
int num_threads = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("e:k:n:s"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        report_exception = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        num_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 's':
        is_shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-e <0|1> "
                           "-k <ior> "
                           "-n <threads> "
                           "-s (shutdown) "
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

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Sleeper_var sleeper =
        Test::Sleeper::_narrow(tmp.in ());

      if (CORBA::is_nil (sleeper.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Sleeper reference <%s>\n",
                             ior),
                            1);
        }

      if (is_shutdown)
      {
        sleeper->shutdown();
      }
      else
      {
        MT_Requestor requestor (sleeper.in());
        requestor.activate (THR_NEW_LWP | THR_JOINABLE, num_threads);
        requestor.wait ();
      }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (report_exception)
        ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

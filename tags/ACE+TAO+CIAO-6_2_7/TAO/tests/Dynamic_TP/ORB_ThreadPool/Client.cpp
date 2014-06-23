// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

class MT_Requestor : public virtual ACE_Task_Base
{
public:
  MT_Requestor (Test::Middle_ptr m, CORBA::Short sec);

  virtual int svc (void);
private:
  Test::Middle_var middle_;
  CORBA::Short seconds_;
};

MT_Requestor::MT_Requestor (Test::Middle_ptr m, CORBA::Short sec)
  : middle_ (Test::Middle::_duplicate (m)),
    seconds_ (sec)

{
}

int
MT_Requestor::svc ()
{
  int retries = 5;
  while (retries-- > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Client thread %t calling delay with %d retries\n"),
                  retries));
      try
        {
          this->middle_->call_delay(seconds_);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client thread %t delay succeeded\n")));
          break;
        }
      catch (CORBA::Exception &ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client thread %t caught %s\n"),
                      ex._name()));
        }
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %t done\n")));
  return 0;
}

const ACE_TCHAR *ior = ACE_TEXT("file://middle.ior");
bool do_shutdown = false;
CORBA::Short sec = 2;
int num_threads = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:s:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        num_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 's':
        sec = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        do_shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-n <threads> "
                           "-x "
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

      Test::Middle_var middle =
        Test::Middle::_narrow(tmp.in ());

      if (CORBA::is_nil (middle.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Middle reference <%s>\n",
                             ior),
                            1);
        }

      MT_Requestor requestor (middle.in(), sec);
      requestor.activate (THR_NEW_LWP | THR_JOINABLE, num_threads);
      requestor.wait ();

      if (do_shutdown)
      {
        middle->shutdown();
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client main exception caught:");
      return 1;
    }

  return 0;
}

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

class MT_Requestor : public virtual ACE_Task_Base
{
public:
  MT_Requestor (Test::Middle_ptr m, CORBA::Short sec);
  MT_Requestor (MT_Requestor const &) = delete;
  MT_Requestor (MT_Requestor &&) = delete;
  MT_Requestor &operator= (MT_Requestor const &) = delete;
  MT_Requestor &operator= (MT_Requestor &&) = delete;

  int svc () override;
private:
  Test::Middle_var const middle_;
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
  int retries {5};
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
      catch (const CORBA::Exception &ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client thread %t caught %s\n"),
                      ex._name()));
        }
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %t done\n")));
  return 0;
}

struct Client_Options
{
  ACE_TString ior {ACE_TEXT("file://middle.ior")};
  bool do_shutdown {};
  CORBA::Short seconds {2};
  int num_threads {1};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:s:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'n':
        options.num_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 's':
        options.seconds = static_cast<CORBA::Short> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'x':
        options.do_shutdown = true;
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (options.ior.c_str ());

      Test::Middle_var const middle =
        Test::Middle::_narrow (tmp.in ());

      if (CORBA::is_nil (middle.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Middle reference <%s>\n",
                             options.ior.c_str ()),
                            1);
        }

      MT_Requestor requestor (middle.in (), options.seconds);
      requestor.activate (THR_NEW_LWP | THR_JOINABLE, options.num_threads);
      requestor.wait ();

      if (options.do_shutdown)
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

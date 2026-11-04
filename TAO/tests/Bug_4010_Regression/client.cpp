#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior,
            int &iterations,
            int &timeout)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:t:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 't':
        timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <iterations> "
                           "-t <timeout> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class PeriodicTask : public ACE_Event_Handler
{
public:
  PeriodicTask (CORBA::ORB_ptr const orb,
                std::basic_string<ACE_TCHAR> const &ior,
                int const iterations)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      ior_ (ior),
      iterations_left_ (iterations)
  {
  }
  PeriodicTask (PeriodicTask const &) = delete;
  PeriodicTask (PeriodicTask &&) = delete;
  PeriodicTask &operator= (PeriodicTask const &) = delete;
  PeriodicTask &operator= (PeriodicTask &&) = delete;

  ~PeriodicTask () override = default;

  bool process_failed () const
  {
    return this->process_failed_;
  }

  int handle_timeout (ACE_Time_Value const &, void const *) override
  {
    --this->iterations_left_;

    try
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\n(%P|%t) - handling timeout with %d iterations left\n",
                    this->iterations_left_ + 1));
        CORBA::Object_var const tmp =
          this->orb_->string_to_object (ACE_TEXT_ALWAYS_CHAR (this->ior_.c_str ()));

        Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

        if (CORBA::is_nil (hello.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "Nil Test::Hello reference <%s>\n",
                               this->ior_.c_str ()),
                              1);
          }

        CORBA::String_var const the_string = hello->get_string ();

        ++this->successful_connections_;
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                    the_string.in ()));
        ACE_OS::sleep (2);
        hello->shutdown ();

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - shutdown requested\n"));
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught:");
        this->process_failed_ = true;
        this->orb_->shutdown ();
      }

    if (this->successful_connections_ == 2)
      {
        this->process_failed_ = false;
        this->orb_->shutdown ();
      }

    return 0;
  }

private:
  CORBA::ORB_var orb_ {};
  std::basic_string<ACE_TCHAR> ior_ {};
  int iterations_left_ {};
  int successful_connections_ {};
  bool process_failed_ {true};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};
      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      int iterations {3};
      int timeout {10};

      if (parse_args (argc, argv, ior, iterations, timeout) != 0)
        return 1;

      PeriodicTask periodic_task (orb.in (), ior, iterations);

      orb->orb_core ()->reactor ()->schedule_timer (&periodic_task,
                                                    nullptr,
                                                    ACE_Time_Value::zero,
                                                    ACE_Time_Value (timeout));

      orb->run ();

      orb->destroy ();
      return periodic_task.process_failed () ? 1 : 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
}


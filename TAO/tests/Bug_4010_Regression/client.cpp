#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
int iterations {3};
int timeout {10};
int process_result {1};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
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
  explicit PeriodicTask (CORBA::ORB_var orb_in)
    : orb (orb_in)
    , iterations_left (iterations)
    , successful_connections {}
  {
  }
  PeriodicTask (PeriodicTask const &) = delete;
  PeriodicTask (PeriodicTask &&) = delete;
  PeriodicTask &operator= (PeriodicTask const &) = delete;
  PeriodicTask &operator= (PeriodicTask &&) = delete;

  ~PeriodicTask () override = default;

  int handle_timeout (ACE_Time_Value const &, void const *) override
  {
    --iterations_left;

    try
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\n(%P|%t) - handling timeout with %d iterations left\n",
                    iterations_left + 1));
        CORBA::Object_var tmp = orb->string_to_object (ior);

        Test::Hello_var hello = Test::Hello::_narrow (tmp.in ());

        if (CORBA::is_nil (hello.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "Nil Test::Hello reference <%s>\n",
                               ior),
                              1);
          }

        CORBA::String_var the_string = hello->get_string ();

        ++successful_connections;
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                    the_string.in ()));
        ACE_OS::sleep (2);
        hello->shutdown ();

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - shutdown requested\n"));
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught:");
        process_result = 1;
        orb->shutdown ();
      }

    if (successful_connections == 2)
      {
        process_result = 0;
        orb->shutdown ();
      }

    return 0;
  }

protected:
private:
  CORBA::ORB_var orb;
  int iterations_left;
  int successful_connections;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      PeriodicTask periodic_task (orb);

      orb->orb_core ()->reactor ()->schedule_timer (&periodic_task, nullptr,
        ACE_Time_Value::zero, ACE_Time_Value (timeout));

      orb->run ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return process_result;
}


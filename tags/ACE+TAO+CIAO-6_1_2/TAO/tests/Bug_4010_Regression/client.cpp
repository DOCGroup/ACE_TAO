// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
int iterations = 3;
int timeout = 10;
int process_result = 1;

int parse_args (int argc, ACE_TCHAR *argv[])
{
ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'i':
        iterations = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 't':
        timeout = ACE_OS::atoi(get_opts.opt_arg ());
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

  explicit PeriodicTask(CORBA::ORB_var orbIn)
      : ACE_Event_Handler(),
      orb(orbIn),
      iterationsLeft(iterations),
      successfulConnections(0)
  {
  }

  virtual ~PeriodicTask()
  {
  }

  virtual int handle_timeout(const ACE_Time_Value &, const void *)
  {
    --iterationsLeft;

    try
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\n(%P|%t) - handling timeout with %d iterations left\n",
                    iterationsLeft + 1));
        CORBA::Object_var tmp = orb->string_to_object(ior);

        Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());

        if (CORBA::is_nil (hello.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "Nil Test::Hello reference <%s>\n",
                               ior),
                              1);
          }

        CORBA::String_var the_string = hello->get_string ();

        ++successfulConnections;
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                    the_string.in ()));
        ACE_OS::sleep(2);
        hello->shutdown ();

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - shutdown requested\n"));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        process_result = 1;
        orb->shutdown();
      }

    if(successfulConnections == 2)
      {
        process_result = 0;
        orb->shutdown();
      }

    return 0;
  }

protected:

private:

  PeriodicTask(const PeriodicTask &);
  PeriodicTask & operator=(const PeriodicTask &);
  CORBA::ORB_var orb;
  int iterationsLeft;
  int successfulConnections;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      PeriodicTask periodicTask(orb);

      orb->orb_core()->reactor()->schedule_timer(&periodicTask, 0, ACE_Time_Value::zero, ACE_Time_Value(timeout));

      orb->run();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return process_result;
}


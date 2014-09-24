// $Id$
#include "testS.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include <ace/Task.h>
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior_input_file = ACE_TEXT ("file://serverA.ior");
int test_duration_sec = 15;
bool expect_object_not_exist = false;

class Client_Task : public ACE_Task_Base
{
  public:
    Client_Task (Test::Time_ptr obj)
    : test_ (Test::Time::_duplicate (obj)),
      communication_failed_ (false),
      reconnected_ (false),
      caught_object_not_exist_ (false)
    {}

    virtual int svc (void)
    {
      ACE_Time_Value start = ACE_OS::gettimeofday ();
      ACE_Time_Value elapsed;
      int i = 0;
      while (elapsed < ACE_Time_Value (test_duration_sec))
      {
        try
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)Request %d\n"), i ));
          test_->current_time();
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)Done request %d\n"), i ));
          if (communication_failed_)
          {
            communication_failed_ = false;
            reconnected_ = true;
          }
          ACE_OS::sleep (1);
        }
        catch (const CORBA::OBJECT_NOT_EXIST &)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)caught OBJECT_NOT_EXIST exception for request %d\n"), i ));
          caught_object_not_exist_ = true;
          ACE_OS::sleep (1);
        }
        catch (const CORBA::Exception & /*ex*/)
        {
          //ex._tao_print_exception ("Exception caught:");
          communication_failed_ = true;
          ACE_OS::sleep (1);
        }
        ++i;
        elapsed = ACE_OS::gettimeofday () - start;
      }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)Client thread exit \n")));
      return 0;
    }

    bool test_passed () const
    {
      bool conn_test = communication_failed_ != reconnected_;
      bool one_test = expect_object_not_exist == caught_object_not_exist_;
      bool success = (conn_test && one_test);
      if (!success)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t)Client results, cf = %d, r = %d, ")
                      ACE_TEXT ("eone = %d, cone = %d\n"),
                      communication_failed_, reconnected_,
                      expect_object_not_exist, caught_object_not_exist_));
        }
      return success;
    }

private:
  Test::Time_var test_;
  bool communication_failed_;
  bool reconnected_;
  bool caught_object_not_exist_;
};


int
parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:t:e:");
  int c;


  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'i':
          ior_input_file = get_opts.opt_arg ();
          break;
        case 't':
          test_duration_sec = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'e':
          expect_object_not_exist = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case '?':
          default:
          ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <iorfile> -t <test_duration>"
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
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
    {
      return 1;
    }

    CORBA::Object_var object =  orb->resolve_initial_references ("RootPOA");

    object = orb->string_to_object(ior_input_file);

    Test::Time_var test = Test::Time::_narrow(object.in ());

    if (CORBA::is_nil(test.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        "(%P|%t) Object reference is nil \n"),
        1);
    }

    Client_Task task (test.in ());
    task.activate (THR_NEW_LWP | THR_JOINABLE, 1, 1);

    ACE_Time_Value tv(test_duration_sec);
    orb->run (&tv);

    task.wait ();

    orb->destroy ();

    if (task.test_passed ())
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)Client test passed \n")));
    }
    else
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("(%P|%t)Client test failed.\n")), 1);
    }
  }
  catch (const CORBA::Exception &ex)
  {
    ex._tao_print_exception ("Exception caught by client:");
    return 1;
  }

  return 0;
}



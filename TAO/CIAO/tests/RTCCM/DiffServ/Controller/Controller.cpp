// $Id$

#include "SenderC.h"
#include "ReceiverC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "tests/RTCORBA/check_supported_priorities.cpp"

static const char *sender_ior = "file://sender.ior";
static const char *receiver_ior = "file://receiver.ior";
static int iterations = 1;
static int corba_priority = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "s:r:n:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        // sender ior
      case 's':
        sender_ior = get_opts.opt_arg ();
        break;

        // receiver ior
      case 'r':
        receiver_ior = get_opts.opt_arg ();
        break;

        // number of itarations
      case 'n':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

        // corba priority
      case 'p':
        corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-s <sender ior> [default is %s]\n"
                           "\t-r <receiver ior> [default is %s]\n"
                           "\t-n <number of invocations> [defaults to %d]\n"
                           "\t-p <corba priority> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           sender_ior,
                           receiver_ior,
                           iterations,
                           corba_priority),
                          -1);
      }

  return 0;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->string_to_object (sender_ior);

      DiffServ::Sender_var sender =
        DiffServ::Sender::_narrow (object);

      object =
        this->orb_->string_to_object (receiver_ior);

      DiffServ::Receiver_var receiver =
        DiffServ::Receiver::_narrow (object);

      sender->start (iterations,
                     corba_priority);

      sender->shutdown ();

      receiver->shutdown ();

      return 0;
    }
  catch (CORBA::Exception &exception)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unexpected exception caught by client: %s (%s)\n",
                  exception._name (),
                  exception._rep_id ()));

      return -1;
    }
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);

      return 0;
    }
  catch (CORBA::Exception &exception)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unexpected exception caught by client: %s (%s)\n",
                  exception._name (),
                  exception._rep_id ()));

      return -1;
    }
}

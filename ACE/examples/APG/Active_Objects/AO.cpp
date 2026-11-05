#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_unistd.h"
#include "ace/Activation_Queue.h"
#include "ace/Method_Request.h"
#include "ace/Task.h"
#include "ace/Future.h"
#include <array>
#include <memory>

// Listing 1 code/ch15
class HA_ControllerAgent
{
  // Proxy to the HA_Controller that is on the network.
public:
  HA_ControllerAgent ()
  {
    ACE_TRACE("HA_ControllerAgent::HA_ControllerAgent");
  }

  int status_update ()
  {
    ACE_TRACE ("HA_ControllerAgent::status_update");
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Obtaining a status_update in %t ")
                ACE_TEXT ("thread of control\n")));
    // Simulate time to send message and get status.
    ACE_OS::sleep (2);
    return next_result_id ();
  }

private:
  int next_result_id ()
  {
    ACE_TRACE ("HA_ControllerAgent::next_cmd_id");
    return status_result_++;
  }

  int status_result_ {1};
};
// Listing 1
// Listing 2 code/ch15
class StatusUpdate : public ACE_Method_Request
{
public:
  StatusUpdate (StatusUpdate const &) = delete;
  StatusUpdate (StatusUpdate &&) = delete;
  StatusUpdate &operator= (StatusUpdate const &) = delete;
  StatusUpdate &operator= (StatusUpdate &&) = delete;

  StatusUpdate (HA_ControllerAgent &controller,
                ACE_Future<int> &return_val)
    : controller_ (controller), return_val_ (return_val)
  {
    ACE_TRACE ("StatusUpdate::StatusUpdate");
  }

  int call () override
  {
    ACE_TRACE ("StatusUpdate::call");

    // status_update with the controller.
    this->return_val_.set (this->controller_.status_update ());
    return 0;
  }

private:
  HA_ControllerAgent &controller_;
  ACE_Future<int> return_val_;
};
// Listing 2
// Listing 3 code/ch15
class ExitMethod : public ACE_Method_Request
{
public:
  int call () override
  {
    // Cause exit.
    return -1;
  }
};
// Listing 3
// Listing 4 code/ch15
class Scheduler : public ACE_Task_Base
{
public:
  Scheduler (Scheduler const &) = delete;
  Scheduler &operator = (Scheduler const &) = delete;
  Scheduler (Scheduler &&) = delete;
  Scheduler &operator = (Scheduler &&) = delete;

  Scheduler ()
  {
    ACE_TRACE ("Scheduler::Scheduler");
    this->activate ();
  }

  int svc () override
  {
    ACE_TRACE ("Scheduler::svc");

    while (1)
      {
        // Dequeue the next method object
        std::unique_ptr<ACE_Method_Request> request (this->activation_queue_.dequeue ());

        // Invoke the method request.
        if (request == nullptr || request->call () == -1)
          break;
      }

    return 0;
  }

  int enqueue (std::unique_ptr<ACE_Method_Request> request)
  {
    ACE_TRACE ("Scheduler::enqueue");
    int const result = this->activation_queue_.enqueue (request.get ());
    if (result != -1)
      request.release ();
    return result;
  }

private:
  ACE_Activation_Queue activation_queue_;
};
// Listing 4
// Listing 5 code/ch15
class HA_ControllerAgentProxy
{
  // This acts as a Proxy to the controller impl object.
public:
  HA_ControllerAgentProxy () = default;
  HA_ControllerAgentProxy (HA_ControllerAgentProxy const &) = delete;
  HA_ControllerAgentProxy &operator = (HA_ControllerAgentProxy const &) = delete;
  HA_ControllerAgentProxy (HA_ControllerAgentProxy &&) = delete;
  HA_ControllerAgentProxy &operator = (HA_ControllerAgentProxy &&) = delete;

  ACE_Future<int> status_update ()
  {
    ACE_TRACE("HA_ControllerAgentProxy::status_update");
    ACE_Future<int> result;

    // Create and enqueue a method request on the scheduler.
    this->scheduler_.enqueue
      (std::make_unique<StatusUpdate> (this->controller_, result));

    // Return Future to the client.
    return result;
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void exit ()
  {
  //FUZZ: enable check_for_lack_ACE_OS
    ACE_TRACE ("HA_ControllerAgentProxy::exit");
    this->scheduler_.enqueue (std::make_unique<ExitMethod> ());
  }

private:
  Scheduler scheduler_;
  HA_ControllerAgent controller_;
};
// Listing 5
// Listing 6 code/ch15
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_ControllerAgentProxy controller;
  std::array<ACE_Future<int>, 10> results;

  for (int i = 0 ; i < 10; i++)
    results[i] = controller.status_update ();

  ACE_OS::sleep (5);  // Do other work.

  // Get results...
  for (int j = 0; j < 10; j++)
    {
      int result {};
      results[j].get (result);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("New status_update %d\n"), result));
    }

  // Cause the status_updater threads to exit.
  controller.exit ();
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
// Listing 6

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */

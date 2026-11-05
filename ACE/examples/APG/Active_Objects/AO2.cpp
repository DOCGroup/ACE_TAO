#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_unistd.h"
#include "ace/Activation_Queue.h"
#include "ace/Method_Request.h"
#include "ace/Task.h"
#include "ace/Future.h"
#include <array>
#include <memory>

class HA_ControllerAgent
{
  // Proxy to the HA_Controller that is on the network.
public:
  HA_ControllerAgent () = default;
  HA_ControllerAgent (HA_ControllerAgent const &) = delete;
  HA_ControllerAgent (HA_ControllerAgent &&) = delete;
  HA_ControllerAgent &operator= (HA_ControllerAgent const &) = delete;
  HA_ControllerAgent &operator= (HA_ControllerAgent &&) = delete;

  int status_update ()
  {
    ACE_TRACE ("HA_ControllerAgent::status_update");
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Obtaining a status_update in %t ")
                ACE_TEXT ("thread of control\n")));
    // Simulate sending message to controller
    // and getting status.
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

class ExitMethod : public ACE_Method_Request
{
public:
  ExitMethod () = default;
  ExitMethod (ExitMethod const &) = delete;
  ExitMethod (ExitMethod &&) = delete;
  ExitMethod &operator= (ExitMethod const &) = delete;
  ExitMethod &operator= (ExitMethod &&) = delete;

  int call () override
  {
    // Cause exit.
    return -1;
  }
};

class Scheduler : public ACE_Task_Base
{
public:
  Scheduler (Scheduler const &) = delete;
  Scheduler &operator= (Scheduler const &) = delete;
  Scheduler (Scheduler &&) = delete;
  Scheduler &operator= (Scheduler &&) = delete;

  Scheduler ()
  {
    ACE_TRACE ("Scheduler::Scheduler");
    this->activate ();
  }

  int svc () override
  {
    ACE_TRACE ("Scheduler::svc");

    while (true)
      {
        // Dequeue the next method object
        std::unique_ptr<ACE_Method_Request> request {this->activation_queue_.dequeue ()};

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
    ACE_TRACE ("HA_ControllerAgentProxy::status_update");
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

// Listing 1 code/ch15
class CompletionCallBack : public ACE_Future_Observer<int>
{
public:
  explicit CompletionCallBack (HA_ControllerAgentProxy &proxy)
    : proxy_ (proxy)
  { }

  CompletionCallBack (CompletionCallBack const &) = delete;
  CompletionCallBack &operator = (CompletionCallBack const &) = delete;
  CompletionCallBack (CompletionCallBack &&) = delete;
  CompletionCallBack &operator = (CompletionCallBack &&) = delete;

  void update (ACE_Future<int> const &future) override
  {
    int result {};
    future.get (result);
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("(%t) New Status %d\n"), result));
    if (result == 10)
      this->proxy_.exit ();
  }

private:
  HA_ControllerAgentProxy &proxy_;
};
// Listing 1
// Listing 2 code/ch15
constexpr int status_update_count = 10;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_ControllerAgentProxy controller;
  std::array<ACE_Future<int>, status_update_count> results {};
  CompletionCallBack cb (controller);

  for (int i = 0; i < status_update_count; ++i)
    {
      results[i] = controller.status_update ();
      results[i].attach (&cb);
    }

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
// Listing 2

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */

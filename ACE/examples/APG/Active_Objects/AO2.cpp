// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_unistd.h"
#include "ace/Activation_Queue.h"
#include "ace/Method_Request.h"
#include "ace/Task.h"
#include "ace/Future.h"
#include "ace/Auto_Ptr.h"

class HA_ControllerAgent
{
  // Proxy to the HA_Controller that is on the network.
public:
  HA_ControllerAgent ()
  {
    ACE_TRACE("HA_ControllerAgent::HA_ControllerAgent");
    status_result_ = 1;
  }

  int status_update (void)
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
  int next_result_id (void)
  {
    ACE_TRACE ("HA_ControllerAgent::next_cmd_id");
    return status_result_++;
  }

  int status_result_;
};

class StatusUpdate : public ACE_Method_Request
{
public:
  StatusUpdate (HA_ControllerAgent& controller,
                ACE_Future<int>& returnVal)
    : controller_(controller), returnVal_(returnVal)
  {
    ACE_TRACE ("StatusUpdate::StatusUpdate");
  }

  virtual int call (void)
  {
    ACE_TRACE ("StatusUpdate::call");

    // status_update with the controller.
    this->returnVal_.set (this->controller_.status_update ());
    return 0;
  }

private:
  HA_ControllerAgent& controller_;
  ACE_Future<int> returnVal_;
};

class ExitMethod : public ACE_Method_Request
{
public:
  virtual int call (void)
  {
    // Cause exit.
    return -1;
  }
};

class Scheduler : public ACE_Task_Base
{
public:
  Scheduler ()
  {
    ACE_TRACE ("Scheduler::Scheduler");
    this->activate ();
  }

  virtual int svc (void)
  {
    ACE_TRACE ("Scheduler::svc");

    while (1)
      {
        // Dequeue the next method object
        auto_ptr<ACE_Method_Request>
          request (this->activation_queue_.dequeue ());

        // Invoke the method request.
        if (request->call () == -1)
          break;
      }

    return 0;
  }

  int enqueue (ACE_Method_Request *request)
  {
    ACE_TRACE ("Scheduler::enqueue");
    return this->activation_queue_.enqueue (request);
  }

private:
  ACE_Activation_Queue activation_queue_;
};

class HA_ControllerAgentProxy
{
  // This acts as a Proxy to the controller impl object.
public:
  ACE_Future<int> status_update (void)
  {
    ACE_TRACE("HA_ControllerAgentProxy::status_update");
    ACE_Future<int> result;

    // Create and enqueue a method request on the scheduler.
    this->scheduler_.enqueue
      (new StatusUpdate (this->controller_, result));

    // Return Future to the client.
    return result;
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void exit (void)
  {
  //FUZZ: enable check_for_lack_ACE_OS

    ACE_TRACE ("HA_ControllerAgentProxy::exit");
    this->scheduler_.enqueue (new ExitMethod);
  }

private:
  Scheduler scheduler_;
  HA_ControllerAgent controller_;
};

// Listing 1 code/ch15
class CompletionCallBack : public ACE_Future_Observer<int>
{
public:
  CompletionCallBack (HA_ControllerAgentProxy& proxy)
    : proxy_(proxy)
  { }

  virtual void update (const ACE_Future<int>& future)
  {
    int result = 0;
    ((ACE_Future<int>)future).get (result);
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("(%t) New Status %d\n"), result));
    if (result == 10)
      this->proxy_.exit ();
  }

private:
  HA_ControllerAgentProxy& proxy_;
};
// Listing 1
// Listing 2 code/ch15
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_ControllerAgentProxy controller;
  ACE_Future<int> results[10];
  CompletionCallBack cb (controller);

  for (int i = 0 ; i < 10; i++)
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

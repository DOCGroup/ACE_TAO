
#ifndef LONGUPCALLS_MANAGER_HANDLER_H
#define LONGUPCALLS_MANAGER_HANDLER_H

#include "TestS.h"
#include "ace/Task.h"

class Manager_Handler
  :  public virtual POA_Test::AMI_ManagerHandler
{
public:
  Manager_Handler (Test::Manager_ptr manager,
                   Test::Controller_ptr controller);
  // Constructor

  // = The skeleton methods
  void start_workers () override;
  void start_workers_excep (::Messaging::ExceptionHolder *holder) override;

  Manager_Handler (Manager_Handler const &) = delete;
  Manager_Handler (Manager_Handler &&) = delete;
  Manager_Handler & operator= (Manager_Handler const &) = delete;
  Manager_Handler & operator= (Manager_Handler &&) = delete;

private:
  Test::Manager_var manager_;
  // The manager to reissue the request from the context of the
  // upcall

  Test::Controller_var controller_;
  // The controller
};

#endif /* LONGUPCALLS_MANAGER_HANDLER_H */

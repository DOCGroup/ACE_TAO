//
// $Id$
//

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
  virtual void start_workers (void);
  virtual void start_workers_excep (::Messaging::ExceptionHolder *holder);

private:
  Test::Manager_var manager_;
  // The manager to reissue the request from the context of the
  // upcall

  Test::Controller_var controller_;
  // The controller
};

#endif /* LONGUPCALLS_MANAGER_HANDLER_H */

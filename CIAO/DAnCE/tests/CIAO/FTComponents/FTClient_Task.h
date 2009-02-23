// $Id$

#ifndef FTCLIENT_TASK_H_
#define FTCLIENT_TASK_H_

#include "ace/Task.h"
#include "tao/ORB.h"

namespace CIDL_FTClient_Impl
{

  // forward declaration
  class FTClient_exec_i;

  class FTClient_Task : public ACE_Task_Base
  {
  public:
    FTClient_Task (CIDL_FTClient_Impl::FTClient_exec_i * client_executor);
    
    // Hook method that is called by the reactor when a timer expires.
    virtual int svc (void);

    void stop (void);

    void set_orb (CORBA::ORB_ptr orb);
    
  private:
    FTClient_exec_i * client_executor_;

    CORBA::ORB_var orb_;

    bool stop_;
  };

} // end namespace CIDL_FTClient_Impl

#endif /* FTCLIENT_TASK_H_ */

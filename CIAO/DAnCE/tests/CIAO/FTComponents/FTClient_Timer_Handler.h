// $Id$

#ifndef FTCLIENT_TIMER_HANDLER_H_
#define FTCLIENT_TIMER_HANDLER_H_

#include "ace/Event_Handler.h"
#include "tao/ORB.h"

namespace CIDL_FTClient_Impl
{

  // forward declaration
  class FTClient_exec_i;

  class FTClient_Timer_Handler : public ACE_Event_Handler
  {
  public:
    FTClient_Timer_Handler (CIDL_FTClient_Impl::FTClient_exec_i * client_executor);
    
    void set_orb (CORBA::ORB_ptr orb);

    // Hook method that is called by the reactor when a timer expires.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *);
    
  private:
    FTClient_exec_i * client_executor_;

    CORBA::ORB_var orb_;
  };

} // end namespace CIDL_FTClient_Impl

#endif /* FTCLIENT_TIMER_HANDLER_H_ */

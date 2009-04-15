// $Id$

#ifndef FTCLIENT_TIMER_HANDLER_H_
#define FTCLIENT_TIMER_HANDLER_H_

#include <list>
#include "ace/Event_Handler.h"
#include "ace/High_Res_Timer.h"
#include "tao/ORB.h"
#include "WorkerC.h"

namespace CIDL_FTClient_Impl
{

  // forward declaration
  class FTClient_exec_i;

  class FTClient_Timer_Handler : public ACE_Event_Handler
  {
  public:
    FTClient_Timer_Handler (CIDL_FTClient_Impl::FTClient_exec_i * client_executor,
                            bool logging = true);
    
    void set_orb (CORBA::ORB_ptr orb);

    void set_server (DeCoRAM::Worker_ptr server);

    void set_prefix (const char * prefix);

    // Hook method that is called by the reactor when a timer expires.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *);
    
    void dump (void);

  private:
    FTClient_exec_i * client_executor_;

    CORBA::ORB_var orb_;

    DeCoRAM::Worker_var server_;

    ACE_High_Res_Timer timer_;
   
    bool logging_;

    CORBA::ULong count_;

    struct ResponseTimeMeasurement {
      ACE_Time_Value server_time;
      ACE_Time_Value client_time;
    };

    typedef std::list<ResponseTimeMeasurement> TimingList;

    TimingList history_;

    std::string prefix_;
  };

} // end namespace CIDL_FTClient_Impl

#endif /* FTCLIENT_TIMER_HANDLER_H_ */

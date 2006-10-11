#ifndef LOCAL_MONITOR_H
#define LOCAL_MONITOR_H

//#include "ace/Time_Value.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/Event_Handler.h"
#include "ace/Mutex.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/Naming/Naming_Server.h"

#include "RACE/Monitor/Central_Monitor/MonitorC.h"

namespace CIAO
{

  namespace RACE
  {

    class Event_Handler: public ACE_Event_Handler
    {
    public:

      int handle_timeout (const ACE_Time_Value &,
                          const void *ref);
    };

    class Local_Monitor
    {
    public:

      Local_Monitor (const char *instance_id,
               CORBA::ORB_ptr orb);

      virtual ~Local_Monitor ();

      virtual void start (const char *id);

      virtual void stop ();

      virtual void dump ();

      virtual int push ();

    protected:

      virtual int resolve_central_monitor (CORBA::ORB_ptr);

      virtual int register_timer ();

    private:

      CORBA::String_var instance_id_;

      Delays delays_;

      long timer_id_;

      Event_Handler *handler_;

      ACE_Reactor *reactor_;

      ACE_Time_Value interval_;

	  ACE_Time_Value start_time_;

      ACE_High_Res_Timer timer_;

      Execution_Time_Monitor_var monitor_;

      bool initialized_;

      ACE_Mutex mutex_;
    };

  };
};

#endif /* LOCAL_MONITOR_H */

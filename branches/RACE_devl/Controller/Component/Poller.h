#ifndef POLLER_H
#define POLLER_H

#include "Monitor/Central_Monitor/MonitorC.h"
#include "TargetManager/TargetManagerExtC.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include "ace/Containers_T.h"
#include "ace/Mutex.h"
#include "common.h"

namespace CIAO
{

  namespace RACE
  {
    class Poller : public ACE_Event_Handler
    {
    public:

      Poller (::CIAO::TargetManagerExt_ptr TM_ptr,
              ::CIAO::RACE::Execution_Time_Monitor_ptr monitor,
              ACE_Reactor *reactor,
              size_t interval);

      virtual ~Poller ();

      virtual void init ();

      virtual int start_poller ();

      virtual int stop_poller ();

      virtual int register_string (App_String & string);

      virtual int handle_timeout (const ACE_Time_Value &, const void *);

    protected:
      virtual size_t deadline_miss_count (const Delays &delay,
                                          CORBA::Double deadline);

      virtual void dump_delays (const Delays &delay);

      virtual void populate_info (App_String & string);

      virtual int set_priority (const App_String & app_string);

      virtual void set_dms_initial_priority (App_String & app_string);

      virtual void set_muf_initial_priority (App_String & app_string);

      virtual void perform_control (double g_miss_ratio, double be_miss_ratio);

      virtual int select_task ();

    private:
      /// TM object reference.
      ::CIAO::TargetManagerExt_var TM_;

      /// Reference of the central monitor.
      ::CIAO::RACE::Execution_Time_Monitor_var monitor_;

      /// Pointer to the ORB's reactor.
      ACE_Reactor *orb_reactor_;

      /// Sampling period.
      ACE_Time_Value interval_;

      ACE_Time_Value start_time_;

      /// Is the poller active?
      bool active_;

      bool initialized_;

      long timer_id_;

      /// Sequence of the object reference of all the node managers in the
      /// domain (this is obtained from the TM).
      CIAO::Host_NodeManager_seq_var node_manager_seq_;

      /// Vector of application strings registered with the controller.
      ACE_Vector <App_String> strings_;

      /// Stack that contains the vector index of best effort task moved to
      /// the guaranteed class.
      ACE_Bounded_Stack <size_t> stack_;

      size_t N;


      ACE_Mutex mutex_;
    };
  }
}

#endif /*POLLER_H */

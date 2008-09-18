#ifndef LOCAL_MONITOR_H
#define LOCAL_MONITOR_H

#include "ace/Log_Msg.h"
#include "ace/Event_Handler.h"
#include "ace/Mutex.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "Monitor/Central_Monitor/MonitorC.h"
#include "Local_Monitor_export.h"

namespace CIAO
{

  namespace RACE
  {

    /**
     * This is a simple monitoring class that measures the time interval
     * between the start and stop method calls. The collected data is
     * periodicall either pushed to a centralized monitor or dumped to a
     * log file.
     */
    class LOCAL_MONITOR_Export Local_Monitor: public ACE_Event_Handler
    {
    public:


      /**
       * Constructor.  @prarm instance_id ID.  @param orb Pointer to the
       * ORB.  @param period Time interval of the periodic task.  @param
       * start_time Time when to start the periodic task.  @param dump_log
       * If true, the collected data is dumped to a log file, else, the
       * data is pushed to the centralized monitor.
       */
      Local_Monitor (const char *instance_id,
                     CORBA::ORB_ptr orb,
                     size_t period = 10,
                     size_t start_time = 10,
                     bool dump_log = true);

      // Descructor.
      virtual ~Local_Monitor ();

      // Start the clock.
      virtual void start (const char *id);

      // Stop the clock.
      virtual void stop ();

      // Periodic task.
      virtual int handle_timeout (const ACE_Time_Value &,
                                  const void *ref);

      // Dump data to the output log file.
      virtual int dump ();


      // Push the collected data to the centralized monitor.
      virtual int push ();

    protected:

      // Obtain the object reference of the centralized monitor using the
      // naming service.
      virtual int resolve_central_monitor (CORBA::ORB_ptr);

      // Register the periodic task with the ORB's reactor.
      virtual int register_timer ();

    private:

      // ID.
      CORBA::String_var instance_id_;

      // Collected data.
      Delays delays_;

      // ID of periodic task registered with the ORB's reactor.
      long timer_id_;

      // Pointer to the ORB's reactor.
      ACE_Reactor *reactor_;

      // Period.
      ACE_Time_Value interval_;

      // Start time.
      ACE_Time_Value start_time_;

      // Timer object.
      ACE_High_Res_Timer timer_;

      // Object reference to the centralized monitor.
      Execution_Time_Monitor_var monitor_;


      // Is this monitor initialized?
      bool initialized_;

      // Mutex to avoid race conditions.
      ACE_Mutex mutex_;

      // Output log file pointer.
      FILE *log_file_;

      // Dump the data to the log file or push it to the centralized
      // monitor.
      bool dump_log_;

    };

  };
};

#endif /* LOCAL_MONITOR_H */

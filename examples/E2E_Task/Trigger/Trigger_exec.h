#ifndef CIAO_TRIGGER_EXEC_H
#define CIAO_TRIGGER_EXEC_H

#include /**/ "ace/pre.h"

#include "TriggerEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Trigger_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/High_Res_Timer.h"
#include "Metrics.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Trigger
    {
      // Forward decleration.
      class PerfMon;

      class TRIGGER_EXEC_Export Trigger_exec_i
        : public virtual Trigger_Exec,
          public virtual TAO_Local_RefCounted_Object,
          public virtual ACE_Task_Base
      {
        public:

        Trigger_exec_i (void);

        virtual ~Trigger_exec_i (void);

        virtual int svc ();

        virtual ::CORBA::Long period ();

        virtual void period (::CORBA::Long period);

        virtual ::CORBA::Double load ();

        virtual void load (::CORBA::Double load);

        virtual char* task_ID ();

        virtual void task_ID (const char *task_ID);

        virtual ::CORBA::Boolean set_rate (::CORBA::Double rate);

        virtual Performance metrics ();

        virtual void
        set_session_context (::Components::SessionContext_ptr ctx);

        virtual void ciao_preactivate ();

        virtual void ciao_postactivate ();

        virtual void ccm_activate ();

        virtual void ccm_passivate ();

        virtual void ccm_remove ();

        private:
        ::CIAO::RACE::CCM_Trigger_Context_var context_;

        // Task period specified in usecs.
        ACE_hrtime_t period_;

        // Task ID.
        ::CORBA::String_var ID_;

        // Load coefficient.
        double load_;

        // Indicates whether the active object has been activated or not.
        bool active_;

        /// The high resolution timer.
        ACE_High_Res_Timer timer_;

        /// Elapsed time.
        ACE_hrtime_t elapsed_time_;

        // Mutex for the period_ variable.
        ACE_Mutex mutex_;

        // Performance metrics.
        Performance metrics_;

        // Performance monitor.
        PerfMon *monitor_;

      };

      // This class periodically outputs performance metrics.
      class PerfMon : public virtual ACE_Task_Base
      {
      public:

        PerfMon (Trigger_exec_i &trigger,
                 ACE_hrtime_t period);

        virtual ~PerfMon ();

        int svc ();

      private:

        Trigger_exec_i &trigger_;

        // Monitor period specified in usecs.
        ACE_hrtime_t period_;

      };

      class TRIGGER_EXEC_Export Trigger_Factory_exec_i
        : public virtual Trigger_Factory_Exec,
          public virtual TAO_Local_RefCounted_Object
      {
        public:
        Trigger_Factory_exec_i (void);
        virtual ~Trigger_Factory_exec_i (void);

        virtual ::Components::EnterpriseComponent_ptr
        create ();
      };

      extern "C" TRIGGER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Trigger_Factory_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_TRIGGER_EXEC_H */

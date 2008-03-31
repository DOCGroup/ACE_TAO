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
#include "Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Trigger
    {

      class TRIGGER_EXEC_Export Trigger_exec_i
        : public virtual Trigger_Exec,
          public virtual TAO_Local_RefCounted_Object,
          public virtual ACE_Task_Base
      {
        public:

        Trigger_exec_i (void);

        virtual ~Trigger_exec_i (void);

        virtual ::CIAO::RACE::CCM_ScienceData_ptr get_data_in ();

        virtual int svc ();

        virtual ::CORBA::Double rate ();

        virtual void rate (::CORBA::Double rate);

        virtual ::CORBA::Long subtasks ();

        virtual void subtasks (::CORBA::Long subtasks);

        virtual ::CORBA::Double load ();

        virtual void load (::CORBA::Double load);

        virtual char* task_ID ();

        virtual void task_ID (const char *task_ID);

        virtual ::CORBA::Boolean set_rate (::CORBA::Double rate);

        virtual void
        set_session_context (::Components::SessionContext_ptr ctx);

        virtual void ciao_preactivate ();

        virtual void ciao_postactivate ();

        virtual void ccm_activate ();

        virtual void ccm_passivate ();

        virtual void ccm_remove ();

        protected:

        // Helper methods.

        void update_priority ();

        private:
        ::CIAO::RACE::CCM_Trigger_Context_var context_;

        // Task rate specified in Hz.
        ::CORBA::Double rate_;

        // Load coefficient.
        double load_;

        // Number of subtasks in this task.
        size_t subtasks_;


        // Task ID.
        ::CORBA::String_var ID_;

        // Indicates whether the active object has been activated or not.
        bool active_;

        /// Timer used to calculate the throughput.
        ACE_High_Res_Timer tp_timer_;

        /// Timer used to calculate the e-2-e response time.
        ACE_High_Res_Timer response_timer_;


        // Mutex for the rate variable.
        ACE_Mutex mutex_;

        // Mutex for the metrics variable.
        ACE_Mutex metrics_mutex_;

        // Performance metrics.
        Metrics metrics_;

        // Logger.
        Logger *logger_;

        // Priority of the task.
        size_t priority_;

        friend class ScienceData_exec_i;
      };

      class TRIGGER_EXEC_Export ScienceData_exec_i
        : public virtual ::CIAO::RACE::CCM_ScienceData,
          public virtual TAO_Local_RefCounted_Object
      {
        public:

        ScienceData_exec_i (Trigger_exec_i & component);

        virtual ~ScienceData_exec_i (void);

        virtual void push_data (const ::CIAO::RACE::Message & msg);

        private:
        Trigger_exec_i & component_;
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

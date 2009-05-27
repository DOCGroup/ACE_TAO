#ifndef CIAO_MONITOR_EXEC_H
#define CIAO_MONITOR_EXEC_H

#include "Monitor_svnt.h"
#include "Monitor_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Central_Monitor_Impl
    {
      class MONITOR_EXEC_Export Execution_Time_Monitor_exec_i
        : public virtual ::CIAO::RACE::CCM_Execution_Time_Monitor,
          public virtual TAO_Local_RefCounted_Object
      {
        public:
        Execution_Time_Monitor_exec_i ();

        virtual ~Execution_Time_Monitor_exec_i ();

        virtual void push_delays (const char * id,
                                const Delays & delay)
          throw (CORBA::SystemException);

        virtual Delays * get_delays (const char * id)
          throw (::CORBA::SystemException,
                 ::CIAO::RACE::Execution_Time_Monitor::IdNotFound);

        private:
        ACE_Map_Manager <ACE_TString, Delays, ACE_Thread_Mutex> delay_map_;

        const size_t MAX_BUFFER_SIZE;
      };

      class MONITOR_EXEC_Export Central_Monitor_exec_i
        : public virtual Central_Monitor_Exec,
          public virtual TAO_Local_RefCounted_Object
      {
        public:
        Central_Monitor_exec_i (void);

        virtual ~Central_Monitor_exec_i (void);

        virtual ::CIAO::RACE::CCM_Execution_Time_Monitor_ptr get_Monitor ()
          throw ( ::CORBA::SystemException);


        virtual void set_session_context (::Components::SessionContext_ptr ctx)
          throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void ciao_preactivate ()
          throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void configuration_complete ()
          throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void ccm_activate ()
          throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void ccm_passivate ()
          throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void ccm_remove ()
          throw (::CORBA::SystemException, ::Components::CCMException);

        protected:
        Central_Monitor_Context *context_;
      };

      class MONITOR_EXEC_Export Central_Monitor_Home_exec_i
        : public virtual Central_Monitor_Home_Exec,
          public virtual TAO_Local_RefCounted_Object
      {
        public:
        Central_Monitor_Home_exec_i ();

        virtual ~Central_Monitor_Home_exec_i ();

        virtual ::Components::EnterpriseComponent_ptr create ()
          throw (::CORBA::SystemException, ::Components::CCMException);
      };

      extern "C" MONITOR_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Central_Monitor_Home_Impl ();
    }
  }
}

#endif /* CIAO_MONITOR_EXEC_H */

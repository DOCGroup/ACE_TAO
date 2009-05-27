#ifndef CIAO_OUTPUT_MANAGER_EXEC_H
#define CIAO_OUTPUT_MANAGER_EXEC_H

#include /**/ "ace/pre.h"

#include "Output_Manager_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Output_Manager_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Output_Manager_Impl
    {
      class Output_Manager_exec_i;

      class OUTPUT_MANAGER_EXEC_Export PlanEgress_exec_i
      : public virtual ::CIAO::RACE::CCM_PlanEgress,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        PlanEgress_exec_i (Output_Manager_exec_i &comp);
        virtual ~PlanEgress_exec_i (void);

        // Operations from ::CIAO::RACE::PlanEgress

        virtual ::CORBA::Boolean
        output_plan (
          const ::CIAO::RACE::Plan_Actions & plan_seq
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      private:
        Output_Manager_exec_i &comp_;

      };

      class OUTPUT_MANAGER_EXEC_Export Output_Manager_exec_i
      : public virtual Output_Manager_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Output_Manager_exec_i (void);
        virtual ~Output_Manager_exec_i (void);

        // Supported or inherited operations.

        // Attribute operations.

        // Port operations.

        virtual ::CIAO::RACE::CCM_PlanEgress_ptr
        get_output (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

        // Operations from Components::SessionComponent

        virtual void
        set_session_context (
          ::Components::SessionContext_ptr ctx
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ciao_preactivate (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        configuration_complete (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ccm_activate (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ccm_passivate (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        virtual void
        ccm_remove (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));

        protected:
        Output_Manager_Context *context_;

        friend class PlanEgress_exec_i;

      };

      class OUTPUT_MANAGER_EXEC_Export Output_Manager_Home_exec_i
      : public virtual Output_Manager_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Output_Manager_Home_exec_i (void);
        virtual ~Output_Manager_Home_exec_i (void);

        // Supported or inherited operations.

        // Home operations.

        // Factory and finder operations.

        // Attribute operations.

        // Implicit operations.

        virtual ::Components::EnterpriseComponent_ptr
        create (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
                          ::CORBA::SystemException,
                          ::Components::CCMException));
      };

      extern "C" OUTPUT_MANAGER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Output_Manager_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_OUTPUT_MANAGER_EXEC_H */

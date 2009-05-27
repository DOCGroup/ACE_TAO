#ifndef CIAO_XML_OUTPUT_ADAPTER_EXEC_H
#define CIAO_XML_OUTPUT_ADAPTER_EXEC_H

#include /**/ "ace/pre.h"

#include "XML_Output_Adapter_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Output_Adapter_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_XML_Output_Adapter_Impl
    {
      class XML_Output_Adapter_exec_i;

      class XML_OUTPUT_ADAPTER_EXEC_Export PlanEgress_exec_i
      : public virtual ::CIAO::RACE::CCM_PlanEgress,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        PlanEgress_exec_i (XML_Output_Adapter_exec_i &);
        virtual ~PlanEgress_exec_i (void);

        // Operations from ::CIAO::RACE::PlanEgress

       virtual ::CORBA::Boolean
        output_plan (const ::CIAO::RACE::Plan_Actions &plan_seq)
          ACE_THROW_SPEC ((CORBA::SystemException));

        virtual bool
          print_plan (const ::Deployment::DeploymentPlan &plan);


      private:
        XML_Output_Adapter_exec_i &comp_;
      };

      class XML_OUTPUT_ADAPTER_EXEC_Export XML_Output_Adapter_exec_i
      : public virtual XML_Output_Adapter_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        XML_Output_Adapter_exec_i (void);
        virtual ~XML_Output_Adapter_exec_i (void);

        // Supported or inherited operations.

        // Attribute operations.

        // Port operations.

        virtual ::CIAO::RACE::CCM_PlanEgress_ptr
        get_plan_egress (
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
        XML_Output_Adapter_Context *context_;

        friend class PlanEgress_exec_i;
      };

      class XML_OUTPUT_ADAPTER_EXEC_Export XML_Output_Adapter_Home_exec_i
      : public virtual XML_Output_Adapter_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        XML_Output_Adapter_Home_exec_i (void);
        virtual ~XML_Output_Adapter_Home_exec_i (void);

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

      extern "C" XML_OUTPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_XML_Output_Adapter_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_XML_OUTPUT_ADAPTER_EXEC_H */

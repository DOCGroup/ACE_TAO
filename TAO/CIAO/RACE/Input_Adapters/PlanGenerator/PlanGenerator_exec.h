// $Id$
//Author: Stoyan Paunov


#ifndef CIAO_PLANGENERATOR_EXEC_H
#define CIAO_PLANGENERATOR_EXEC_H

#include /**/ "ace/pre.h"

#include "PlanGenerator_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PlanGenerator_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_PlanGenerator_Impl
    {
      class PLANGENERATOR_EXEC_Export PlanGenerator_exec_i
      : public virtual PlanGenerator_Exec,
        public virtual TAO_Local_RefCounted_Object
      {
        public:
        PlanGenerator_exec_i (void);
        virtual ~PlanGenerator_exec_i (void);

        // Supported or inherited operations.

        // Attribute operations.

        // Port operations.

        virtual void
        push_deployment (
          ::CIAO::RACE::Deploy_Input *ev
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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
        ciao_postactivate (
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
        PlanGenerator_Context *context_;
      };

      class PLANGENERATOR_EXEC_Export PlanGenerator_Home_exec_i
      : public virtual PlanGenerator_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        PlanGenerator_Home_exec_i (void);
        virtual ~PlanGenerator_Home_exec_i (void);

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

      extern "C" PLANGENERATOR_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_PlanGenerator_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_PLANGENERATOR_EXEC_H */


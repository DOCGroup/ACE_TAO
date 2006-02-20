// $Id$
#ifndef CIAO_CONTROLLER_EXEC_H
#define CIAO_CONTROLLER_EXEC_H

#include /**/ "ace/pre.h"

#include "Controller_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Controller_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      class CONTROLLER_EXEC_Export Controller_exec_i
      : public virtual Controller_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Controller_exec_i (void);
        virtual ~Controller_exec_i (void);

        // Supported operations.
        virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

        virtual void stop (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException));

        // Attribute operations.
        virtual ::CORBA::Double
        sampling_period (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

        // Port operations.

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
        Controller_Context *context_;
      };

      class CONTROLLER_EXEC_Export Controller_Home_exec_i
      : public virtual Controller_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Controller_Home_exec_i (void);
        virtual ~Controller_Home_exec_i (void);

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

      extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Controller_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_CONTROLLER_EXEC_H */

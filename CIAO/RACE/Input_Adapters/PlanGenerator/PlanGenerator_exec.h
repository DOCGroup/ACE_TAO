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
          ::CIAO::RACE::Deploy_Input *ev);

        // Operations from Components::SessionComponent

        virtual void
        set_session_context (
          ::Components::SessionContext_ptr ctx);

        virtual void
        ciao_preactivate ();

        virtual void
        ciao_postactivate ();

        virtual void
        ccm_activate ();

        virtual void
        ccm_passivate ();

        virtual void
        ccm_remove ();

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
        create ();
      };

      extern "C" PLANGENERATOR_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_PlanGenerator_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_PLANGENERATOR_EXEC_H */


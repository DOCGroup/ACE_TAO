#ifndef CIAO_SUBTASK_EXEC_H
#define CIAO_SUBTASK_EXEC_H

#include /**/ "ace/pre.h"

#include "SubtaskEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Subtask_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Subtask
    {
      class SUBTASK_EXEC_Export Subtask_exec_i
        : public virtual Subtask_Exec,
          public virtual TAO_Local_RefCounted_Object
      {
        public:
        Subtask_exec_i (void);

        virtual ~Subtask_exec_i (void);

        virtual ::CORBA::Long iterations ();

        virtual void iterations (::CORBA::Long iterations);

        virtual char* ID ();

        virtual void ID (const char *ID);

        virtual void
        push_test_in (::CIAO::RACE::Test *ev);

        virtual void
        set_session_context (
          ::Components::SessionContext_ptr ctx);

        virtual void ciao_preactivate ();

        virtual void configuration_complete ();

        virtual void ccm_activate ();

        virtual void ccm_passivate ();

        virtual void ccm_remove ();

        private:
        ::CIAO::RACE::CCM_Subtask_Context_var context_;

        ::CORBA::Long iterations_;

        ::CORBA::String_var ID_;


      };

      class SUBTASK_EXEC_Export Subtask_Factory_exec_i
        : public virtual Subtask_Factory_Exec,
          public virtual TAO_Local_RefCounted_Object
      {
        public:
        Subtask_Factory_exec_i (void);
        virtual ~Subtask_Factory_exec_i (void);

        virtual ::Components::EnterpriseComponent_ptr
        create ();
      };

      extern "C" SUBTASK_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Subtask_Factory_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_SUBTASK_EXEC_H */

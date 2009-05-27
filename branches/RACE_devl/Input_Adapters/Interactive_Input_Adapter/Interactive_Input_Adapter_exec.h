// $Id$
#ifndef CIAO_INTERACTIVE_INPUT_ADAPTER_EXEC_H
#define CIAO_INTERACTIVE_INPUT_ADAPTER_EXEC_H

#include /**/ "ace/pre.h"

#include "Interactive_Input_AdapterEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Interactive_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"
#include "Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
    {
      namespace CIDL_Interactive_IA_Component_Impl
      {
        class INTERACTIVE_INPUT_ADAPTER_EXEC_Export
        Interactive_IA_Component_exec_i
          : public virtual Interactive_IA_Component_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Interactive_IA_Component_exec_i (void);

          virtual ~Interactive_IA_Component_exec_i (void);

          virtual ::CIAO::RACE::Input_Adapter::CCM_Admin_ptr
          get_admin ();

          virtual void
          set_session_context (
            ::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          private:
          ::CIAO::RACE::Input_Adapter::
          CCM_Interactive_IA_Component_Context_var context_;

          Logger logger_;

        };

        class INTERACTIVE_INPUT_ADAPTER_EXEC_Export
        Interactive_IA_Component_Home_exec_i
          : public virtual Interactive_IA_Component_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Interactive_IA_Component_Home_exec_i (void);

          virtual ~Interactive_IA_Component_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr
          create ();
        };

        extern "C" INTERACTIVE_INPUT_ADAPTER_EXEC_Export
        ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Input_Adapter_Interactive_IA_Component_Home_Impl
        (void);
      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_INTERACTIVE_INPUT_ADAPTER_EXEC_H */

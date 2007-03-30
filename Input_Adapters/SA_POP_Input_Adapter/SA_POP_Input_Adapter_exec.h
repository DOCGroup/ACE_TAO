#ifndef CIAO_SA_POP_INPUT_ADAPTER_EXEC_H
#define CIAO_SA_POP_INPUT_ADAPTER_EXEC_H

#include "SA_POP_Input_Adapter_svnt.h"
#include "SA_POP_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace SA_POP_IA
    {
      namespace CIDL_SA_POP_IA_Component_Impl
      {
        class SA_POP_INPUT_ADAPTER_EXEC_Export SA_POP_IA_Component_exec_i
          : public virtual SA_POP_IA_Component_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          SA_POP_IA_Component_exec_i (void);

          virtual ~SA_POP_IA_Component_exec_i (void);

          virtual ::CORBA::Boolean deploy_string (const char * uri);

          virtual ::CIAO::RACE::SA_POP_IA::CCM_Admin_ptr
          get_admin ();

          virtual void
          set_session_context
            (::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void ciao_postactivate ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          protected:
          SA_POP_IA_Component_Context *context_;
        };

        class SA_POP_INPUT_ADAPTER_EXEC_Export SA_POP_IA_Component_Home_exec_i
          : public virtual SA_POP_IA_Component_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          SA_POP_IA_Component_Home_exec_i (void);
          virtual ~SA_POP_IA_Component_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr
          create ();
        };

        extern "C" SA_POP_INPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_SA_POP_IA_SA_POP_IA_Component_Home_Impl (void);
      }
    }
  }
}

#endif /* CIAO_SA_POP_INPUT_ADAPTER_EXEC_H */

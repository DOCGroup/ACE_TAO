#ifndef CIAO_SA_POP_INPUT_ADAPTER_EXEC_H
#define CIAO_SA_POP_INPUT_ADAPTER_EXEC_H

#include "SA_POP_Input_Adapter_svnt.h"
#include "SA_POP_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"
#include "Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Input_Adapter
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

          virtual ::CIAO::RACE::Input_Adapter::CCM_Admin_ptr
          get_admin ();

          virtual void
          set_session_context (::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          protected:
          SA_POP_IA_Component_Context *context_;

          private:
          Logger logger_;
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
        create_CIAO_RACE_Input_Adapter_SA_POP_IA_Component_Home_Impl (void);
      }
    }
  }
}

#endif /* CIAO_SA_POP_INPUT_ADAPTER_EXEC_H */

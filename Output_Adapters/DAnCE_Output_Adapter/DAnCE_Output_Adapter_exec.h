#ifndef CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H
#define CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H

#include "DAnCE_Output_Adapter_svnt.h"
#include "DAnCE_Output_Adapter_exec_export.h"
#include "tao/LocalObject.h"
#include "utils/Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace DAnCE_OA
    {
      namespace CIDL_DAnCE_OA_Impl
      {
        class DANCE_OUTPUT_ADAPTER_EXEC_Export DAnCE_OA_Component_exec_i
          : public virtual CIAO_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Impl::DAnCE_OA_Component_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:

          DAnCE_OA_Component_exec_i (void);

          virtual ~DAnCE_OA_Component_exec_i (void);

          virtual ::CIAO::RACE::DAnCE_OA::CCM_Admin_ptr get_admin ();

          virtual void repoman_id (const char * id);

          virtual char * repoman_id ();

          virtual void set_session_context
            (::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          protected:
          CIAO_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Impl::DAnCE_OA_Component_Context *context_;

          private:

          CORBA::String_var repoman_id_;

          Logger logger_;


        };

        class DANCE_OUTPUT_ADAPTER_EXEC_Export DAnCE_OA_Component_Home_exec_i
          : public virtual CIAO_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Impl::DAnCE_OA_Component_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          DAnCE_OA_Component_Home_exec_i (void);

          virtual ~DAnCE_OA_Component_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr create ();
        };

        extern "C" DANCE_OUTPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_DAnCE_OA_DAnCE_OA_Component_Home_Impl (void);
      }
    }
  }
}

#endif /* CIAO_DANCE_OUTPUT_ADAPTER_EXEC_H */

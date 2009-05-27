#ifndef CIAO_CONDUCTOR_EXEC_H
#define CIAO_CONDUCTOR_EXEC_H

#include /**/ "ace/pre.h"

#include "ConductorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Conductor_exec_export.h"
#include "tao/LocalObject.h"
#include "Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Conductor
    {
      namespace CIDL_Conductor_Component_Impl
      {
        class CONDUCTOR_EXEC_Export Conductor_Component_exec_i
          : public virtual Conductor_Component_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Conductor_Component_exec_i (void);

          virtual ~Conductor_Component_exec_i (void);
          
          virtual ::CIAO::RACE::Conductor::CCM_Admin_ptr
          get_admin ();

          virtual void
          set_session_context (::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          private:
          ::CIAO::RACE::Conductor::CCM_Conductor_Component_Context_var
            context_;
          Logger logger_;

        };

        class CONDUCTOR_EXEC_Export Conductor_Component_Home_exec_i
          : public virtual Conductor_Component_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Conductor_Component_Home_exec_i (void);
          virtual ~Conductor_Component_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr
          create ();
        };

        extern "C" CONDUCTOR_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Conductor_Conductor_Component_Home_Impl (void);
      }
    }
  }
}
#include /**/ "ace/post.h"

#endif /* CIAO_CONDUCTOR_EXEC_H */

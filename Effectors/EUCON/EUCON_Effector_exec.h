// $Id$
#ifndef CIAO_EUCON_EFFECTOR_EXEC_H
#define CIAO_EUCON_EFFECTOR_EXEC_H

#include /**/ "ace/pre.h"

#include "EUCON_Effector_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EUCON_Effector_exec_export.h"
#include "tao/LocalObject.h"
#include "Logger.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Effector
    {
      namespace CIDL_EUCON_Effector_Impl
      {
        class EUCON_EFFECTOR_EXEC_Export EUCON_Effector_exec_i
          : public virtual EUCON_Effector_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          EUCON_Effector_exec_i (void);

          virtual ~EUCON_Effector_exec_i (void);

          virtual ::CIAO::RACE::Effector::CCM_ApplicationActuator_ptr
          get_appActuator ();

          virtual ::CIAO::RACE::Effector::CCM_SystemActuator_ptr
          get_sysActuator ();

          virtual void
          set_session_context (
            ::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          private:
          EUCON_Effector_Context *context_;

          Logger logger_;

        };

        class EUCON_EFFECTOR_EXEC_Export EUCON_Effector_Home_exec_i
          : public virtual EUCON_Effector_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          EUCON_Effector_Home_exec_i (void);
          virtual ~EUCON_Effector_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr
          create ();
        };

        extern "C" EUCON_EFFECTOR_EXEC_Export
        ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Effector_EUCON_Effector_Home_Impl (void);
      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_EUCON_EFFECTOR_EXEC_H */

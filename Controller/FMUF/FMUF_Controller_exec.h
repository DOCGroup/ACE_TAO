// $Id$

#ifndef CIAO_FMUF_CONTROLLER_EXEC_H
#define CIAO_FMUF_CONTROLLER_EXEC_H

#include /**/ "ace/pre.h"

#include "FMUF_ControllerEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "FMUF_Controller_exec_export.h"
#include "tao/LocalObject.h"
#include "Controller.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      namespace CIDL_FMUF_Controller_Impl
      {
        class FMUF_CONTROLLER_EXEC_Export FMUF_Controller_exec_i
          : public virtual FMUF_Controller_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          FMUF_Controller_exec_i (void);

          virtual ~FMUF_Controller_exec_i (void);

          // Supported or inherited operations.

          // Attribute operations.

          virtual ::CORBA::Long
          sampling_period ();

          virtual void
          sampling_period (::CORBA::Long sampling_period);

          // Port operations.

          virtual ::CIAO::RACE::Controller::CCM_Control_Operations_ptr
          get_operations ();

          virtual ::CIAO::RACE::Controller::CCM_Admin_ptr
          get_admin ();

          // Operations from Components::SessionComponent

          virtual void
          set_session_context (
            ::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          private:

          ::CIAO::RACE::Controller::CCM_FMUF_Controller_Context_var context_;

          Controller *controller_;

          ::CORBA::Long sampling_period_;
        };

        class FMUF_CONTROLLER_EXEC_Export FMUF_Controller_Home_exec_i
          : public virtual FMUF_Controller_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          FMUF_Controller_Home_exec_i (void);
          virtual ~FMUF_Controller_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr
          create ();
        };

        extern "C" FMUF_CONTROLLER_EXEC_Export
        ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Controller_FMUF_Controller_Home_Impl (void);
      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_FMUF_CONTROLLER_EXEC_H */

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
        virtual void start ();

        virtual void stop ();

        // Attribute operations.
        virtual ::CORBA::Double sampling_period ();

        // Port operations.

        // Operations from Components::SessionComponent

        virtual void set_session_context (::Components::SessionContext_ptr ctx);

        virtual void ciao_preactivate ();

        virtual void ciao_postactivate ();

        virtual void ccm_activate ();

        virtual void ccm_passivate ();

        virtual void ccm_remove ();

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

        virtual ::Components::EnterpriseComponent_ptr create ();
      };

      extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Controller_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_CONTROLLER_EXEC_H */

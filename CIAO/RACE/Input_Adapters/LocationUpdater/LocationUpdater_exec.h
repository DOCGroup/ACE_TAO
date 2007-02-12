// $Id$
//Author: Stoyan Paunov


#ifndef CIAO_LOCATIONUPDATER_EXEC_H
#define CIAO_LOCATIONUPDATER_EXEC_H

#include /**/ "ace/pre.h"

#include "LocationUpdater_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LocationUpdater_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_LocationUpdater_Impl
    {
      class LOCATIONUPDATER_EXEC_Export LocationUpdater_exec_i
      : public virtual LocationUpdater_Exec,
        public virtual TAO_Local_RefCounted_Object
      {
        public:
        LocationUpdater_exec_i (void);
        virtual ~LocationUpdater_exec_i (void);

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
        LocationUpdater_Context *context_;
      };

      class LOCATIONUPDATER_EXEC_Export LocationUpdater_Home_exec_i
      : public virtual LocationUpdater_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        LocationUpdater_Home_exec_i (void);
        virtual ~LocationUpdater_Home_exec_i (void);

        // Supported or inherited operations.

        // Home operations.

        // Factory and finder operations.

        // Attribute operations.

        // Implicit operations.

        virtual ::Components::EnterpriseComponent_ptr
        create ();
      };

      extern "C" LOCATIONUPDATER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_LocationUpdater_Home_Impl (void);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_LOCATIONUPDATER_EXEC_H */


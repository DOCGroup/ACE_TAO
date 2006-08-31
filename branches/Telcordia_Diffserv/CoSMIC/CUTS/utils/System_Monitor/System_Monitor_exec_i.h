// -*- C++ -*-

//=============================================================================
/**
 * @file    System_Monitor_exec_i.h
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CIAO_SYSTEM_MONITOR_EXEC_I_H_
#define _CIAO_SYSTEM_MONITOR_EXEC_I_H_

#include /**/ "ace/pre.h"

#include "System_Monitor_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "System_Monitor_exec_export.h"
#include "System_Monitor_Task.h"
#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CUTS
{
  namespace CIDL_System_Monitor_Impl
  {
    class SYSTEM_MONITOR_EXEC_Export System_Monitor_exec_i :
      public virtual System_Monitor_Exec,
      public virtual TAO_Local_RefCounted_Object,
      public ACE_Event_Handler
    {
    public:
      /// Constructor.
      System_Monitor_exec_i (void);

      /// Destructor.
      virtual ~System_Monitor_exec_i (void);

      virtual ::CORBA::Long
        timeout (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void
        timeout (::CORBA::Long ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void
        set_session_context (::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

      virtual void
        ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));

    protected:
      System_Monitor_Context *context_;

    private:
      int handle_timeout (const ACE_Time_Value & current_time,
                          const void * act);

      bool active_;

      System_Monitor_Task task_;

      long timer_;

      long timeout_;

      long path_id_;
    };

    class SYSTEM_MONITOR_EXEC_Export System_Monitor_Home_exec_i :
      public virtual System_Monitor_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      System_Monitor_Home_exec_i (void);

      virtual ~System_Monitor_Home_exec_i (void);

      virtual ::Components::EnterpriseComponent_ptr
        create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((
        ::CORBA::SystemException,
        ::Components::CCMException));
    };

    extern "C" SYSTEM_MONITOR_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CUTS_System_Monitor_Home_Impl (void);
  }
}

#include /**/ "ace/post.h"

#endif  // !defined _CIAO_SYSTEM_MONITOR_EXEC_I_H_

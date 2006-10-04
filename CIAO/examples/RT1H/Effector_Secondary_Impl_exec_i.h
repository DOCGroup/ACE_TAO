// -*- C++ -*-

#ifndef _EFFECTOR_SECONDARY_IMPL_EXEC_I_H_
#define _EFFECTOR_SECONDARY_IMPL_EXEC_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ace/pre.h"

#include "Effector_Secondary_Impl_svnt.h"
#include "Effector_Secondary_Impl_exec_export.h"

#include "cuts/config.h"
#include "cuts/CCM_CoWorkEr_T.h"
#include "cuts/CCM_Factory_T.h"
#include "cuts/Trigger_T.h"

#include "cuts/CPUWorker.h"
#include "cuts/MemoryWorker.h"

namespace CIDL_MonolithicImplementation
{
  class EFFECTOR_SECONDARY_IMPL_EXEC_Export Effector_Secondary :
  public CUTS_CCM_CoWorkEr_T <Effector_Secondary_Exec, Effector_Secondary_Context> 
  {
    public:
    // Type definition of this component
    typedef Effector_Secondary This_Component;

    // Type definition of the CoWorkEr type.
    typedef CUTS_CCM_CoWorkEr_T <
      Effector_Secondary_Exec,
      Effector_Secondary_Context> _coworker_type;

    // Default constructor.
    Effector_Secondary (void);

    // Destructor.
    virtual ~Effector_Secondary (void);

    // EventSink: command
    virtual void push_command (
      TSCE::Command_Event * ev
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
    // Worker: memory
    CUTS_Memory_Worker memory_;

    // Worker: cpu
    CUTS_CPU_Worker cpu_;

    // Event handler: command
    void push_command_handler (
      TSCE::Command_Event * ev, CUTS_Activation_Record * record);

    CUTS_Event_Handler_Manager_T <
      This_Component, TSCE::Command_Event> push_command_handler_;

    public:
    // SessionComponent: set_session_context
    virtual void set_session_context (
      ::Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException));

    // SessionComponent: ciao_preactivate
    virtual void ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException));

    // Environment: activate
    virtual void ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException));

    // SessionComponent: ciao_postactivate
    virtual void ciao_postactivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException));

    virtual void ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException));

    virtual void ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::CCMException));
  };

  class EFFECTOR_SECONDARY_IMPL_EXEC_Export Effector_Secondary_Factory :
    public CUTS_CCM_Factory_T <Effector_Secondary_Factory_Exec, Effector_Secondary> 
  {
    public:
    // Default constructor.
    Effector_Secondary_Factory (void);

    // Destructor.
    virtual ~Effector_Secondary_Factory (void);
  };

  CUTS_FACTORY_EXPORT_DECLARE (EFFECTOR_SECONDARY_IMPL_EXEC_Export,
                                create_TSCE_Effector_Secondary_Factory_Impl);
}

#include /**/ "ace/post.h"

#endif  // !defined _EFFECTOR_SECONDARY_IMPL_EXEC_I_H_

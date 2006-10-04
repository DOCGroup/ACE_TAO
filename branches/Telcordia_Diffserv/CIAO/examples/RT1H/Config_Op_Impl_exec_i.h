// -*- C++ -*-

#ifndef _CONFIG_OP_IMPL_EXEC_I_H_
#define _CONFIG_OP_IMPL_EXEC_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ace/pre.h"

#include "Config_Op_Impl_svnt.h"
#include "Config_Op_Impl_exec_export.h"

#include "cuts/config.h"
#include "cuts/CCM_CoWorkEr_T.h"
#include "cuts/CCM_Factory_T.h"
#include "cuts/Trigger_T.h"

#include "cuts/CPUWorker.h"
#include "cuts/MemoryWorker.h"

namespace CIDL_Config_Op
{
  class CONFIG_OP_IMPL_EXEC_Export Config_Op :
  public CUTS_CCM_CoWorkEr_T <Config_Op_Exec, Config_Op_Context> 
  {
    public:
    // Type definition of this component
    typedef Config_Op This_Component;

    // Type definition of the CoWorkEr type.
    typedef CUTS_CCM_CoWorkEr_T <
      Config_Op_Exec,
      Config_Op_Context> _coworker_type;

    // Default constructor.
    Config_Op (void);

    // Destructor.
    virtual ~Config_Op (void);

    // EventSink: status
    virtual void push_status (
      TSCE::Status_Event * ev
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    // EventSink: assessment
    virtual void push_assessment (
      TSCE::Assessment_Event * ev
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
    // Worker: memory
    CUTS_Memory_Worker memory_;

    // Worker: cpu
    CUTS_CPU_Worker cpu_;

    // Event handler: status
    void push_status_handler (
      TSCE::Status_Event * ev, CUTS_Activation_Record * record);

    CUTS_Event_Handler_Manager_T <
      This_Component, TSCE::Status_Event> push_status_handler_;

    // Event handler: assessment
    void push_assessment_handler (
      TSCE::Assessment_Event * ev, CUTS_Activation_Record * record);

    CUTS_Event_Handler_Manager_T <
      This_Component, TSCE::Assessment_Event> push_assessment_handler_;

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

  class CONFIG_OP_IMPL_EXEC_Export Config_Op_Factory :
    public CUTS_CCM_Factory_T <Config_Op_Factory_Exec, Config_Op> 
  {
    public:
    // Default constructor.
    Config_Op_Factory (void);

    // Destructor.
    virtual ~Config_Op_Factory (void);
  };

  CUTS_FACTORY_EXPORT_DECLARE (CONFIG_OP_IMPL_EXEC_Export,
                                create_TSCE_Config_Op_Factory_Impl);
}

#include /**/ "ace/post.h"

#endif  // !defined _CONFIG_OP_IMPL_EXEC_I_H_

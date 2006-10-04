// -*- C++ -*-

#ifndef _ENV_DECTECTOR_IMPL_EXEC_I_H_
#define _ENV_DECTECTOR_IMPL_EXEC_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ace/pre.h"

#include "Env_Dectector_Impl_svnt.h"
#include "Env_Dectector_Impl_exec_export.h"

#include "cuts/config.h"
#include "cuts/CCM_CoWorkEr_T.h"
#include "cuts/CCM_Factory_T.h"
#include "cuts/Trigger_T.h"

#include "cuts/CPUWorker.h"
#include "cuts/MemoryWorker.h"

namespace CIDL_MonolithicImplementation
{
  class ENV_DECTECTOR_IMPL_EXEC_Export Env_Detector :
  public CUTS_CCM_CoWorkEr_T <Env_Detector_Exec, Env_Detector_Context> 
  {
    public:
    // Type definition of this component
    typedef Env_Detector This_Component;

    // Type definition of the CoWorkEr type.
    typedef CUTS_CCM_CoWorkEr_T <
      Env_Detector_Exec,
      Env_Detector_Context> _coworker_type;

    // Default constructor.
    Env_Detector (void);

    // Destructor.
    virtual ~Env_Detector (void);

    // EventSink: command
    virtual void push_command (
      TSCE::Command_Event * ev
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
    // Worker: cpu
    CUTS_CPU_Worker cpu_;

    // Worker: memory
    CUTS_Memory_Worker memory_;

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

  class ENV_DECTECTOR_IMPL_EXEC_Export Env_Detector_Factory :
    public CUTS_CCM_Factory_T <Env_Detector_Factory_Exec, Env_Detector> 
  {
    public:
    // Default constructor.
    Env_Detector_Factory (void);

    // Destructor.
    virtual ~Env_Detector_Factory (void);
  };

  CUTS_FACTORY_EXPORT_DECLARE (ENV_DECTECTOR_IMPL_EXEC_Export,
                                create_TSCE_Env_Detector_Factory_Impl);
}

#include /**/ "ace/post.h"

#endif  // !defined _ENV_DECTECTOR_IMPL_EXEC_I_H_

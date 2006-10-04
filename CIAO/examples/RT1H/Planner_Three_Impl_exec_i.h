// -*- C++ -*-

#ifndef _PLANNER_THREE_IMPL_EXEC_I_H_
#define _PLANNER_THREE_IMPL_EXEC_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ace/pre.h"

#include "Planner_Three_Impl_svnt.h"
#include "Planner_Three_Impl_exec_export.h"

#include "cuts/config.h"
#include "cuts/CCM_CoWorkEr_T.h"
#include "cuts/CCM_Factory_T.h"
#include "cuts/Trigger_T.h"

#include "cuts/CPUWorker.h"
#include "cuts/MemoryWorker.h"

namespace CIDL_MonolithicImplementation
{
  class PLANNER_THREE_IMPL_EXEC_Export Planner_Three :
  public CUTS_CCM_CoWorkEr_T <Planner_Three_Exec, Planner_Three_Context> 
  {
    public:
    // Type definition of this component
    typedef Planner_Three This_Component;

    // Type definition of the CoWorkEr type.
    typedef CUTS_CCM_CoWorkEr_T <
      Planner_Three_Exec,
      Planner_Three_Context> _coworker_type;

    // Default constructor.
    Planner_Three (void);

    // Destructor.
    virtual ~Planner_Three (void);

    // EventSink: track
    virtual void push_track (
      TSCE::Track_Event * ev
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    // Periodic event: send_track_event
    void periodic_send_track_event (void);

    private:
    // Worker: memory
    CUTS_Memory_Worker memory_;

    // Worker: cpu
    CUTS_CPU_Worker cpu_;

    // Event handler: track
    void push_track_handler (
      TSCE::Track_Event * ev, CUTS_Activation_Record * record);

    CUTS_Event_Handler_Manager_T <
      This_Component, TSCE::Track_Event> push_track_handler_;

    // Periodic: send_track_event
    CUTS_Periodic_Trigger_T <This_Component> periodic_send_track_event_;

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

  class PLANNER_THREE_IMPL_EXEC_Export Planner_Three_Factory :
    public CUTS_CCM_Factory_T <Planner_Three_Factory_Exec, Planner_Three> 
  {
    public:
    // Default constructor.
    Planner_Three_Factory (void);

    // Destructor.
    virtual ~Planner_Three_Factory (void);
  };

  CUTS_FACTORY_EXPORT_DECLARE (PLANNER_THREE_IMPL_EXEC_Export,
                                create_TSCE_Planner_Three_Factory_Impl);
}

#include /**/ "ace/post.h"

#endif  // !defined _PLANNER_THREE_IMPL_EXEC_I_H_

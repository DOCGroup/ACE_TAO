// -*- C++ -*-

#ifndef _PLANNER_ONE_IMPL_EXEC_I_H_
#define _PLANNER_ONE_IMPL_EXEC_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ace/pre.h"

#include "Planner_One_Impl_svnt.h"
#include "Planner_One_Impl_exec_export.h"

#include "cuts/config.h"
#include "cuts/CCM_CoWorkEr_T.h"
#include "cuts/CCM_Factory_T.h"
#include "cuts/Trigger_T.h"

#include "cuts/CPUWorker.h"
#include "cuts/MemoryWorker.h"

namespace CIDL_MonolithicImplementation
{
  class PLANNER_ONE_IMPL_EXEC_Export Planner_One :
  public CUTS_CCM_CoWorkEr_T <Planner_One_Exec, Planner_One_Context> 
  {
    public:
    // Type definition of this component
    typedef Planner_One This_Component;

    // Type definition of the CoWorkEr type.
    typedef CUTS_CCM_CoWorkEr_T <
      Planner_One_Exec,
      Planner_One_Context> _coworker_type;

    // Default constructor.
    Planner_One (void);

    // Destructor.
    virtual ~Planner_One (void);

    // EventSink: situation
    virtual void push_situation (
      TSCE::Situation_Event * ev
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    private:
    // Worker: memory
    CUTS_Memory_Worker memory_;

    // Worker: cpu
    CUTS_CPU_Worker cpu_;

    // Event handler: situation
    void push_situation_handler (
      TSCE::Situation_Event * ev, CUTS_Activation_Record * record);

    CUTS_Event_Handler_Manager_T <
      This_Component, TSCE::Situation_Event> push_situation_handler_;

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

  class PLANNER_ONE_IMPL_EXEC_Export Planner_One_Factory :
    public CUTS_CCM_Factory_T <Planner_One_Factory_Exec, Planner_One> 
  {
    public:
    // Default constructor.
    Planner_One_Factory (void);

    // Destructor.
    virtual ~Planner_One_Factory (void);
  };

  CUTS_FACTORY_EXPORT_DECLARE (PLANNER_ONE_IMPL_EXEC_Export,
                                create_TSCE_Planner_One_Factory_Impl);
}

#include /**/ "ace/post.h"

#endif  // !defined _PLANNER_ONE_IMPL_EXEC_I_H_

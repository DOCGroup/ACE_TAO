// -*- C++ -*-
//
// $Id$

#ifndef CIAO_THREECOMP_MASTER_EXEC_H_
#define CIAO_THREECOMP_MASTER_EXEC_H_

#include "ThreeComp_MasterEC.h"
#include "ThreeComp_Master_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_ThreeComp_Master_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  /// Worker thread for asynchronous invocations
  class asynch_state_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_state_generator (::ThreeComp::CCM_Master_Context_ptr context);

    virtual int svc (void);

  private:
    ::ThreeComp::CCM_Master_Context_var context_;
  };

  /// Worker thread for synchronous invocations
  class synch_state_generator : public virtual ACE_Task_Base
  {
  public:
    synch_state_generator (::ThreeComp::CCM_Master_Context_ptr context);
    virtual int svc (void);

  private:
    ::ThreeComp::CCM_Master_Context_var context_;
  };

  class State_callback_exec_i
    : public virtual ::ThreeComp::CCM_AMI4CCM_StateReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    State_callback_exec_i (void);
    virtual ~State_callback_exec_i (void);

    virtual void bar (::CORBA::Long ret, const char * answer);
    virtual void bar_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
  };

  class Master_exec_i
    : public virtual Master_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Master_exec_i (void);
    virtual ~Master_exec_i (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::ThreeComp::CCM_Master_Context_var context_;
  };

  extern "C" THREECOMP_MASTER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ThreeComp_AMI_Master_Impl (void);
}

#endif /* ifndef */

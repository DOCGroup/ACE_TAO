//$Id$:
//============================================================
/**
 * @file Receiver_exec.h
 *
 * Header file for the Executor implementation.
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */
//============================================================

#ifndef RECEIVER_EXEC_H
#define RECEIVER_EXEC_H

#include "ReceiverEIC.h"
#include "tao/LocalObject.h"

#ifdef linux
//#define RECEIVER_EXEC_Export
#endif

namespace MyImpl
{

  /**
   * @class Receiver_exec_i
   *
   * Receiver executor implementation class.
   */

  class RECEIVER_EXEC_Export Receiver_exec_i :
    public virtual Hello::Receiver_Exec,
	  public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    Receiver_exec_i ();

    /// Default destructor.
    ~Receiver_exec_i ();

    // Operation which will be called upon receiving the timeout event.
    virtual void
    push_click_in (Hello::timeout *ev
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:
    /// Copmponent specific context
    Hello::CCM_Receiver_Context_var context_;
  private:
    CORBA::String_var message_;
  };

  /**
   * @class ReceiverHome_exec_i
   *
   * Receiver home executor implementation class.
   */
  class RECEIVER_EXEC_Export ReceiverHome_exec_i :
    public virtual Hello::CCM_ReceiverHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    ReceiverHome_exec_i ();

    /// Default dtor.
    ~ReceiverHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
createReceiverHome_Impl (void);

#endif /* RECEIVER_EXEC_H */

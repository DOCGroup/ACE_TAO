//$Id$:
//============================================================
/**
 * @file Receiver_exec.h
 *
 * Header file for the Executor implementation.
 */
//============================================================

#ifndef RECEIVER_EXEC_H
#define RECEIVER_EXEC_H

#include "ciao/CIAO_SwapExecC.h"
#include "ReceiverEC.h"
#include "Receiver_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Receiver_Impl
{

  /**
   * @class Receiver_exec_i
   *
   * Receiver executor implementation class.
   */

  class RECEIVER_EXEC_Export ReceiverSwap_exec_i :
      public virtual CIAO::Swap_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    ReceiverSwap_exec_i ();

    ~ReceiverSwap_exec_i ();

    virtual ::Components::EnterpriseComponent_ptr
    incarnate ();

    virtual ::Components::EnterpriseComponent_ptr
    etherealize ();
  };

  class RECEIVER_EXEC_Export Receiver_exec_i :
          public virtual Receiver_Exec,
          public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    Receiver_exec_i ();

    /// Default destructor.
    ~Receiver_exec_i ();

    // Operation which will be called upon receiving the timeout event.
    virtual void
    push_click_in (Hello::TimeOut *ev);

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

  protected:
    /// Copmponent specific context
    Receiver_Exec_Context_var context_;
  private:
    CORBA::String_var message_;
  };

  /**
   * @class ReceiverHome_exec_i
   *
   * Receiver home executor implementation class.
   */
  class RECEIVER_EXEC_Export ReceiverHome_exec_i :
    public virtual ReceiverHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    ReceiverHome_exec_i ();

    /// Default dtor.
    ~ReceiverHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createReceiverHome_Impl (void);
}

#endif /* RECEIVER_EXEC_H */



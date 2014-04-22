// $Id$

//============================================================
/**
 * @file EC_exec.h
 *
 * Header file for the actualy EC and ECHome component
 * implementation.
 *
 * @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//============================================================

#ifndef EC_EXEC_H
#define EC_EXEC_H

#include "ECEC.h"
#include "tao/LocalObject.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"
#include "EC_exec_export.h"

namespace MyImpl
{
  // Forward decl.
  class EC_exec_i;

  /**
   * @brief Active pulse generater
   */
  class timeout_Handler : public ACE_Task_Base
  {
  public:
    // Default constructor
    timeout_Handler (EC_exec_i *cb);
    ~timeout_Handler ();

    int open_h (void);

    int close_h (void);

    int start (CORBA::Long hertz);

    int stop (void);

    int active (void);

    // Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

    // Called when <Time_Handler> is removed.
    virtual int handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask);

    virtual int svc (void);

  private:
    /// Tracking whether we are actively generating pulse or not.
    long active_;

    /// Flag to indicate completion of this active object.
    int done_;

    /// The timer id we are waiting.
    long tid_;

    EC_exec_i *pulse_callback_;

    ACE_Thread_Manager thr_mgr_;
  };

  extern "C" EC_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_BasicSP_EC_Impl (void);

  /**
   * @class EC_exec_i
   *
   * EC executor implementation class.
   */
  class EC_EXEC_Export EC_exec_i :
    public virtual CIAO_BasicSP_EC_Impl::EC_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default constructor.
    EC_exec_i ();

    /// Initialize with a default frequency.
    EC_exec_i (CORBA::Long hz);

    /// Default destructor.
    ~EC_exec_i ();

    // Attribute operations.

    virtual CORBA::Long hertz ();

    virtual void hertz (CORBA::Long hertz);

    // Operations from supported interface(s)

    virtual void start ();

    virtual void stop ();

    virtual CORBA::Boolean active ();

    // Operations from Components::SessionComponent

    virtual void set_session_context (Components::SessionContext_ptr ctx);

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    /// Helper function to be called back by timeout_Handler
    void pulse (void);

  protected:
    /// Frequency
    CORBA::Long hertz_;

    /// Copmponent specific context
    BasicSP::CCM_EC_Context_var context_;

    /// An active object that actually trigger the generation of
    /// periodic events.
    timeout_Handler pulser_;
  };

  /**
   * @class ECHome_exec_i
   *
   * EC home executor implementation class.
   */
  class EC_EXEC_Export ECHome_exec_i :
    public virtual ::CIAO_BasicSP_EC_Impl::ECHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    ECHome_exec_i ();

    /// Default dtor.
    ~ECHome_exec_i ();

    // Explicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    new_EC (CORBA::Long hertz);

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

}

extern "C" EC_EXEC_Export ::Components::HomeExecutorBase_ptr
createECHome_Impl (void);

#endif /* CIAO_ECGEN_EXEC_H */

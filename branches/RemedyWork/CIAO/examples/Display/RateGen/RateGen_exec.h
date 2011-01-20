// $Id$

/**
 * @file RateGen_exec.h
 *
 * Header file for the actualy RateGen and RateGenHome component
 * implementation.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef RATEGEN_EXEC_H
#define RATEGEN_EXEC_H

#include "RateGen_exec_export.h"
#include "RateGenEC.h"
#include "tao/LocalObject.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"

namespace MyImpl
{
  // Forward decl.
  class RateGen_exec_i;

  /**
   * @brief Active pulse generater
   */
  class Pulse_Handler : public ACE_Task_Base
  {
  public:
    // Default constructor
    Pulse_Handler (RateGen_exec_i *cb);
    ~Pulse_Handler ();

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

    RateGen_exec_i *pulse_callback_;

    ACE_Thread_Manager thr_mgr_;
  };

  /**
   * @class RateGen_exec_i
   *
   * RateGen executor implementation class.
   */
  class RATEGEN_EXEC_Export RateGen_exec_i :
    public virtual ::CIAO_HUDisplay_RateGen_Impl::RateGen_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default constructor.
    RateGen_exec_i ();

    /// Initialize with a default frequency.
    RateGen_exec_i (CORBA::Long hz);

    /// Default destructor.
    ~RateGen_exec_i ();

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

    /// Helper function to be called back by Pulse_Handler
    void pulse (void);

  protected:
    /// Frequency
    CORBA::Long hertz_;

    /// Copmponent specific context
    HUDisplay::CCM_RateGen_Context_var context_;

    /// An active object that actually trigger the generation of
    /// periodic events.
    Pulse_Handler pulser_;
  };

  /**
   * @class RateGenHome_exec_i
   *
   * RateGen home executor implementation class.
   */
  class RATEGEN_EXEC_Export RateGenHome_exec_i :
    public virtual HUDisplay::CCM_RateGenHome,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    RateGenHome_exec_i ();

    /// Default dtor.
    ~RateGenHome_exec_i ();

    // Explicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    new_RateGen (CORBA::Long hertz);

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

}

extern "C" RATEGEN_EXEC_Export ::Components::HomeExecutorBase_ptr
createRateGenHome_Impl (void);

#endif /* RATEGEN_EXEC_H */

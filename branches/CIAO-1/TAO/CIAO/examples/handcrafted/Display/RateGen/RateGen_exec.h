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

#include "RateGenEIC.h"
#include "tao/LocalObject.h"
#include "ace/Thread_Manager.h"
#include "ace/Event_Handler.h"

namespace MyImpl
{
  // Forward decl.
  class RateGen_exec_impl;

  /**
   * @brief Active pulse generater
   */
  class Pulse_Handler : public ACE_Event_Handler
  {
  public:
    // Default constructor
    Pulse_Handler (RateGen_exec_impl *cb);
    ~Pulse_Handler ();

    int open (void);

    int close (void);

    int start (CORBA::Long hertz);

    int stop (void);

    int active (void);

    // Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

    // Called when <Time_Handler> is removed.
    virtual int handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask);

    static ACE_THR_FUNC_RETURN svc_run (void *args);

  private:
    long active_;

    long count_;

    int done_;

    int tid_;

    RateGen_exec_impl *pulse_callback_;

    ACE_Thread_Manager thr_mgr_;
  };

  /**
   * @class RateGen_exec_impl
   *
   * RateGen executor implementation class.
   */
  class RATEGEN_EXEC_Export RateGen_exec_impl :
    public virtual HUDisplay::RateGen_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    RateGen_exec_impl ();

    /// Initialize with a default frequency.
    RateGen_exec_impl (CORBA::Long hz);

    /// Default destructor.
    ~RateGen_exec_impl ();

    // Attribute operations.

    virtual CORBA::Long hertz (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void hertz (CORBA::Long hertz
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from supported interface(s)

    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void stop (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Boolean active (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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

    /// Helper function to be called back by Pulse_Handler
    void pulse (void);

  protected:
    /// Frequency
    CORBA::Long hertz_;

    /// Copmponent specific context
    HUDisplay::CCM_RateGen_Context_var context_;

    ///
    Pulse_Handler pulser_;
  };

  /**
   * @class RateGenHome_exec_impl
   *
   * RateGen home executor implementation class.
   */
  class RATEGEN_EXEC_Export RateGenHome_exec_impl :
    public virtual HUDisplay::CCM_RateGenHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    RateGenHome_exec_impl ();

    /// Default dtor.
    ~RateGenHome_exec_impl ();

    // Explicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    new_RateGen (CORBA::Long hertz
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));
  };

}

extern "C" RATEGEN_EXEC_Export ::Components::HomeExecutorBase_ptr
createRateGenHome_Impl (void);

#endif /* RATEGEN_EXEC_H */

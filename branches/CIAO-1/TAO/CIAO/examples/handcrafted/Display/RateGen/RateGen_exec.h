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

#include "RateGen_ImplC.h"
#include "tao/LocalObject.h"

namespace MyImpl
{
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

  protected:
    /// status
    int active_;

    /// Frequency
    CORBA::Long hertz_;

    /// Copmponent specific context
    HUDisplay::CCM_RateGen_Context_var context_;
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

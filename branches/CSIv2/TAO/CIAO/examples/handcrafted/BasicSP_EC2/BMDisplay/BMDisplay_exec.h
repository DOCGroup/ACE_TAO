// $Id$

//==============================================================
/**
 * @file BMDisplay_exec.h
 *
 * Header file for the actual BMDisplay and BMDisplayHome component
 * implementations.
 *
 * @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//==============================================================
#ifndef CIAO_BMDISPLAY_EXEC_H
#define CIAO_BMDISPLAY_EXEC_H

#include "BMDisplayEIC.h"
#include "tao/LocalObject.h"

namespace MyImpl
{
  /**
   * @class BMDisplay_exec_impl
   *
   * RateGen executor implementation class.
   */
  class BMDISPLAY_EXEC_Export BMDisplay_exec_impl :
    public virtual BasicSP::BMDisplay_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    BMDisplay_exec_impl ();

    /// Default destructor.
    ~BMDisplay_exec_impl ();

    // Operations from BasicSP::BMDisplay

    virtual void
    push_data_ready (BasicSP::DataAvailable *ev
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  protected:
    /// Copmponent specific context
    BasicSP::CCM_BMDisplay_Context_var context_;
  };

  /**
   * @class BMDisplayHome_exec_impl
   *
   * BMDisplay home executor implementation class.
   */
  class BMDISPLAY_EXEC_Export BMDisplayHome_exec_impl :
    public virtual BasicSP::CCM_BMDisplayHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    BMDisplayHome_exec_impl ();

    /// Default dtor.
    ~BMDisplayHome_exec_impl ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" BMDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDisplayHome_Impl (void);

#endif /* BMDISPLAY_EXEC_H */

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

#include "BMDisplayEC.h"
#include "tao/LocalObject.h"
#include "BMDisplay_exec_export.h"

namespace MyImpl
{
  /**
   * @class BMDisplay_exec_i
   *
   * RateGen executor implementation class.
   */
  class BMDISPLAY_EXEC_Export BMDisplay_exec_i :
    public virtual CIAO_BasicSP_BMDisplay_Impl::BMDisplay_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default constructor.
    BMDisplay_exec_i ();

    /// Default destructor.
    ~BMDisplay_exec_i ();

    // Operations from BasicSP::BMDisplay

    virtual void
    push_data_ready (BasicSP::DataAvailable *ev);

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx);

    virtual void
    configuration_complete ();

    virtual void
    ccm_activate ();

    virtual void
    ccm_passivate ();

    virtual void
    ccm_remove ();
  protected:
    /// Copmponent specific context
    BasicSP::CCM_BMDisplay_Context_var context_;
  };

  extern "C" BMDISPLAY_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_BasicSP_BMDisplay_Impl (void);

  /**
   * @class BMDisplayHome_exec_impl
   *
   * BMDisplay home executor implementation class.
   */
  class BMDISPLAY_EXEC_Export BMDisplayHome_exec_impl :
    public virtual CIAO_BasicSP_BMDisplay_Impl::BMDisplayHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    BMDisplayHome_exec_impl ();

    /// Default dtor.
    ~BMDisplayHome_exec_impl ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

}

extern "C" BMDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDisplayHome_Impl (void);

#endif /* BMDISPLAY_EXEC_H */

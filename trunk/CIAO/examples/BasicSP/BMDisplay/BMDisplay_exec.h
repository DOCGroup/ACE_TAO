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
#include "BMDisplay_exec_export.h"

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
    push_data_ready (BasicSP::DataAvailable *ev);

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx);

    virtual void
    ciao_preactivate ();

    virtual void
    ccm_activate ();

    virtual void
    ciao_postactivate ();

    virtual void
    ccm_passivate ();

    virtual void
    ccm_remove ();
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
    create ();
  };

}

extern "C" BMDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDisplayHome_Impl (void);

#endif /* BMDISPLAY_EXEC_H */

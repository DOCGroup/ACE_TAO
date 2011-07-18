// $Id$

//================================================================
/**
 * @file BMClosedED_exec.h
 *
 * @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_BMCLOSED_EXEC_H
#define CIAO_BMCLOSED_EXEC_H

#include "BMClosedEDEC.h"
#include "tao/LocalObject.h"
#include "BMClosedED_exec_export.h"
#include "BasicSP_exec.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class BMClosedED_exec_i
   *
   * An example RateGen executor implementation class.
   */
  class BMCLOSEDED_EXEC_Export BMClosedED_exec_i :
    public virtual CIDL_BMClosedED_Impl::BMClosedED_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default constructor.
    BMClosedED_exec_i (void);

    /// Default destructor.
    ~BMClosedED_exec_i (void);

    // Operations from HUDisplay::BMClosedED

    virtual BasicSP::CCM_ReadData_ptr
    get_dataout ();

    virtual void
    push_in_avail (BasicSP::DataAvailable *ev);

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
    CORBA::String_var str_;

    MyImpl::ReadData_Impl* dataout_;

    /// Copmponent specific context
    BasicSP::CCM_BMClosedED_Context_var context_;
  };

  extern "C" BMCLOSEDED_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_BasicSP_BMClosedED_Impl (void);

  /**
   * @class BMClosedEDHome_exec_i
   *
   * BMClosedED home executor implementation class.
   */
  class BMCLOSEDED_EXEC_Export BMClosedEDHome_exec_i :
    public virtual CIDL_BMClosedED_Impl::BMClosedEDHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    BMClosedEDHome_exec_i ();

    /// Default dtor.
    ~BMClosedEDHome_exec_i ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" BMCLOSEDED_EXEC_Export ::Components::HomeExecutorBase_ptr
create_BasicSP_BMClosedEDHome_Impl (void);

#endif /* CIAO_BMCLOSED_EXEC_H*/

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

#include "BMClosedEDEIC.h"
#include "tao/LocalObject.h"

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
    public virtual BasicSP::BMClosedED_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    BMClosedED_exec_i (void);

    /// Default destructor.
    ~BMClosedED_exec_i (void);

    // Operations from HUDisplay::BMClosedED

    virtual BasicSP::CCM_ReadData_ptr
    get_dataout (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    push_in_avail (BasicSP::DataAvailable *ev
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from HUDisplay::position
    virtual char *
    get_data (ACE_ENV_SINGLE_ARG_DECL)
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
    CORBA::String_var str_;

    /// Copmponent specific context
    BasicSP::CCM_BMClosedED_Context_var context_;
  };

  /**
   * @class BMClosedEDHome_exec_i
   *
   * BMClosedED home executor implementation class.
   */
  class BMCLOSEDED_EXEC_Export BMClosedEDHome_exec_i :
    public virtual BasicSP::CCM_BMClosedEDHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    BMClosedEDHome_exec_i ();

    /// Default dtor.
    ~BMClosedEDHome_exec_i ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" BMCLOSEDED_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMClosedEDHome_Impl (void);

#endif /* CIAO_BMCLOSED_EXEC_H*/

// $Id$

//============================================================
/**
 * @file RoundTrip_exec.h
 *
 * Header file for the Executor implementation.
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//============================================================

#ifndef NODEAPP_ROUNDTRIP_EXEC_H
#define NODEAPP_ROUNDTRIP_EXEC_H

#include "RoundTripEIC.h"
#include "tao/LocalObject.h"

namespace MyImpl
{

  /**
   * @class RoundTrip_exec_i
   *
   * RoundTrip executor implementation class.
   */

  class NODEAPPTEST_ROUNDTRIP_EXEC_Export RoundTrip_exec_i :
    public virtual NodeAppTest::RoundTrip_Exec,
    public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    RoundTrip_exec_i ();

    /// Default destructor.
      ~RoundTrip_exec_i ();

    /// Operation to test the data
      virtual CORBA::Long cube_long (CORBA::Long data)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::NodeAppTest::CCM_LatencyTest*
                        get_latency (ACE_ENV_SINGLE_ARG_DECL)
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

    /// Helper function to be called back by timeout_Handler
    void pulse (void);

    // CIAO defined methods
    virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:
    /// Copmponent specific context
    NodeAppTest::CCM_NodeAppTest_RoundTrip_Context_var context_;
  };

  /**
   * @class RoundTripHome_exec_i
   *
   * RoundTrip home executor implementation class.
   */
  class NODEAPPTEST_ROUNDTRIP_EXEC_Export RoundTripHome_exec_i :
    public virtual NodeAppTest::CCM_NodeAppTest_RoundTripHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    RoundTripHome_exec_i ();

    /// Default dtor.
    ~RoundTripHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" NODEAPPTEST_ROUNDTRIP_EXEC_Export
::Components::HomeExecutorBase_ptr createRoundTripHome_Impl (void);

#endif /* NODEAPP_ROUNDTRIPGEN_EXEC_H */

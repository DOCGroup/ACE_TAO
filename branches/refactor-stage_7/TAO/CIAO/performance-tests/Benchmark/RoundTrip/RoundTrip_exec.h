// $Id$

//============================================================
/**
 * @file Test_exec.h
 *
 * Header file for the Executor implementation.
 *
 * @author Diego Sevilla <dsevilla@um.es> 
*  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//============================================================

#ifndef ROUNDTRIP_EXEC_H
#define ROUNDTRIP_EXEC_H

#include "RoundTripEIC.h"
#include "tao/LocalObject.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"

namespace MyImpl
{

  /**
   * @class RoundTrip_exec_i
   *
   * RoundTrip executor implementation class.
   */

  class ROUNDTRIP_EXEC_Export RoundTrip_exec_i :
    public virtual Benchmark::RoundTrip_Exec,
    public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    RoundTrip_exec_i ();

    /// Default destructor.
    ~RoundTrip_exec_i ();

    /// Operation to test the data
    virtual CORBA::Long 
      makeCall (CORBA::Long data)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Benchmark::CCM_LatencyTest*
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

  protected:
    /// Copmponent specific context
    Benchmark::CCM_RoundTrip_Context_var context_;

  };

  /**
   * @class RoundTripHome_exec_i
   *
   * RoundTrip home executor implementation class.
   */
  class ROUNDTRIP_EXEC_Export RoundTripHome_exec_i :
    public virtual Benchmark::CCM_RoundTripHome,
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

extern "C" ROUNDTRIP_EXEC_Export ::Components::HomeExecutorBase_ptr
createRoundTripHome_Impl (void);

#endif /* CIAO_ROUNDTRIPGEN_EXEC_H */

// $Id$

//============================================================
/**
 * @file RoundTripClient_exec.h
 *
 * Header file for the Executor implementation.
 *
 * @author Diego <dsevilla@um.es>
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//============================================================

#ifndef BENCHMARK_ROUNDTRIPCLIENT_EXEC_H
#define BENCHMARK_ROUNDTRIPCLIENT_EXEC_H

#include "RoundTripClientEIC.h"
#include "tao/LocalObject.h"


namespace MyImpl
{

  /**
   * @class RoundTripClient_exec_i
   *
   * RoundTripClient executor implementation class.
   */

  class ROUNDTRIPCLIENT_EXEC_Export RoundTripClient_exec_i :
    public virtual Benchmark::RoundTripClient_Exec,
    public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    RoundTripClient_exec_i ();

    /// Default destructor.
    ~RoundTripClient_exec_i ();

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual ::Benchmark::CCM_Controller_ptr
                        get_controller (ACE_ENV_SINGLE_ARG_DECL)
                ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void start (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));


  protected:
    /// Copmponent specific context
    Benchmark::CCM_RoundTripClient_Context_var context_;

  };

  /**
   * @class RoundTripClientHome_exec_i
   *
   * RoundTripClient home executor implementation class.
   */
  class ROUNDTRIPCLIENT_EXEC_Export RoundTripClientHome_exec_i :
    public virtual Benchmark::CCM_RoundTripClientHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    RoundTripClientHome_exec_i ();

    /// Default dtor.
    ~RoundTripClientHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" ROUNDTRIPCLIENT_EXEC_Export ::Components::HomeExecutorBase_ptr
createRoundTripClientHome_Impl (void);

#endif /* CIAO_ROUNDTRIPCLIENTGEN_EXEC_H */

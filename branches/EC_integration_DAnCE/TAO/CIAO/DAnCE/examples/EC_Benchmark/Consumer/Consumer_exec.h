// $Id$

// ================================================================
/**
 * @file Consumer_exec.h
 *
 * Header file for the actual Consumer and ConsumerHome component
 * implementations. 
 *
 * @author Gan Deng <gan.deng@vanderbilt.edu>
 */
// ================================================================

#ifndef CIAO_CONSUMER_EXEC_H
#define CIAO_CONSUMER_EXEC_H

#include "ConsumerEC.h"
#include "Consumer_exec_export.h"

#include "tao/LocalObject.h"
#include "ace/High_Res_Timer.h"

const int niterations = 100;

namespace Consumer_Impl
{
  /**
   * @class Consumer_exec_i
   *
   * An example RateGen executor implementation class.
   */
  class CONSUMER_EXEC_Export Consumer_exec_i :
    public virtual Consumer_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Consumer_exec_i ();

    /// Default destructor.
    ~Consumer_exec_i ();

    // Operations from EC_Benchmark::Consumer

    virtual void
    push_timeout (EC_Benchmark::TimeOut *ev
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
    virtual void
    ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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
    Consumer_Exec_Context_var context_;

    CORBA::ULong event_count_;
    // Keep track of the number of events received.

    ACE_hrtime_t end_time_[niterations];
  };

  /**
   * @class ConsumerHome_exec_i
   *
   * Consumer home executor implementation class.
   */
  class CONSUMER_EXEC_Export ConsumerHome_exec_i :
    public virtual ConsumerHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    ConsumerHome_exec_i ();

    /// Default dtor.
    virtual ~ConsumerHome_exec_i ();

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
extern "C" CONSUMER_EXEC_Export ::Components::HomeExecutorBase_ptr
createConsumerHome_Impl (void);

#endif /* CIAO_CONSUMER_EXEC_H*/

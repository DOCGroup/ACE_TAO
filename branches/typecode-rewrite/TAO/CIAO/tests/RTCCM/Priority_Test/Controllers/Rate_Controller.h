// $Id$

/**
 * @file Rate_Controller.h
 *
 * Header file for the Rate Controller and Rate ControllerHome
 * component implementations.  These controller trigger a remote
 * worker at some rate to do some amount of work.
 *
 * We should make "hertz" a configurable parameter using a property
 * file, but, unfortunately, CIAO doesn't support this feature when
 * this executor at this time.  Therefore, we use different entry
 * points for the DLL to control the rate of the Rate_Controller.
 *
 * define macro: COLLECT_AND_DUMP_SAMPLE_HISTORY to collect and dump
 * sample history.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef RATE_CONTROLLER_H
#define RATE_CONTROLLER_H

#include "ControllerEIC.h"
#include "tao/LocalObject.h"
#include "Pulser.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class Rate_Controller_i
   *
   * A generic controller implementation.
   */
  class CONTROLLER_EXEC_Export Rate_Controller_i :
    public virtual PTImpl::Pulse_Handler,
    public virtual Priority_Test::Controller_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Rate_Controller_i (CORBA::Long hertz);

    /// Default destructor.
    ~Rate_Controller_i ();

    // Operations from Priority_Test::Controller

    virtual void
    start (CORBA::Long arg
           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    stop (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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

    /// Callback
    virtual void
    pulse (void);

  protected:
    const CORBA::Long rate_;

    CORBA::Long work_;

    /// Copmponent specific context
    Priority_Test::CCM_Controller_Context_var context_;

    /// Pulser generate periodical events
    PTImpl::Pulser pulser_;

    // ------ Statistic related variables --------

    /// Amount of memory to allocate for performance samples.
    static const int sample_time_;     // in minute

    /// Mark the time when the "start method" is called.
    ACE_hrtime_t start_time_;

#if defined (COLLECT_AND_DUMP_SAMPLE_HISTORY)
    typedef ACE_Sample_History SAMPLER;
#else
    typedef ACE_Basic_Stats SAMPLER;
#endif /* COLLECT_AND_DUMP_SAMPLE_HISTORY */
    SAMPLER *sampler_;
  };

  /**
   * @class Controller_exec_i
   *
   * CB_Worker home executor implementation class.
   */
  class CONTROLLER_EXEC_Export Rate_Controller_Home_i :
    public virtual Priority_Test::CCM_ControllerHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    Rate_Controller_Home_i (CORBA::Long hertz);

    /// Default dtor.
    ~Rate_Controller_Home_i ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  private:
    CORBA::Long hertz_;
  };

}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome25_Impl (void);

extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome50_Impl (void);

extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome75_Impl (void);

#endif /* RATE_CONTROLLER_H */

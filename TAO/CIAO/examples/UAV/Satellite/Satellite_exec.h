// $Id$

//================================================================
/**
 * @file Satellite_exec.h
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_SATELLITE_EXEC_H
#define CIAO_SATELLITE_EXEC_H

#include "SatelliteEIC.h"
#include "tao/LocalObject.h"
#include "ace/High_Res_Timer.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class Satellite_exec_i
   *
   * The Satellite implementation class.
   */
  class SATELLITE_EXEC_Export Satellite_exec_i :
    public virtual BBN_UAV::Satellite_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Satellite_exec_i (void);

    /// Default destructor.
    ~Satellite_exec_i (void);

    // Operations from event sink(s)

    virtual void
    push_uav_ready (BBN_UAV::UAVReady *ev
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    push_ucav_ready (BBN_UAV::UCAVReady *ev
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    push_battle_ready (BBN_UAV::BattleReady *ev
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from supported interface(s)

    virtual void alert (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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
    BBN_UAV::CCM_Satellite_Context_var context_;

    ACE_hrtime_t start_;

    ACE_hrtime_t UAV_times_[3000];

    ACE_hrtime_t UCAV_times_[3000];

    ACE_hrtime_t Battle_times_[1000];

    int counter_;
  };

  /**
   * @class SatelliteHome_exec_i
   *
   * Satellite home executor implementation class.
   */
  class SATELLITE_EXEC_Export SatelliteHome_exec_i :
    public virtual BBN_UAV::CCM_SatelliteHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    SatelliteHome_exec_i ();

    /// Default dtor.
    ~SatelliteHome_exec_i ();

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
extern "C" SATELLITE_EXEC_Export ::Components::HomeExecutorBase_ptr
createSatelliteHome_Impl (void);

#endif /* CIAO_SATELLITE_EXEC_H*/

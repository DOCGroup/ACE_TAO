// $Id$

//================================================================
/**
 * @file UCAV_exec.h
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_UCAV_EXEC_H
#define CIAO_UCAV_EXEC_H

#include "UCAVEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class UCAV_exec_i
   *
   * The UCAV implementation class.
   */
  class UCAV_EXEC_Export UCAV_exec_i :
    public virtual BBN_UAV::UCAV_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    UCAV_exec_i (void);

    /// Default destructor.
    ~UCAV_exec_i (void);

    // Operations from BBN_UAV::UCAV

    virtual void
    push_start_move (BBN_UAV::StartMove *ev
                         ACE_ENV_ARG_DECL)
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
    BBN_UAV::CCM_UCAV_Context_var context_;
  };

  /**
   * @class UCAVHome_exec_i
   *
   * UCAV home executor implementation class.
   */
  class UCAV_EXEC_Export UCAVHome_exec_i :
    public virtual BBN_UAV::CCM_UCAVHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    UCAVHome_exec_i ();

    /// Default dtor.
    ~UCAVHome_exec_i ();

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
extern "C" UCAV_EXEC_Export ::Components::HomeExecutorBase_ptr
createUCAVHome_Impl (void);

#endif /* CIAO_UCAV_EXEC_H*/

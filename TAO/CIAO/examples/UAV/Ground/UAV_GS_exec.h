// $Id$

//================================================================
/**
 * @file UAV_GS_exec.h
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_UAV_GS_EXEC_H
#define CIAO_UAV_GS_EXEC_H

#include "UAV_GSEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class UAV_GS_exec_i
   *
   * The UAV_GS implementation class.
   */
  class UAV_GS_EXEC_Export UAV_GS_exec_i :
    public virtual BBN_UAV::UAV_GS_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    UAV_GS_exec_i (void);

    /// Default destructor.
    ~UAV_GS_exec_i (void);

    // Operations from BBN_UAV::UAV_GS

    virtual void
    push_prepare_capture (BBN_UAV::PrepareCapture *ev
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
    BBN_UAV::CCM_UAV_GS_Context_var context_;
  };

  /**
   * @class UAV_GSHome_exec_i
   *
   * UAV_GS home executor implementation class.
   */
  class UAV_GS_EXEC_Export UAV_GSHome_exec_i :
    public virtual BBN_UAV::CCM_UAV_GSHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    UAV_GSHome_exec_i ();

    /// Default dtor.
    ~UAV_GSHome_exec_i ();

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
extern "C" UAV_GS_EXEC_Export ::Components::HomeExecutorBase_ptr
createUAV_GSHome_Impl (void);

#endif /* CIAO_UAV_GS_EXEC_H*/

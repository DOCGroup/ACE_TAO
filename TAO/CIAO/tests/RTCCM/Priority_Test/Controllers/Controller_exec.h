// $Id$

/**
 * @file Controller_exec.h
 *
 * Header file for the actual Controller and ControllerHome component
 * implementations.  These classes are the implementations of local
 * interfaces defined in ControllerEI.idl.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef CONTROLLER_EXEC_H
#define CONTROLLER_EXEC_H

#include "ControllerEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class Controller_exec_i
   *
   * A generic controller implementation.
   */
  class CONTROLLER_EXEC_Export Controller_exec_i :
    public virtual Priority_Test::Controller_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Controller_exec_i ();

    /// Default destructor.
    ~Controller_exec_i ();

    // Operations from Priority_Test::Controller

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
    /// Copmponent specific context
    Priority_Test::CCM_Controller_Context_var context_;
  };

  /**
   * @class Controller_exec_i
   *
   * CB_Worker home executor implementation class.
   */
  class CONTROLLER_EXEC_Export Controller_Home_exec_i :
    public virtual Priority_Test::CCM_ControllerHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    Controller_Home_exec_i ();

    /// Default dtor.
    ~Controller_Home_exec_i ();

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
extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome_Impl (void);

#endif /* CONTROLLER_EXEC_H */

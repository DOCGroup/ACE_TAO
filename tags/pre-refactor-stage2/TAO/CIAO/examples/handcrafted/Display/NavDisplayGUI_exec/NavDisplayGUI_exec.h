// $Id$

/**
 * @file NavDisplayGUI_exec.h
 *
 * Header file for the actual NavDisplay and NavDisplayHome component
 * implementations.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef NAVDISPLAY_EXEC_H
#define NAVDISPLAY_EXEC_H

#include "NavDisplayEIC.h"
#include "ace/Synch.h"
#include "tao/LocalObject.h"
#include "NavUnit.h"

class Worker;


namespace MyImpl
{
  /**
   * @class NavDisplayGUI_exec_impl
   *
   * RateGen executor implementation class.
   */
  class NAVDISPLAY_EXEC_Export NavDisplayGUI_exec_impl :
    public virtual HUDisplay::NavDisplay_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    NavDisplayGUI_exec_impl ();

    /// Default destructor.
    ~NavDisplayGUI_exec_impl ();

    // Operations from HUDisplay::NavDisplay

    virtual void
    push_Refresh (HUDisplay::tick *ev
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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
    /// Copmponent specific context
    HUDisplay::CCM_NavDisplay_Context_var context_;
    Worker *worker_;
    NavUnit unit_;
    UnitLocation loc_;
    ACE_Thread_Mutex mutex_;
  };

  /**
   * @class NavDisplayGUIHome_exec_impl
   *
   * NavDisplay home executor implementation class.
   */
  class NAVDISPLAY_EXEC_Export NavDisplayGUIHome_exec_impl :
    public virtual HUDisplay::CCM_NavDisplayHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    NavDisplayGUIHome_exec_impl ();

    /// Default dtor.
    ~NavDisplayGUIHome_exec_impl ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" NAVDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void);

#endif /* NAVDISPLAY_EXEC_H */

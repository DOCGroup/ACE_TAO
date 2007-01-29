// $Id$

/**
 * @file NavDisplay_exec.h
 *
 * Header file for the actual NavDisplay and NavDisplayHome component
 * implementations.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef NAVDISPLAY_EXEC_H
#define NAVDISPLAY_EXEC_H

#include "NavDisplayEC.h"
#include "NavDisplay_exec_export.h"
#include "tao/LocalObject.h"

namespace MyImpl
{
  /**
   * @class NavDisplay_exec_impl
   *
   * RateGen executor implementation class.
   */
  class NAVDISPLAY_EXEC_Export NavDisplay_exec_impl :
    public virtual CIDL_NavDisplay_Impl::NavDisplay_exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    NavDisplay_exec_impl ();

    /// Default destructor.
    ~NavDisplay_exec_impl ();

    // Operations from HUDisplay::NavDisplay

    virtual void
    push_Refresh (HUDisplay::tick *ev)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ciao_preactivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
    virtual void
    ccm_activate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ciao_postactivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_passivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_remove ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  protected:
    /// Copmponent specific context
    HUDisplay::CCM_NavDisplay_Context_var context_;
  };

  /**
   * @class NavDisplayHome_exec_impl
   *
   * NavDisplay home executor implementation class.
   */
  class NAVDISPLAY_EXEC_Export NavDisplayHome_exec_impl :
    public virtual HUDisplay::CCM_NavDisplayHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    NavDisplayHome_exec_impl ();

    /// Default dtor.
    ~NavDisplayHome_exec_impl ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" NAVDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void);

#endif /* NAVDISPLAY_EXEC_H */

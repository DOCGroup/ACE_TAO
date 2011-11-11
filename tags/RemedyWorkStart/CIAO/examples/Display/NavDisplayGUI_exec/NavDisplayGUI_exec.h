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

#include "NavDisplayEC.h"
#include "NavDisplay_exec_export.h"
#include "ace/Synch.h"
#include "tao/LocalObject.h"
#include "NavUnit.h"
#include <vector>

class Worker;

namespace MyImpl
{
  /**
   * @class NavDisplayGUI_exec_impl
   *
   * RateGen executor implementation class.
   */

  class NAVDISPLAY_EXEC_Export NavDisplayGUI_exec_impl
    : public virtual ::CIAO_HUDisplay_NavDisplay_Impl::NavDisplay_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    /// Default constructor.
    NavDisplayGUI_exec_impl (void);

    /// Default destructor.
    ~NavDisplayGUI_exec_impl (void);

    // Operations from HUDisplay::NavDisplay

    virtual void
    push_Refresh (HUDisplay::tick *ev);

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx);

    virtual void
    configuration_complete (void);

    virtual void
    ccm_activate (void);

    virtual void
    ccm_passivate (void);

    virtual void
    ccm_remove (void);

    //attribute operations
    virtual CORBA::Long number_of_GPS ();
    virtual void number_of_GPS (CORBA::Long number_of_GPS);

  protected:
    /// Copmponent specific context
    HUDisplay::CCM_NavDisplay_Context_var context_;
    Worker *worker_;
    CORBA::Long number_of_GPS_;
    UnitLocation loc_;
    std::vector<NavUnit*> navunitarr;
    TAO_SYNCH_RECURSIVE_MUTEX mutex_;
  };

  /**
   * @class NavDisplayGUIHome_exec_impl
   *
   * NavDisplay home executor implementation class.
   */
  class NAVDISPLAY_EXEC_Export NavDisplayGUIHome_exec_impl
    : public virtual HUDisplay::CCM_NavDisplayHome,
      public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    NavDisplayGUIHome_exec_impl (void);

    /// Default dtor.
    ~NavDisplayGUIHome_exec_impl (void);

    // Explicit home operations.

    // Implicit home operations.

    virtual
    ::Components::EnterpriseComponent_ptr
    create (void);
  };

}

extern "C" NAVDISPLAY_EXEC_Export
::Components::HomeExecutorBase_ptr
createNavDisplayHome_Impl (void);

#endif /* NAVDISPLAY_EXEC_H */

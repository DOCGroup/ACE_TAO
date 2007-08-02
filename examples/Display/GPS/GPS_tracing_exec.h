// $Id$

/**
 * @file GPS_tracing_exec.h
 *
 * Header file for the actual GPS and GPSHome component
 * implementations.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef GPS_TRACING_EXEC_H
#define GPS_TRACING_EXEC_H

#include "GPSEIC.h"
#include "tao/LocalObject.h"

namespace MyImpl
{
  /**
   * @class GPS_tracing_exec_i
   *
   * RateGen executor implementation class.
   */
  class GPS_EXEC_Export GPS_tracing_exec_i :
    public virtual HUDisplay::GPS_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    GPS_tracing_exec_i ();

    /// Default destructor.
    ~GPS_tracing_exec_i ();

    // Operations from HUDisplay::GPS

    virtual HUDisplay::CCM_position_ptr
    get_MyLocation ();

    virtual void
    push_Refresh (HUDisplay::tick *ev);

    // Operations from HUDisplay::position

    virtual CORBA::Long
    posx ();

    virtual CORBA::Long
    posy ();

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx);

    virtual void
    ciao_preactivate ();

    virtual void
    ccm_activate ();

    virtual void
    ciao_postactivate ();

    virtual void
    ccm_passivate ();

    virtual void
    ccm_remove ();
  protected:
    /// Current GPS reading.
    CORBA::Long positionx_;
    CORBA::Long positiony_;

    /// Delta amounts to emulate the position shift of each reading.
    int dx_;
    int dy_;

    /// Copmponent specific context
    HUDisplay::CCM_GPS_Context_var context_;
  };

  /**
   * @class GPSHome_tracing_exec_i
   *
   * GPS home executor implementation class.
   */
  class GPS_EXEC_Export GPSHome_tracing_exec_i :
    public virtual HUDisplay::CCM_GPSHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    GPSHome_tracing_exec_i ();

    /// Default dtor.
    ~GPSHome_tracing_exec_i ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();
  };

}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" GPS_EXEC_Export ::Components::HomeExecutorBase_ptr
createGPSHome_Tracing_Impl (void);

#endif /* GPS_TRACING_EXEC_H */

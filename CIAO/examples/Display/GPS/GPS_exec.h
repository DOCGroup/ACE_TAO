/**
 * @file GPS_exec.h
 * $Id$
 * Header file for the GPS component implementation.
 *
 */

#ifndef GPS_EXEC_H
#define GPS_EXEC_H

#include "GPS_exec_export.h"
#include "GPSEC.h"
#include "ace/OS_NS_time.h"
#include "tao/LocalObject.h"

#define DISPLACEMENT 256

namespace MyImpl
{
 /**
   * @class GPS_exec_i
   *
   * GPS executor implementation class.
   */
  class GPS_EXEC_Export GPS_exec_i :
    public virtual ::CIAO_HUDisplay_GPS_Impl::GPS_Exec,
    public virtual ::CORBA::LocalObject
  {

  public:
    /// Default constructor.
    GPS_exec_i ();

    /// Default destructor.
    ~GPS_exec_i ();

    // Operations from HUDisplay::GPS
    HUDisplay::CCM_position_ptr get_MyLocation ();

    void push_Refresh (HUDisplay::tick *);

    // Operations from HUDisplay::position
    HUDisplay::GPS_position posxy ();

    CORBA::UShort id ();
    void id ( CORBA::UShort id);

    CORBA::Boolean started ();

    // Operations from Components::SessionComponent
    void set_session_context (Components::SessionContext_ptr ctx);

    void configuration_complete ();

    void ccm_activate ();

    void ccm_passivate ();

    void ccm_remove ();

   protected:
    // Component specific context
    HUDisplay::CCM_GPS_Context_var context_;

   private:
    CORBA::Long positionx_, positiony_;
    CORBA::UShort id_;
    CORBA::Boolean started_;
  };

  /**
   * @class GPSHome_exec_i
   *
   * GPS home executor implementation class.
   */
    class GPS_EXEC_Export GPSHome_exec_i :
      public virtual HUDisplay::CCM_GPSHome,
      public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    GPSHome_exec_i ();

    /// Default dtor.
    ~GPSHome_exec_i ();

    // Implicit home operations
    virtual ::Components::EnterpriseComponent_ptr create ();
  };


 class Position_Impl : public virtual HUDisplay::CCM_position,
                       public virtual ::CORBA::LocalObject
  {
  public:
    Position_Impl (GPS_exec_i& component)
        : component_ (component)
    {
    }

    // Operations from HUDisplay::position
    HUDisplay::GPS_position posxy ();

    CORBA::UShort id ();

    CORBA::Boolean started ();

  private:
    GPS_exec_i& component_;
  };

}

extern "C" GPS_EXEC_Export ::Components::HomeExecutorBase_ptr
createGPSHome_Impl (void);

#endif /* GPS_EXEC_H */

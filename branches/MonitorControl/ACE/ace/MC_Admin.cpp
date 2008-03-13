// $Id$

#include "ace/MC_Admin.h"
#include "ace/Reactor.h"
#include "ace/Monitor_Point_Registry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    int
    MonitorPointAutoUpdater::handle_timeout (
      const ACE_Time_Value& /* current_time */,
      const void* monitor_point)
    {
      const Monitor_Base* const_mp =
        reinterpret_cast<const Monitor_Base*> (monitor_point);
      Monitor_Base* mp = const_cast<Monitor_Base*> (const_mp);
      mp->update ();
      return 0;
    }
    
    //====================================================================
    
    MC_Admin::InvalidName::InvalidName (
      const MonitorControl_Types::NameList& names)
      : names_ (names)
    {}
    
    MC_Admin::InvalidName::~InvalidName (void)
    {
      // TODO - destroy name list?
    }
    
    //====================================================================
    
    MC_Admin::MC_Admin (void)
      : reactor_ (ACE_Reactor::instance ()),
        delete_reactor_ (false)
    {}
    
    MC_Admin::~MC_Admin (void)
    {
      if (this->delete_reactor_)
        {
          delete this->reactor_;
        }
      else
        {
          /// Destroys the timers associated with our event handler
          /// before its destructor is called.
          ACE_Reactor::instance ()->close_singleton ();
        }
    }
    
    bool
    MC_Admin::monitor_point (Monitor_Base* monitor_point,
                             unsigned long auto_update_msec)
    {
      bool good_reg_add =
        Monitor_Point_Registry::instance ()->add (monitor_point);
       
      /// @@@ (JP) TODO if the boolean in false.  
      if (good_reg_add && auto_update_msec != 0)
        {
          time_t secs = static_cast<time_t> (auto_update_msec / 1000);
          suseconds_t usecs =
            static_cast<suseconds_t> ((auto_update_msec % 1000) * 1000);
          ACE_Time_Value tv (secs, usecs);
          this->reactor_->schedule_timer (&this->auto_updater_,
                                          monitor_point,
                                          ACE_Time_Value::zero,
                                          tv);
        }
        
      return good_reg_add;
    }
    
    Monitor_Base*
    MC_Admin::monitor_point (const char* name)
    {
      ACE_CString name_str (name, 0, false);
      return Monitor_Point_Registry::instance ()->get (name_str);
    }
    
    void
    MC_Admin::auto_query (ACE_Event_Handler* handler,
                          MonitorQuery* query,
                          unsigned long period_secs)
    {
      ACE_Time_Value tv (period_secs);
      this->reactor_->schedule_timer (handler,
                                      query,
                                      ACE_Time_Value::zero,
                                      tv);
    }
    
    bool
    MC_Admin::control_action (Control_Action* /* control_action */)
    {
      // TODO
      return true;
    }
    
    void
    MC_Admin::clear_statistics (const MonitorControl_Types::NameList& /* names */)
    {
      // TODO
    }
    
    void
    MC_Admin::update_monitors (const MonitorControl_Types::NameList& /* names */,
                               bool /* notify */)
    {
      // TODO
    }
    
    void
    MC_Admin::reactor (ACE_Reactor* new_reactor)
    {
      this->reactor_ = new_reactor;
      this->delete_reactor_ = true;
    }
    
    ACE_Reactor*
    MC_Admin::reactor (void) const
    {
      return this->reactor_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


// $Id$

#include "ace/Reactor.h"

#include "MonitorControl/Admin.h"
#include "MonitorControl/MonitorPointRegistry.h"

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
    
    Admin::InvalidName::InvalidName (
      const MonitorControl_Types::NameList& names)
      : names_ (names)
    {}
    
    Admin::InvalidName::~InvalidName (void)
    {
      // TODO - destroy name list?
    }
    
    //====================================================================
    
    Admin::Admin (void)
      : reactor_ (ACE_Reactor::instance ()),
        delete_reactor_ (false)
    {}
    
    Admin::~Admin (void)
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
    Admin::add_monitor_point (Monitor_Base* monitor_point,
                              unsigned long auto_update_msec)
    {
      bool good_reg_add =
        MonitorPointRegistry::instance ()->add (monitor_point);
       
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
    
    bool
    Admin::add_control_action (ControlAction* /* control_action */)
    {
      // TODO
      return true;
    }
    
    void
    Admin::clear_statistics (const MonitorControl_Types::NameList& /* names */)
    {
      // TODO
    }
    
    void
    Admin::update_monitors (const MonitorControl_Types::NameList& /* names */,
                            bool /* notify */)
    {
      // TODO
    }
    
    void
    Admin::reactor (ACE_Reactor* new_reactor)
    {
      this->reactor_ = new_reactor;
      this->delete_reactor_ = true;
    }
    
    ACE_Reactor*
    Admin::reactor (void) const
    {
      return this->reactor_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


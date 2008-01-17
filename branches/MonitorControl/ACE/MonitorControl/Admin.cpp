// $Id$

#include "MonitorControl/Admin.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPointAutoUpdater::MonitorPointAutoUpdater (void)
    {}
  
    MonitorPointAutoUpdater::~MonitorPointAutoUpdater (void)
    {}
  
    int
    MonitorPointAutoUpdater::handle_timeout (
      const ACE_Time_Value& /* interval */,
      const void* /* monitor_point */)
    {
      // TODO
      return 0;
    }
    
    //====================================================================
    
    Admin::InvalidName::InvalidName (const Datatypes::NameList& names)
      : names_ (names)
    {}
    
    Admin::InvalidName::~InvalidName (void)
    {
      // TODO - destroy name list?
    }
    
    //====================================================================
    
    Admin::Admin (void)
      : auto_updater_ (new MonitorPointAutoUpdater)
    {}
    
    Admin::~Admin (void)
    {
      delete this->auto_updater_;
    }
    
    bool
    Admin::add_monitor_point (Statistic* /* monitor_point */,
                              unsigned long /* auto_update_msec */)
    {
      // TODO
      return true;
    }
    
    bool
    Admin::remove_monitor_point (const char* /* name */)
    {
      // TODO
      return true;
    }
    
    bool
    Admin::add_control_action (ControlAction* /* control_action */)
    {
      // TODO
      return true;
    }
    
    bool
    Admin::remove_control_action (const char* /* name */)
    {
      // TODO
      return true;
    }
    
    void
    Admin::clear_statistics (const Datatypes::NameList& /* names */)
    {
      // TODO
    }
    
    void
    Admin::update (const Datatypes::NameList& /* names */,
                   bool /* notify */)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


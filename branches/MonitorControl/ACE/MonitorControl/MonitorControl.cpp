// $Id$

#include "ace/Service_Config.h"

#include "MonitorControl/MonitorControl.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    AdminManager::AdminManager (void)
    {}
    
    AdminManager::~AdminManager (void)
    {}
    
    int
    AdminManager::init (int argc, ACE_TCHAR* argv[])
    {
      // TODO
      return 0;
    }
    
    int
    AdminManager::fini (void)
    {
      // TODO
      return 0;
    }
    
    int
    AdminManager::run (void)
    {
      // TODO
      return 0;
    }
    
    void
    AdminManager::shutdown (void)
    {
      // TODO
    }
    
    Admin&
    AdminManager::admin (void)
    {
      return this->admin_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (MONITORCONTROL, MC_ADMINMANAGER)
    

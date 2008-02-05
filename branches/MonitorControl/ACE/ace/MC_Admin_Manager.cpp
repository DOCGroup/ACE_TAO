// $Id$

#include "ace/MC_Admin_Manager.h"
#include "ace/Service_Config.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MC_Admin_Manager::MC_Admin_Manager (void)
    {}
    
    MC_Admin_Manager::~MC_Admin_Manager (void)
    {}
    
    int
    MC_Admin_Manager::init (int argc, ACE_TCHAR* argv[])
    {
      // TODO
      return 0;
    }
    
    int
    MC_Admin_Manager::fini (void)
    {
      // TODO
      return 0;
    }
    
    int
    MC_Admin_Manager::run (void)
    {
      // TODO
      return 0;
    }
    
    void
    MC_Admin_Manager::shutdown (void)
    {
      // TODO
    }
    
    MC_Admin&
    MC_Admin_Manager::admin (void)
    {
      return this->admin_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (ACE, MC_ADMINMANAGER)
    

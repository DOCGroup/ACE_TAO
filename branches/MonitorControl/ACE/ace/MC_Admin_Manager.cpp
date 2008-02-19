// $Id$

#include "ace/MC_Admin_Manager.h"
#include "ace/Service_Config.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MC_Admin&
    MC_Admin_Manager::admin (void)
    {
      return this->admin_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (ACE, MC_ADMINMANAGER)
    

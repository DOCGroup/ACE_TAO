// $Id$

#include "ace/Singleton.h"

#include "MonitorControl/ControlActionRegistry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    ControlActionRegistry*
    ControlActionRegistry::instance (void)
    {
      return
        ACE_Singleton<ControlActionRegistry, ACE_Null_Mutex>::instance ();
      /// @@@ (JP) We may need to use ACE_Recursive_Thread_Mutex instead.
    }
    
    ControlActionRegistry::ControlActionRegistry (void)
    {}
    
    ControlActionRegistry::~ControlActionRegistry (void)
    {}
    
    ControlAction*
    ControlActionRegistry::get (const ACE_CString& name) const
    {
      // TODO
      return 0;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


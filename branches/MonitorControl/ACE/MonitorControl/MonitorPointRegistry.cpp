// $Id$

#include "ace/Singleton.h"

#include "MonitorControl/MonitorPointRegistry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPointRegistry*
    MonitorPointRegistry::instance (void)
    {
      return
        ACE_Singleton<MonitorPointRegistry, ACE_Null_Mutex>::instance ();
      /// @@@ (JP) We may need to use ACE_Recursive_Thread_Mutex instead.
    }
    
    MonitorPointRegistry::MonitorPointRegistry (void)
    {}
    
    MonitorPointRegistry::~MonitorPointRegistry (void)
    {}
    
    Monitor_Base*
    MonitorPointRegistry::get (const ACE_CString& name) const
    {
      // TODO
      return 0;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


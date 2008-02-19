// $Id$

#include "ace/Monitor_Point_Registry.h"
#include "ace/Monitor_Base.h"
#include "ace/Singleton.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Monitor_Point_Registry*
    Monitor_Point_Registry::instance (void)
    {
      return
        ACE_Singleton<Monitor_Point_Registry, ACE_Null_Mutex>::instance ();
      /// @@@ (JP) We may need to use ACE_Recursive_Thread_Mutex instead.
    }
    
    Monitor_Base*
    Monitor_Point_Registry::get (const ACE_CString& name) const
    {
      /// getobj() returns MC_Generic, which has no virtual methods,
      /// so dynamic_cast<> isn't needed - in fact it would be an error.
      return static_cast<Monitor_Base*> (this->getobj (name));
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


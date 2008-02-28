// $Id$

#include "ace/Singleton.h"

#include "ace/Control_Action_Registry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Control_Action_Registry*
    Control_Action_Registry::instance (void)
    {
      return
        ACE_Singleton<Control_Action_Registry, ACE_Thread_Mutex>::instance ();
    }

    Control_Action*
    Control_Action_Registry::get (const ACE_CString& name) const
    {
      // TODO
      return 0;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


// $Id$

#include "ace/Monitor_Base.h"
#include "ace/MC_Admin_Manager.h"
#include "ace/Monitor_Point_Registry.h"
#include "ace/Guard_T.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_sys_time.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    ACE_INLINE
    const char*
    Monitor_Base::name (void) const
    {
      return this->name_;
    }

    ACE_INLINE
    void
    Monitor_Base::name (const char* new_name)
    {
      this->name_ = new_name;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


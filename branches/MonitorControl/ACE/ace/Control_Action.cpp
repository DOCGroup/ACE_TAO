// $Id$

#include "ace/Control_Action.h"
#include "ace/Guard_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Control_Action::Control_Action (void)
      : ACE_Refcountable (1)
    {}
  
    Control_Action::~Control_Action (void)
    {}
    
    void
    Control_Action::add_ref (void)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      (void) this->increment ();
    }
    
    void
    Control_Action::remove_ref (void)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      const int new_count = this->decrement ();
      
      if (new_count != 0)
        {
          return;
        }
        
      delete this;
    }
    
    int
    Control_Action::refcount (void) const
    {
      return ACE_Refcountable::refcount ();
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


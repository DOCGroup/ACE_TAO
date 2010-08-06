// -*- C++ -*-
//
// $Id$

#include "Monitor_Manager.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#if !defined (__ACE_INLINE__)
#include "Monitor_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    Monitor_Manager::Monitor_Manager (const Monitor_Manager &m)
      : ptr_ (m.get ())
    {
      this->ptr_->add_ref ();
    }
  
    Monitor_Manager &
    Monitor_Manager::operator= (Monitor_Base *p)
    {
      this->reset (p);
      return *this;
    }
    
    Monitor_Manager &
    Monitor_Manager::operator= (const Monitor_Manager &m)
    {
      this->reset (m.get ());
      this->ptr_->add_ref ();
      return *this;
    }
    
    Monitor_Base *
    Monitor_Manager::release (void)
    {
      Monitor_Base * val = this->ptr_;
      this->ptr_ = 0;
      return val;
    }
    
    void
    Monitor_Manager::free (void)
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->remove_ref ();
        }
    }
    
    void
    Monitor_Manager::reset (Monitor_Base *p)
    {
      this->free ();
      this->ptr_ = p;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // ACE_HAS_MONITOR_FRAMEWORK == 1


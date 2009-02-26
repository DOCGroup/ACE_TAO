// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    ACE_INLINE
    Monitor_Manager::Monitor_Manager (void)
      : ptr_ (0)
    {
    }
    
    ACE_INLINE
    Monitor_Manager::Monitor_Manager (Monitor_Base *p)
      : ptr_ (p)
    {
    }
    
    ACE_INLINE
    Monitor_Manager::~Monitor_Manager (void)
    {
      this->free ();
    }
    
    ACE_INLINE
    Monitor_Base *
    Monitor_Manager::operator-> (void) const
    {
      return this->ptr_;
    }
    
    ACE_INLINE
    Monitor_Base *
    Monitor_Manager::get (void) const
    {
      return this->ptr_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


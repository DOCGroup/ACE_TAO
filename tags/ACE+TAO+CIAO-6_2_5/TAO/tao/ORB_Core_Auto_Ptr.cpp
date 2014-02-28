// -*- C++ -*-
// $Id$

#include "tao/ORB_Core_Auto_Ptr.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORB_Core_Auto_Ptr.inl"
#endif  /* !__ACE_INLINE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ORB_Core_Auto_Ptr::~TAO_ORB_Core_Auto_Ptr (void)
{
  if (this->get () != 0)
    {
      this->get ()->_decr_refcnt ();
    }
}

void
TAO_ORB_Core_Auto_Ptr::reset (TAO_ORB_Core *p)
{
  if (this->get () != p && this->get () != 0)
    {
      this->get ()->_decr_refcnt ();
    }

  this->p_ = p;
}

TAO_END_VERSIONED_NAMESPACE_DECL

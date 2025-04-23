// -*- C++ -*-
#include "ace/Global_Macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer_Basic<CONCRETE>::based (ACE_Based_Pointer_Basic *p)
{
  return reinterpret_cast<CONCRETE *> (reinterpret_cast<char *> (p) - p->base_offset_ + p->target_);
}

template <class CONCRETE> ACE_INLINE CONCRETE const *
ACE_Based_Pointer_Basic<CONCRETE>::based (ACE_Based_Pointer_Basic const *p)
{
  return reinterpret_cast<CONCRETE const *> (reinterpret_cast<const char *> (p) - p->base_offset_ + p->target_);
}

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer<CONCRETE>::operator-> ()
{
  ACE_TRACE ("ACE_Based_Pointer<CONCRETE>::operator->");
  return ACE_Based_Pointer::based (this);
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer_Basic<CONCRETE>::operator= (CONCRETE *rhs)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator=");
  if (rhs == 0)
    // Store a value of <target_> that indicate "NULL" pointer.
    this->target_ = -1;
  else
    this->target_ = ((char *) rhs
                     - ((char *) this - this->base_offset_));
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer<CONCRETE>::operator= (CONCRETE *rhs)
{
  ACE_TRACE ("ACE_Based_Pointer<CONCRETE>::operator=");
  if (rhs == 0)
    // Store a value of <target_> that indicate "NULL" pointer.
    this->target_ = -1;
  else
    this->target_ = ((char *) rhs
                     - ((char *) this - this->base_offset_));
}

template <class CONCRETE> ACE_INLINE CONCRETE
ACE_Based_Pointer_Basic<CONCRETE>::operator * () const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator *");
  return *ACE_Based_Pointer_Basic::based (this);
}

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer_Basic<CONCRETE>::addr () const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::addr");

  if (this->target_ == -1)
    return 0;
  else
    return const_cast<CONCRETE *> (ACE_Based_Pointer_Basic::based (this));
}

template <class CONCRETE> ACE_INLINE
ACE_Based_Pointer_Basic<CONCRETE>::operator CONCRETE * () const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator CONCRETE *()");

  return this->addr ();
}

template <class CONCRETE> ACE_INLINE CONCRETE
ACE_Based_Pointer_Basic<CONCRETE>::operator[] (int index) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator[]");
  return ACE_Based_Pointer_Basic::based (this)[index];
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer_Basic<CONCRETE>::operator+= (int index)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator+=");
  this->base_offset_ += (index * sizeof (CONCRETE));
}

template <class CONCRETE> ACE_INLINE bool
ACE_Based_Pointer_Basic<CONCRETE>::operator== (const ACE_Based_Pointer_Basic &rhs) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator==");
  return ACE_Based_Pointer_Basic::based (this) == ACE_Based_Pointer_Basic::based (&rhs);
}

template <class CONCRETE> ACE_INLINE bool
ACE_Based_Pointer_Basic<CONCRETE>::operator!= (const ACE_Based_Pointer_Basic &rhs) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator!=");
  return !(*this == rhs);
}

template <class CONCRETE> ACE_INLINE bool
ACE_Based_Pointer_Basic<CONCRETE>::operator< (const ACE_Based_Pointer_Basic &rhs) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator<");
  return ACE_Based_Pointer_Basic::based (this) < ACE_Based_Pointer_Basic::based (&rhs);
}

template <class CONCRETE> ACE_INLINE bool
ACE_Based_Pointer_Basic<CONCRETE>::operator<= (const ACE_Based_Pointer_Basic &rhs) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator<=");
  return ACE_Based_Pointer_Basic::based (this) <= ACE_Based_Pointer_Basic::based (&rhs);
}

template <class CONCRETE> ACE_INLINE bool
ACE_Based_Pointer_Basic<CONCRETE>::operator> (const ACE_Based_Pointer_Basic &rhs) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator>");
  return ACE_Based_Pointer_Basic::based (this) > ACE_Based_Pointer_Basic::based (&rhs);
}

template <class CONCRETE> ACE_INLINE bool
ACE_Based_Pointer_Basic<CONCRETE>::operator>= (const ACE_Based_Pointer_Basic &rhs) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator>=");
  return ACE_Based_Pointer_Basic::based (this) >= ACE_Based_Pointer_Basic::based (&rhs);
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer_Basic<CONCRETE>::operator= (const ACE_Based_Pointer_Basic &rhs)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::operator=");
  *this = rhs.addr ();
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer<CONCRETE>::operator= (const ACE_Based_Pointer &rhs)
{
  ACE_TRACE ("ACE_Based_Pointer<CONCRETE>::operator=");
  *this = rhs.addr ();
}

ACE_END_VERSIONED_NAMESPACE_DECL

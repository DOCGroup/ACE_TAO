/* -*- C++ -*- */
// $Id$

#define ACE_COMPUTE_BASED_POINTER(P) (((char *) (P) - (P)->base_offset_) + (long) (P)->target_)

template <class CONCRETE> ACE_INLINE
ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer (const ACE_Based_Pointer<CONCRETE> &lhs)
  : target_ (lhs.target_),
    base_offset_ (lhs.base_offset_)
{
}

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer<CONCRETE>::operator->(void)
{
  return (CONCRETE *)(ACE_COMPUTE_BASED_POINTER (this));
}

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer<CONCRETE>::operator =(CONCRETE *from)
{
  this->target_ = (CONCRETE *)((char *) from 
                               - ((char *) this - this->base_offset_));
  return from;
}

template <class CONCRETE> ACE_INLINE CONCRETE 
ACE_Based_Pointer<CONCRETE>::operator *(void) const
{
  return *(CONCRETE *)(ACE_COMPUTE_BASED_POINTER (this));
}

template <class CONCRETE> ACE_INLINE 
ACE_Based_Pointer<CONCRETE>::operator void *() const
{
  return ACE_reinterpret_cast (void *,
                               ACE_COMPUTE_BASED_POINTER (this));
}

template <class CONCRETE> ACE_INLINE CONCRETE
ACE_Based_Pointer<CONCRETE>::operator [] (long index) const
{
  return *((CONCRETE *)(ACE_COMPUTE_BASED_POINTER (this)) + index);
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer<CONCRETE>::operator += (long index)
{
  this->base_offset_ += (index * sizeof (CONCRETE));
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer<CONCRETE>::operator == (const ACE_Based_Pointer<CONCRETE> &rhs) const
{
  return ACE_COMPUTE_BASED_POINTER (this) == ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer<CONCRETE>::operator != (const ACE_Based_Pointer<CONCRETE> &rhs) const
{
  return !(*this == rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer<CONCRETE>::operator < (const ACE_Based_Pointer<CONCRETE> &rhs) const
{
  return ACE_COMPUTE_BASED_POINTER (this) < ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer<CONCRETE>::operator <= (const ACE_Based_Pointer<CONCRETE> &rhs) const
{
  return ACE_COMPUTE_BASED_POINTER (this) <= ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer<CONCRETE>::operator > (const ACE_Based_Pointer<CONCRETE> &rhs) const
{
  return ACE_COMPUTE_BASED_POINTER (this) > ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer<CONCRETE>::operator >= (const ACE_Based_Pointer<CONCRETE> &rhs) const
{
  return ACE_COMPUTE_BASED_POINTER (this) >= ACE_COMPUTE_BASED_POINTER (&rhs);
}

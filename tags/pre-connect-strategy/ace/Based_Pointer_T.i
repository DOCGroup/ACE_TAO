/* -*- C++ -*- */
// $Id$

#define ACE_COMPUTE_BASED_POINTER(P) (((char *) (P) - (P)->base_offset_) + (P)->target_)

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer<CONCRETE>::operator->(void)
{
  ACE_TRACEX ("ACE_Based_Pointer<CONCRETE>::operator->");
    this->dump ();
  return (CONCRETE *)(ACE_COMPUTE_BASED_POINTER (this));
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer_Basic<CONCRETE>::operator = (CONCRETE *rhs)
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator=");
  if (rhs != 0)
    this->target_ = ((char *) rhs
                     - ((char *) this - this->base_offset_));
  this->dump ();
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer<CONCRETE>::operator = (CONCRETE *rhs)
{
  ACE_TRACEX ("ACE_Based_Pointer<CONCRETE>::operator=");
  this->target_ = ((char *) rhs
                   - ((char *) this - this->base_offset_));
    this->dump ();
}

template <class CONCRETE> ACE_INLINE CONCRETE 
ACE_Based_Pointer_Basic<CONCRETE>::operator *(void) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator *");
  this->dump ();
  return *ACE_reinterpret_cast (CONCRETE *,
                                ACE_COMPUTE_BASED_POINTER (this));
}

template <class CONCRETE> ACE_INLINE CONCRETE *
ACE_Based_Pointer_Basic<CONCRETE>::addr (void) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::addr");
    this->dump ();
  return ACE_reinterpret_cast (CONCRETE *,
                               ACE_COMPUTE_BASED_POINTER (this));
}

template <class CONCRETE> ACE_INLINE CONCRETE
ACE_Based_Pointer_Basic<CONCRETE>::operator [] (long index) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator[]");
    this->dump ();
  CONCRETE *c = ACE_reinterpret_cast (CONCRETE *,
                                      ACE_COMPUTE_BASED_POINTER (this));
  return c[index];
}

template <class CONCRETE> ACE_INLINE void
ACE_Based_Pointer_Basic<CONCRETE>::operator += (long index)
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator+=");
    this->dump ();
  this->base_offset_ += (index * sizeof (CONCRETE));
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer_Basic<CONCRETE>::operator == (const ACE_Based_Pointer_Basic<CONCRETE> &rhs) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator==");
    this->dump ();
  return ACE_COMPUTE_BASED_POINTER (this) == ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer_Basic<CONCRETE>::operator != (const ACE_Based_Pointer_Basic<CONCRETE> &rhs) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator!=");
    this->dump ();
  return !(*this == rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer_Basic<CONCRETE>::operator < (const ACE_Based_Pointer_Basic<CONCRETE> &rhs) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator<");
    this->dump ();
  return ACE_COMPUTE_BASED_POINTER (this) < ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer_Basic<CONCRETE>::operator <= (const ACE_Based_Pointer_Basic<CONCRETE> &rhs) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator<=");
    this->dump ();
  return ACE_COMPUTE_BASED_POINTER (this) <= ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer_Basic<CONCRETE>::operator > (const ACE_Based_Pointer_Basic<CONCRETE> &rhs) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator>");
    this->dump ();
  return ACE_COMPUTE_BASED_POINTER (this) > ACE_COMPUTE_BASED_POINTER (&rhs);
}

template <class CONCRETE> ACE_INLINE int 
ACE_Based_Pointer_Basic<CONCRETE>::operator >= (const ACE_Based_Pointer_Basic<CONCRETE> &rhs) const
{
  ACE_TRACEX ("ACE_Based_Pointer_Basic<CONCRETE>::operator>=");
    this->dump ();
  return ACE_COMPUTE_BASED_POINTER (this) >= ACE_COMPUTE_BASED_POINTER (&rhs);
}


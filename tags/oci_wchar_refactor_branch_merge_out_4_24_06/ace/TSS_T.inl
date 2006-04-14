/* -*- C++ -*- */
// $Id$

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))

template <class TYPE> ACE_INLINE
ACE_TSS<TYPE>::ACE_TSS (TYPE *type)
  : type_ (type)
{
}

template <class TYPE> ACE_INLINE int
ACE_TSS<TYPE>::ts_init (void) const
{
  return 0;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_object (void) const
{
  return this->type_;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_object (TYPE *type)
{
  this->type_ = type;
  return this->type_;
}

template <class TYPE> ACE_INLINE TYPE *
ACE_TSS<TYPE>::ts_get (void) const
{
  return this->type_;
}

#endif /* ! (defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))) */

// -*- C++ -*-
// $Id$

#if defined (ACE_HAS_WINCE_BROKEN_ERRNO)

ACE_INLINE ACE_CE_Errno *
ACE_CE_Errno::instance ()
{
  // This should be inlined.
  return ACE_CE_Errno::instance_;
}

ACE_INLINE
ACE_CE_Errno::operator int (void) const
{
  return (int) TlsGetValue (ACE_CE_Errno::errno_key_);
}

ACE_INLINE int
ACE_CE_Errno::operator= (int x)
{
  // error checking?
  TlsSetValue (ACE_CE_Errno::errno_key_, (void *) x);
  return x;
}

#endif /* ACE_HAS_WINCE_BROKEN_ERRNO */

ACE_INLINE
ACE_Errno_Guard::ACE_Errno_Guard (ACE_ERRNO_TYPE &errno_ref,
                                  int error)
  :
#if defined (ACE_MT_SAFE)
    errno_ptr_ (&errno_ref),
#endif /* ACE_MT_SAFE */
    error_ (error)
{
#if !defined(ACE_MT_SAFE)
  ACE_UNUSED_ARG (errno_ref);
#endif /* ACE_MT_SAFE */
}

ACE_INLINE
ACE_Errno_Guard::ACE_Errno_Guard (ACE_ERRNO_TYPE &errno_ref)
  :
#if defined (ACE_MT_SAFE)
    errno_ptr_ (&errno_ref),
#endif /* ACE_MT_SAFE */
    error_ (errno_ref)
{
}

ACE_INLINE
ACE_Errno_Guard::~ACE_Errno_Guard (void)
{
#if defined (ACE_MT_SAFE)
  *errno_ptr_ = this->error_;
#else
  errno = this->error_;
#endif /* ACE_MT_SAFE */
}

#if defined (ACE_HAS_WINCE_BROKEN_ERRNO)
ACE_INLINE int
ACE_Errno_Guard::operator= (const ACE_ERRNO_TYPE &error)
{
  return this->error_ = error;
}
#endif /* ACE_HAS_WINCE_BROKEN_ERRNO */

ACE_INLINE int
ACE_Errno_Guard::operator= (int error)
{
  return this->error_ = error;
}

ACE_INLINE int
ACE_Errno_Guard::operator== (int error)
{
  return this->error_ == error;
}

ACE_INLINE int
ACE_Errno_Guard::operator!= (int error)
{
  return this->error_ != error;
}

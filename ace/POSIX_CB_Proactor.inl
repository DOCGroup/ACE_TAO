/* -*- C++ -*- */
// $Id$

#if defined (ACE_HAS_AIO_CALLS)

ACE_INLINE
ACE_POSIX_Proactor::Proactor_Type ACE_POSIX_CB_Proactor::get_impl_type (void)
{
  return PROACTOR_CB;
}

#endif /* ACE_HAS_AIO_CALLS */

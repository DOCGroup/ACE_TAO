/* -*- C++ -*- */
// $Id$

ACE_INLINE 
ACE_POSIX_Proactor::Proactor_Type ACE_POSIX_Proactor::get_impl_type (void)
{
  return PROACTOR_POSIX;
} 

ACE_INLINE 
ACE_POSIX_Proactor::Proactor_Type ACE_POSIX_AIOCB_Proactor::get_impl_type (void)
{
  return PROACTOR_AIOCB;
} 

ACE_INLINE 
ACE_POSIX_Proactor::Proactor_Type ACE_POSIX_SIG_Proactor::get_impl_type (void)
{
  return PROACTOR_SIG;
} 

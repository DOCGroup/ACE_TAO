// $Id$

#if ! defined (__ACE_INLINE__)
#include "ace/Env_Value_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template void ACE_Convert(const char*, char*&);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate void ACE_Convert (const char*, char*&)
#endif


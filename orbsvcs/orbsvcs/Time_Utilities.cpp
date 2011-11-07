// $Id$

#include "orbsvcs/Time_Utilities.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/Time_Utilities.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Number of nanoseconds between CORBA and POSIX epochs.
#ifdef ACE_LACKS_LONGLONG_T
const ACE_UINT64
ORBSVCS_Time::Time_Base_Offset(0x9e7d0000, 0x0A993A60F);
#else
const ACE_UINT64
ORBSVCS_Time::Time_Base_Offset(ACE_UINT64_LITERAL(12219292800000000000));
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

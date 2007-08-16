// $Id$

#include "orbsvcs/Time_Utilities.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/Time_Utilities.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Time_Utilities, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Number of nanoseconds between CORBA and POSIX epochs.
#ifdef ACE_LACKS_LONGLONG_T
const ACE_UINT64
ORBSVCS_Time::Time_Base_Offset(0x82B60000, 0x0A990E2D6);
#else
const ACE_UINT64
ORBSVCS_Time::Time_Base_Offset(ACE_UINT64_LITERAL(12218515200000000000));
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

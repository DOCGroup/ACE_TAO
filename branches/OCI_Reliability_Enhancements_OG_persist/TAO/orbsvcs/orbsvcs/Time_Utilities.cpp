// $Id$

#include "orbsvcs/Time_Utilities.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/Time_Utilities.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Number of nanoseconds between CORBA and POSIX epochs.
const ACE_UINT64
ORBSVCS_Time::Time_Base_Offset(ACE_UINT64_LITERAL(12219292800000000000));

TAO_END_VERSIONED_NAMESPACE_DECL

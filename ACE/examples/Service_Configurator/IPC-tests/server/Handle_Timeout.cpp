#include "Handle_Timeout.h"

#if !defined (__ACE_INLINE__)
#include "Handle_Timeout.inl"
#endif /* __ACE_INLINE__ */

Handle_Timeout timer_1;
ACE_Service_Object_Type t1 (&timer_1, ACE_TEXT("Timer_1"));

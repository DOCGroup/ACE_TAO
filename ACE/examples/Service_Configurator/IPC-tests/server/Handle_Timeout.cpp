// $Id$

#include "Handle_Timeout.h"

#if defined (SunOS4)
extern "C"
{
  int init (void);
  int fini (void);
  void __sti__Handle_Timeout_C_init_();
  void __std__Handle_Timeout_C_init_();
}

int
init (void)
{
  __sti__Handle_Timeout_C_init_();
  return 0;
}

int
fini (void)
{
  __std__Handle_Timeout_C_init_();
  return 0;
}
#endif /* SunOS4 */

#if !defined (__ACE_INLINE__)
#include "Handle_Timeout.inl"
#endif /* __ACE_INLINE__ */

Handle_Timeout timer_1;
ACE_Service_Object_Type t1 (&timer_1, ACE_TEXT("Timer_1"));

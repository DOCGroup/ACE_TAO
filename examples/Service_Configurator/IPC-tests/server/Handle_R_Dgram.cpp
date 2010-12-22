// $Id$

#include "Handle_R_Dgram.h"

#if defined (SunOS4)
extern "C"
{
  int init (void);
  int fini (void);
  void __sti__Handle_R_Dgram_C_init_();
  void __std__Handle_R_Dgram_C_init_();
}

int
init (void)
{
  __sti__Handle_R_Dgram_C_init_();
  return 0;
}

int
fini (void)
{
  __std__Handle_R_Dgram_C_init_();
  return 0;
}
#endif /* SunOS4 */

unsigned short Handle_R_Dgram::DEFAULT_PORT = ACE_DEFAULT_SERVER_PORT;

#if !defined (__ACE_INLINE__)
#include "Handle_R_Dgram.inl"
#endif /* __ACE_INLINE__ */

Handle_R_Dgram remote_dgram;
ACE_Service_Object_Type rd (&remote_dgram, ACE_TEXT ("Remote_Dgram"));

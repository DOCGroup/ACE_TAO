// $Id$

#include "Handle_L_CODgram.h"

ACE_RCSID(server, Handle_L_CODgram, "$Id$")

#if defined (SunOS4)
extern "C" 
{
  int init (void);
  int fini (void);
  void __sti__Handle_L_CODgram_C_init_();
  void __std__Handle_L_CODgram_C_init_();
}

int 
init (void)
{
  __sti__Handle_L_CODgram_C_init_();
  return 0;
}

int 
fini (void)
{
  __std__Handle_L_CODgram_C_init_();
  return 0;
}
#endif /* SunOS4 */

const char *Handle_L_CODgram::DEFAULT_RENDEZVOUS = "/tmp/foo_codgram";

#if !defined (__ACE_INLINE__)
#include "Handle_L_CODgram.i"
#endif /* __ACE_INLINE__ */

Handle_L_CODgram local_codgram;
ACE_Service_Object_Type lc (&local_codgram, "Local_CODgram");

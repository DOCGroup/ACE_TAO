#include "Handle_L_FIFO.h"
// @(#)Handle_L_FIFO.cpp	1.1	10/18/96


#if defined (SunOS4)
extern "C" 
{
  int init (void);
  int fini (void);
  void __sti__Handle_L_FIFO_C_init_();
  void __std__Handle_L_FIFO_C_init_();
}

int 
init (void)
{
  __sti__Handle_L_FIFO_C_init_();
  return 0;
}

int 
fini (void)
{
  __std__Handle_L_FIFO_C_init_();
  return 0;
}
#endif /* SunOS4 */

const char *Handle_L_FIFO::DEFAULT_RENDEZVOUS = "/tmp/foo_fifo";

#if !defined (__ACE_INLINE__)
#include "Handle_L_FIFO.i"
#endif /* __ACE_INLINE__ */

Handle_L_FIFO local_fifo;
ACE_Service_Object_Type lf (&local_fifo, "Local_FIFO");

#include "Handle_L_Pipe.h"
// @(#)Handle_L_Pipe.cpp	1.1	10/18/96


#if defined (SunOS4)
extern "C" 
{
  int init (void);
  int fini (void);
  void __sti__Handle_L_Pipe_C_recv_n_();
  void __std__Handle_L_Pipe_C_recv_n_();
}

int 
init (void)
{
  __sti__Handle_L_Pipe_C_recv_n_();
  return 0;
}

int 
fini (void)
{
  __std__Handle_L_Pipe_C_recv_n_();
  return 0;
}
#endif /* SunOS4 */

const char *Handle_L_Pipe::DEFAULT_RENDEZVOUS = "/tmp/foo_pipe";

#if !defined (__ACE_INLINE__)
#include "Handle_L_Pipe.i"
#endif /* __ACE_INLINE__ */

Handle_L_Pipe local_pipe;
ACE_Service_Object_Type lp (&local_pipe, "Local_Pipe");

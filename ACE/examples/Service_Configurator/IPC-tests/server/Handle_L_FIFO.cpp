#include "Handle_L_FIFO.h"

const ACE_TCHAR *Handle_L_FIFO::DEFAULT_RENDEZVOUS = ACE_TEXT("/tmp/foo_fifo");

#if !defined (__ACE_INLINE__)
#include "Handle_L_FIFO.inl"
#endif /* __ACE_INLINE__ */

Handle_L_FIFO local_fifo;
ACE_Service_Object_Type lf (&local_fifo, ACE_TEXT("Local_FIFO"));

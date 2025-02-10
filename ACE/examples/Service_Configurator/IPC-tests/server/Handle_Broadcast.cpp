#include "Handle_Broadcast.h"

unsigned short Handle_Broadcast::DEFAULT_PORT = ACE_DEFAULT_BROADCAST_PORT;

#if !defined (__ACE_INLINE__)
#include "Handle_Broadcast.inl"
#endif /* __ACE_INLINE__ */

Handle_Broadcast remote_broadcast;
ACE_Service_Object_Type rb (&remote_broadcast, ACE_TEXT("Remote_Brdcast"));

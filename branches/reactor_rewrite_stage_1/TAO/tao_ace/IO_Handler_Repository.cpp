#include "IO_Handler_Repository.h"


#if !defined (__ACE_INLINE__)
#include "IO_Handler_Repository.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, SOCK_Event_Reactive_Handler, "$Id$")


TAO_ACE_IO_Handler_Repository::IO_Handler_State::IO_Handler_State (void)
  : thread_count_ (0),
    marked_for_deletion_ (0),
    event_handler_ (0)
{
}

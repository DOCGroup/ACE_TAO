#include "Fair_Perf_Selection_Strategy.h"

#if !defined (__ACE_INLINE__)
#include "Fair_Performance_Selection_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, Fair_Perf_Selection_Strategy, "$Id$")


int
TAO_ACE_Fair_Perf_Selection_Strategy:event_reselect (void)
{
  return 0;
}


ACE_HANDLE
TAO_ACE_Fair_Perf_Selection_Strategy::dispatchable_io_event (
    TAO_ACE_Select_Strategy_Handle_Set & /*io_set*/,
    TAO_ACE_Event_Masks::Event_Masks & /*t*/)
{
  ACE_NOTSUP_RETURN (ACE_INVALID_HANDLE);
}

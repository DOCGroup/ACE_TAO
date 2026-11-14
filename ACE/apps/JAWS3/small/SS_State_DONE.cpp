#include "SS_State_DONE.h"
#include "SS_Data.h"

int
TeraSS_State_DONE::service (JAWS_Event_Completer *, void *)
{
  // Returning -1 means it is time for the protocol handler to shut
  // itself down.

  return -1;
}

JAWS_Protocol_State *
TeraSS_State_DONE::transition (JAWS_Event_Result const &, void *, void *)
{
  // In the DONE state, this won't get called.

  ACE_ASSERT (false);
  return nullptr;
}


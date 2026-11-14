#include "jaws3/Jaws_IO.h"

#include "SS_State_ERROR.h"
#include "SS_State_DONE.h"
#include "SS_Data.h"

int
TeraSS_State_ERROR::service (JAWS_Event_Completer *ec, void *data)
{
  // Send an error message.
  TeraSS_Data * const tdata = static_cast<TeraSS_Data *> (data);

  tdata->mb ().reset ();
  tdata->mb ().copy ("FAIL\r\n", 6);

  JAWS_IO::instance ()->send (tdata->peer ().get_handle (), &tdata->mb (), ec);

  return 0;
}

JAWS_Protocol_State *
TeraSS_State_ERROR::transition (JAWS_Event_Result const &, void *, void *)
{
  // In the ERROR state, always transition to DONE.

  return TeraSS_State_DONE::instance ();
}


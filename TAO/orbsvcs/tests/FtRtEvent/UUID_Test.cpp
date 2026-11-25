#include "orbsvcs/FtRtEvent/Utils/UUID.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  TAO_FtRt::UUID const valid (ACE_TEXT ("12345678-1234-1234-1234-123456789012"));
  TAO_FtRt::UUID const invalid (ACE_TEXT ("z2345678-1234-1234-1234-123456789012"));

  if (!valid.is_valid () || invalid.is_valid ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("UUID validity check failed\n")), 1);
    }
  return 0;
}

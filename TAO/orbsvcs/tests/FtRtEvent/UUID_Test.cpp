#include "orbsvcs/FtRtEvent/Utils/UUID.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <cstring>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  TAO_FtRt::UUID const valid (ACE_TEXT ("12345678-1234-1234-1234-123456789012"));
  TAO_FtRt::UUID const invalid (ACE_TEXT ("z2345678-1234-1234-1234-123456789012"));

  if (!valid.is_valid () || invalid.is_valid ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("UUID validity check failed\n")), 1);
    }

  char text[TAO_FtRt::UUID::STRING_LENGTH] {};
  valid.to_string (text);
  if (std::strcmp (text, "12345678-1234-1234-1234-123456789012") != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("UUID round trip failed\n")), 1);
    }

  ACE_CString string;
  valid.to_string (string);
  if (string != text)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("UUID string conversion failed\n")), 1);
    }

  TAO_FtRt::UUID updated (ACE_TEXT ("12345678-1234-1234-1234-123456789012"));
  if (updated.from_string ("87654321-1234-1234-1234-12345678901z")
      || updated != valid
      || updated.from_string ("123")
      || updated != valid
      || updated.from_string (nullptr)
      || updated != valid)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid UUID changed its value\n")), 1);
    }
  return 0;
}

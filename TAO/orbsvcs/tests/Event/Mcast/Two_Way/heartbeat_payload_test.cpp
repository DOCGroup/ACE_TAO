/// @file heartbeat_payload_test.cpp

#include "Heartbeat_Payload.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

namespace
{
  bool invalid_payload (size_t length, bool terminate)
  {
    CORBA::OctetSeq payload;
    payload.length (static_cast<CORBA::ULong> (length));
    for (size_t i = 0; i < length; ++i)
      {
        payload[static_cast<CORBA::ULong> (i)] = 'x';
      }
    if (terminate && length > 0)
      {
        payload[static_cast<CORBA::ULong> (length - 1)] = 0;
      }

    pid_t pid {};
    char const *host {};
    size_t host_length {};
    return !Heartbeat_Payload::decode (payload, pid, host, host_length);
  }

  bool valid_payload (size_t length)
  {
    CORBA::OctetSeq payload;
    payload.length (static_cast<CORBA::ULong> (sizeof (pid_t) + length + 1));
    pid_t const expected_pid = 123;
    std::memcpy (payload.get_buffer (), &expected_pid, sizeof (expected_pid));
    for (size_t i = 0; i < length; ++i)
      {
        payload[static_cast<CORBA::ULong> (sizeof (pid_t) + i)] = 'h';
      }
    payload[static_cast<CORBA::ULong> (sizeof (pid_t) + length)] = 0;

    pid_t pid {};
    char const *host {};
    size_t host_length {};
    return Heartbeat_Payload::decode (payload, pid, host, host_length)
      && pid == expected_pid && host_length == length
      && host[length] == '\0';
  }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  if (!invalid_payload (0, false)
      || !invalid_payload (sizeof (pid_t) - 1, false)
      || !invalid_payload (sizeof (pid_t), false)
      || !invalid_payload (sizeof (pid_t) + 3, false)
      || !invalid_payload (sizeof (pid_t) + MAXHOSTNAMELEN + 2, true)
      || !valid_payload (0)
      || !valid_payload (3)
      || !valid_payload (MAXHOSTNAMELEN))
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Heartbeat payload validation failed\n"), 1);
    }
  return 0;
}

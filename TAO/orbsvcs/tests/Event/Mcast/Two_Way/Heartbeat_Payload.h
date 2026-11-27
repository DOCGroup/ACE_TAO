/// @file Heartbeat_Payload.h

#ifndef HEARTBEAT_PAYLOAD_H
#define HEARTBEAT_PAYLOAD_H

#include "orbsvcs/RtecEventCommC.h"
#include "ace/os_include/os_netdb.h"

#include <cstring>

namespace Heartbeat_Payload
{
  inline bool decode (CORBA::OctetSeq const &payload,
                      pid_t &pid,
                      char const *&host,
                      size_t &host_length)
  {
    size_t const length = payload.length ();
    if (length <= sizeof (pid))
      {
        return false;
      }

    CORBA::Octet const * const buffer = payload.get_buffer ();
    char const * const hostname =
      reinterpret_cast<char const *> (buffer + sizeof (pid));
    char const * const end = static_cast<char const *> (
      std::memchr (hostname, '\0', length - sizeof (pid)));
    if (end == nullptr || static_cast<size_t> (end - hostname) > MAXHOSTNAMELEN)
      {
        return false;
      }

    std::memcpy (&pid, buffer, sizeof (pid));
    host = hostname;
    host_length = static_cast<size_t> (end - hostname);
    return true;
  }
}

#endif /* HEARTBEAT_PAYLOAD_H */

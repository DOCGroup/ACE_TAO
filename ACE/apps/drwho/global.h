/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    global.h
 *
 *  Here are all the declarations that are needed throughout the program.
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "ace/config-all.h"
#include "ace/Basic_Types.h"

#include <cstring>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// These constants are used throughout drwho.

enum
{
  MAXUSERIDNAMELEN = 8,
  MAX_USER_TIMEOUT = 300,
  MAX_HOST_TIMEOUT = 300,
  UDP_PACKET_SIZE = 1024 * 8,
  PORT_NUMBER = 12344
};

// Default name of file where friends info is stored.
#define FRIEND_FILE ".friends.dta"

// Default name where rwho info is stored.
#define RWHODIR "/usr/spool/rwho"

// Helpers for handling message types without assuming packet alignment.
inline ACE_UINT16
drwho_get_packet_type (char const *packet)
{
  ACE_UINT16 type {};
  std::memcpy (&type, packet, sizeof type);
  return ACE_NTOHS (type);
}

inline void
drwho_set_packet_type (char *packet, ACE_UINT16 type)
{
  ACE_UINT16 const network_type = ACE_HTONS (type);
  std::memcpy (packet, &network_type, sizeof network_type);
}

inline char *
drwho_skip_packet_type (char *packet)
{
  return packet + sizeof (ACE_UINT16);
}

inline int
drwho_subtract_packet_type (int length)
{
  return length - sizeof (ACE_UINT16);
}

#endif /* _GLOBAL_H */

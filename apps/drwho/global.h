/* -*- C++ -*- */
// $Id$

/* Here are all the declarations that are needed throughout the program. */

#ifndef _GLOBAL_H
#define _GLOBAL_H

/* These constants are used throughout the program... */

enum 
{
  MAXUSERIDNAMELEN 	= 8,
  MAX_USER_TIMEOUT 	= 300, 
  MAX_HOST_TIMEOUT 	= 300, 
#ifdef i386
  UDP_PACKET_SIZE 	= 0x2000,
#else
  UDP_PACKET_SIZE 	= 0x4000,
#endif
  PORT_NUMBER 		= 12344
/*  MAXPATHLEN 		= 1024, 
    MAXHOSTNAMELEN 	= 64 + 1 */
};

/* Default name of file where friends info is stored. */
#define FRIEND_FILE ".friends.dta"

/* Default name where rwho info is stored. */
#define RWHODIR "/usr/spool/rwho"

/* Daemon creator. */
extern void daemon_start (int ignsigcld);

#endif /* _GLOBAL_H */

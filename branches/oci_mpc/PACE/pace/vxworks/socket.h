/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/socket.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_SOCKET_H_VXWORKS
#define PACE_SOCKET_H_VXWORKS

#include <sys/types.h>
#include <sockLib.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_SOMAXCONN
#define PACE_SOMAXCONN SOMAXCONN
#endif /* PACE_SOMAXCONN */

#ifndef PACE_SOCKLEN_T
#define PACE_SOCKLEN_T
    typedef int pace_socklen_t;
#endif /* PACE_SOCKLEN_T */

#ifndef PACE_SA_FAMILY_T
#define PACE_SA_FAMILY_T
#endif /* PACE_SA_FAMILY_T */

#ifndef PACE_MSGHDR
#define PACE_MSGHDR
    typedef struct msghdr pace_msghdr;
#endif /* PACE_MSGHDR */

#ifndef PACE_CMSGRHDR
#define PACE_CMSGRHDR
    typedef struct cmsghdr pace_cmsghdr;
#endif /* PACE_CMSGRHDR */

#ifndef PACE_LINGER
#define PACE_LINGER
    typedef struct linger pace_linger;
#endif /* PACE_LINGER */

#ifndef PACE_SOCK_STREAM
#define PACE_SOCK_STREAM SOCK_STREAM
#endif /* PACE_SOCK_STREAM */

#ifndef PACE_SOCK_DGRAM
#define PACE_SOCK_DGRAM SOCK_DGRAM
#endif /* PACE_SOCK_DGRAM */

#ifndef PACE_SOCK_RAW
#define PACE_SOCK_RAW SOCK_RAW
#endif /* PACE_MSGHDR */

#ifndef PACE_SOCK_SEQPACKET
#define PACE_SOCK_SEQPACKET SOCK_SEQPACKET
#endif /* PACE_SOCK_SEQPACKET */

#ifndef PACE_PF_UNSPEC
#define PACE_PF_UNSPEC PF_UNSPEC
#endif /* PACE_PF_UNSPEC */

#ifndef PACE_PF_LOCAL
#define PACE_PF_LOCAL PF_LOCAL
#endif /* PACE_PF_LOCAL */

#ifndef PACE_PF_INET
#define PACE_PF_INET PF_INET
#endif /* PACE_PF_INET */

#ifndef PACE_PF_ISO
#define PACE_PF_ISO PF_ISO
#endif /* PACE_PF_ISO */

#ifndef PACE_PF_OSI
#define PACE_PF_OSI PF_OSI
#endif /* PACE_PF_OSI */

#ifndef PACE_AF_UNSPEC
#define PACE_AF_UNSPEC AF_UNSPEC
#endif /* PACE_AF_UNSPEC */

#ifndef PACE_AF_LOCAL
#define PACE_AF_LOCAL AF_LOCAL
#endif /* PACE_AF_LOCAL */

#ifndef PACE_AF_INET
#define PACE_AF_INET AF_INET
#endif /* PACE_AF_INET */

#ifndef PACE_AF_ISO
#define PACE_AF_ISO AF_ISO
#endif /* PACE_AF_ISO */

#ifndef PACE_AF_OSI
#define PACE_AF_OSI AF_OSI
#endif /* PACE_AF_OSI */

#ifndef PACE_MSG_OOB
#define PACE_MSG_OOB MSG_OOB
#endif /* PACE_MSG_OOB */

#ifndef PACE_MSG_PEEK
#define PACE_MSG_PEEK MSG_PEEK
#endif /* PACE_MSG_PEEK */

#ifndef PACE_MSG_DONTROUTE
#define PACE_MSG_DONTROUTE MSG_DONTROUTE
#endif /* PACE_MSG_DONTROUTE */

#ifndef PACE_MSG_EOR
#define PACE_MSG_EOR MSG_EOR
#endif /* PACE_MSG_EOR */

#ifndef PACE_MSG_TRUNC
#define PACE_MSG_TRUNC MSG_TRUNC
#endif /* PACE_MSG_TRUNC */

#ifndef PACE_MSG_CTRUNC
#define PACE_MSG_CTRUNC MSG_CTRUNC
#endif /* PACE_MSG_CTRUNC */

#ifndef PACE_MSG_WAITALL
#define PACE_MSG_WAITALL MSG_WAITALL
#endif /* PACE_MSG_WAITALL */

#ifndef PACE_SOL_SOCKET
#define PACE_SOL_SOCKET SOL_SOCKET
#endif /* PACE_SOL_SOCKET */

#ifndef PACE_SO_BROADCAST
#define PACE_SO_BROADCAST SO_BROADCAST
#endif /* PACE_SO_BROADCAST */

#ifndef PACE_SO_DEBUG
#define PACE_SO_DEBUG SO_DEBUG
#endif /* PACE_SO_DEBUG */

#ifndef PACE_SO_DONTROUTE
#define PACE_SO_DONTROUTE SO_DONTROUTE
#endif /* PACE_SO_DONTROUTE */

#ifndef PACE_SO_ERROR
#define PACE_SO_ERROR SO_ERROR
#endif /* PACE_SO_ERROR */

#ifndef PACE_SO_KEEPALIVE
#define PACE_SO_KEEPALIVE SO_KEEPALIVE
#endif /* PACE_SO_KEEPALIVE */

#ifndef PACE_SO_LINGER
#define PACE_SO_LINGER SO_LINGER
#endif /* PACE_SO_LINGER */

#ifndef PACE_SO_OOBINLINE
#define PACE_SO_OOBINLINE SO_OOBINLINE
#endif /* PACE_SO_OOBINLINE */

#ifndef PACE_SO_RCVBUF
#define PACE_SO_RCVBUF SO_RCVBUF
#endif /* PACE_SO_RCVBUF */

#ifndef PACE_SO_RCVLOWAT
#define PACE_SO_RCVLOWAT SO_RCVLOWAT
#endif /* PACE_SO_RCVLOWAT */

#ifndef PACE_SO_RCVTIMEO
#define PACE_SO_RCVTIMEO SO_RCVTIMEO
#endif /* PACE_SO_RCVTIMEO */

#ifndef PACE_SO_REUSEADDR
#define PACE_SO_REUSEADDR SO_REUSEADDR
#endif /* PACE_SO_REUSEADDR */

#ifndef PACE_SO_SNDBUF
#define PACE_SO_SNDBUF SO_SNDBUF
#endif /* PACE_SO_SNDBUF */

#ifndef PACE_SO_SNDLOWAT
#define PACE_SO_SNDLOWAT SO_SNDLOWAT
#endif /* PACE_SO_SNDLOWAT */

#ifndef PACE_SO_SNDTIMEO
#define PACE_SO_SNDTIMEO SO_SNDTIMEO
#endif /* PACE_SO_SNDTIMEO */

#ifndef PACE_SO_TYPE
#define PACE_SO_TYPE SO_TYPE
#endif /* PACE_SO_TYPE */

#ifndef PACE_CMSG_DATA
#define PACE_CMSG_DATA(cmsg)          CMSG_DATA(cmsg)
#endif /* PACE_CMSG_DATA */

#ifndef PACE_CMSG_NXTHDR
#define PACE_CMSG_NXTHDR(mhdr, cmsg)  CMSG_NXTHDR(mhdr, cmsg)
#endif /* PACE_CMSG_DATA */

#ifndef PACE_CMSG_FIRSTHDR
#define PACE_CMSG_NXTHDR(mhdr, cmsg)  CMSG_NXTHDR(mhdr, cmsg)
#endif /* PACE_CMSG_DATA */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SOCKET_H_VXWORKS */

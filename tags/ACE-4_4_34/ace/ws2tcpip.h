/*	
**	WS2TCPIP.H - WinSock2 Extension for TCP/IP protocols
**
**	This file contains TCP/IP specific information for use 
**	by WinSock2 compatible applications. 
**
**	To provide the backward compatibility, all the TCP/IP  
**	specific definitions that were included in the WINSOCK.H
**	 file are now included in WINSOCK2.H file. WS2TCPIP.H 
**	file includes only the definitions  introduced in the 
**	"WinSock 2 Protocol-Specific Annex" document.
**
**	Rev 0.3	Nov 13, 1995
*/

#ifndef _WS2TCPIP_H_
#define _WS2TCPIP_H_

/* Structure to keep interface specific information */

typedef struct _INTERFACE_INFO
{
	u_long		iiFlags;		/* Interface flags */
	struct sockaddr	iiAddress;		/* Interface address */
	struct sockaddr	iiBroadcastAddress; 	/* Broadcast address */
	struct sockaddr	iiNetmask;		/* Network mask */
} INTERFACE_INFO;

/* Possible flags for the  iiFlags - bitmask  */ 

#define IFF_UP		0x00000001 /* Interface is up */
#define IFF_BROADCAST	0x00000002 /* Broadcast is  supported */
#define IFF_LOOPBACK	0x00000004 /* this is loopback interface */
#define IFF_POINTTOPOINT 0x00000008 /*this is point-to-point 
interface*/
#define IFF_MULTICAST	0x00000010 /* multicast is supported */

/* Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP */

struct ip_mreq {
	struct in_addr imr_multiaddr;	/* IP multicast address of group */
	struct in_addr imr_interface;	/* local IP address of interface */
};

/* TCP/IP specific Ioctl codes */

#define SIO_GET_INTERFACE_LIST 	<TBD>

/* Option to use with [gs]etsockopt at the IPPROTO_IP level */

#define	IP_OPTIONS			1 /* set/get IP options */
#define	IP_HDRINCL			2 /* header is included with data */
#define	IP_TOS			3 /* IP type of service and preced*/
#define	IP_TTL			4 /* IP time to live */
#define	IP_MULTICAST_IF		9 /* set/get IP multicast i/f  */
#define	IP_MULTICAST_TTL	     10 /* set/get IP multicast ttl */
#define	IP_MULTICAST_LOOP	     11 /*set/get IP multicast loopback */
#define	IP_ADD_MEMBERSHIP	     12 /* add an IP group membership */
#define	IP_DROP_MEMBERSHIP     13/* drop an IP group membership */


/* Option to use with [gs]etsockopt at the IPPROTO_UDP level */

#define UDP_NOCHECKSUM	1

/* Option to use with [gs]etsockopt at the IPPROTO_TCP level */

#define  TCP_EXPEDITED_1122	0x0002


/* IPv6 definitions */

struct in_addr6 {
		u_char	s6_addr[16];	/* IPv6 address */
};

struct sockaddr_in6 {
		short		sin6_family;   /* AF_INET6 */
		u_short   	sin6_port;	   /* Transport level port numb
er */
		u_long	sin6_flowinfo; /* IPv6 flow information */
		struct in_addr6 	sin6_addr; /* IPv6 address */
};


#endif	/* _WS2TCPIP_H_ */

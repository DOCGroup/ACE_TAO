/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    INET_Addr.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_INET_ADDR_H
#define ACE_INET_ADDR_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"

#if defined (ACE_HAS_IPV6)
#define ACE_INADDR_ANY in6addr_any
#define ACE_AF_INET AF_INET6
#else
#define ACE_INADDR_ANY INADDR_ANY
#define ACE_AF_INET AF_INET
#endif

/**
 * @class ACE_INET_Addr
 *
 * @brief Defines a C++ wrapper facade for the Internet domain address
 * family format.
 */
class ACE_Export ACE_INET_Addr : public ACE_Addr
{
public:
#if defined (ACE_HAS_IPV6)
  typedef sockaddr_in6 ace_sockaddr_in_t;
  typedef in6_addr ace_in_addr_t;
#else
  typedef sockaddr_in ace_sockaddr_in_t;
  typedef ACE_UINT32 ace_in_addr_t;
#endif

  // = Initialization methods.
  /// Default constructor.
  ACE_INET_Addr (void);

  /// Copy constructor.
  ACE_INET_Addr (const ACE_INET_Addr &);

  /// Creates an <ACE_INET_Addr> from a ace_sockaddr_in structure.
  ACE_INET_Addr (const sockaddr_in *, int len);

  /// Creates an <ACE_INET_Addr> from a sockaddr_in structure.
  int set (const sockaddr_in *,
	   int len);


  /// Creates an <ACE_INET_Addr> from a <port_number> and the remote
  /// <host_name>.
  ACE_INET_Addr (u_short port_number,
                 const char host_name[]);

  /**
   * Initializes an <ACE_INET_Addr> from the <address>, which can be
   * "ip-number:port-number" (e.g., "tango.cs.wustl.edu:1234" or
   * "128.252.166.57:1234").  If there is no ':' in the <address> it
   * is assumed to be a port number, with the IP address being
   * ACE_INADDR_ANY.
   */
  ACE_EXPLICIT ACE_INET_Addr (const char address[]);

  /**
   * Creates an <ACE_INET_Addr> from a <port_number> and an Internet
   * <ip_addr>.  This method assumes that <port_number> and <ip_addr>
   * are in host byte order.
   */
  ACE_INET_Addr (u_short port_number,
		 ace_in_addr_t ip_addr = ACE_INADDR_ANY);

  /// Uses <getservbyname> to create an <ACE_INET_Addr> from a
  /// <port_name>, the remote <host_name>, and the <protocol>.
  ACE_INET_Addr (const char port_name[],
		 const char host_name[],
                 const char protocol[] = "tcp");

#if !defined (ACE_HAS_IPV6)
  /**
   * Uses <getservbyname> to create an <ACE_INET_Addr> from a
   * <port_name>, an Internet <ip_addr>, and the <protocol>.  This
   * method assumes that <ip_addr> is in host byte order.
   */
  ACE_INET_Addr (const char port_name[],
		 ace_in_addr_t ip_addr,
                 const char protocol[] = "tcp");
#endif

#if defined (ACE_HAS_WCHAR)
  ACE_INET_Addr (u_short port_number,
                 const wchar_t host_name[]);

  ACE_EXPLICIT ACE_INET_Addr (const wchar_t address[]);

  ACE_INET_Addr (const wchar_t port_name[],
		 const wchar_t host_name[],
                 const wchar_t protocol[] = ACE_TEXT_WIDE ("tcp"));

#if !defined (ACE_HAS_IPV6)
  ACE_INET_Addr (const wchar_t port_name[],
		 ace_in_addr_t ip_addr,
                 const wchar_t protocol[] = ACE_TEXT_WIDE ("tcp"));
#endif /* ! ACE_HAS_IPV6 */
#endif /* ACE_HAS_WCHAR */

  /// Default dtor.
  ~ACE_INET_Addr (void);

  // = Direct initialization methods.

  // These methods are useful after the object has been constructed.

  /// Initializes from another <ACE_INET_Addr>.
  int set (const ACE_INET_Addr &);

  /**
   * Initializes an <ACE_INET_Addr> from a <port_number> and the
   * remote <host_name>.  If <encode> is enabled then <port_number> is
   * converted into network byte order, otherwise it is assumed to be
   * in network byte order already and are passed straight through.
   */
  int set (u_short port_number,
	   const char host_name[],
           int encode = 1);

  // Initializes an <ACE_INET_Addr> from a <port_number> and the
  // remote <host_name>.  If <encode> is enabled then <port_number> is
  // converted into network byte order, otherwise it is assumed to be
  // in network byte order already and are passed straight through.
  // Note: if ACE_HAS_IPV6 is defined, this will create a IPv4 address
  // inside the IPv6 structure.  This interface is likely to be depreciated
  // very soon.
  int set (u_short port_number,
           ACE_UINT32 ip_addr = INADDR_ANY,
           int encode = 1);

  /// Uses <getservbyname> to initialize an <ACE_INET_Addr> from a
  /// <port_name>, the remote <host_name>, and the <protocol>.
  int set (const char port_name[],
	   const char host_name[],
           const char protocol[] = "tcp");

  /**
   * Initializes an <ACE_INET_Addr> from the <addr>, which can be
   * "ip-number:port-number" (e.g., "tango.cs.wustl.edu:1234" or
   * "128.252.166.57:1234").  If there is no ':' in the <address> it
   * is assumed to be a port number, with the IP address being
   * ACE_INADDR_ANY.
   */
  int set (const char addr[]);

#if defined (ACE_HAS_WCHAR)
  int set (u_short port_number,
	   const wchar_t host_name[],
           int encode = 1);

  int set (const wchar_t port_name[],
	   const wchar_t host_name[],
           const wchar_t protocol[] = ACE_TEXT_WIDE ("tcp"));

#if !defined (ACE_HAS_IPV6)
  int set (const wchar_t port_name[],
	   ace_in_addr_t ip_addr,
           const wchar_t protocol[] = ACE_TEXT_WIDE ("tcp"));

  int set (const wchar_t addr[]);
#endif /* ! ACE_HAS_IPV6 */
#endif /* ACE_HAS_WCHAR */

  /// Return a pointer to the underlying network address.
  virtual void *get_addr (void) const;

  /// Set a pointer to the address.
  virtual void set_addr (void *, int len);

  /**
   * Transform the current <ACE_INET_Addr> address into string format.
   * If <ipaddr_format> is non-0 this produces "ip-number:port-number"
   * (e.g., "128.252.166.57:1234"), whereas if <ipaddr_format> is 0
   * this produces "ip-name:port-number" (e.g.,
   * "tango.cs.wustl.edu:1234").  Returns -1 if the <size> of the
   * <buffer> is too small, else 0.
   */
  virtual int addr_to_string (ACE_TCHAR buffer[],
                              size_t size,
                              int ipaddr_format = 1) const;

  /**
   * Initializes an <ACE_INET_Addr> from the <address>, which can be
   * "ip-addr:port-number" (e.g., "tango.cs.wustl.edu:1234"),
   * "ip-addr:port-name" (e.g., "tango.cs.wustl.edu:telnet"),
   * "ip-number:port-number" (e.g., "128.252.166.57:1234"), or
   * "ip-number:port-name" (e.g., "128.252.166.57:telnet").  If there
   * is no ':' in the <address> it is assumed to be a port number,
   * with the IP address being ACE_INADDR_ANY.
   */
  virtual int string_to_addr (const char address[]);

#if defined (ACE_HAS_WCHAR)
  /*
  virtual int string_to_addr (const char address[]);
  */
#endif /* ACE_HAS_WCHAR */


  /**
   * Sets the port number without affecting the host name.  If
   * <encode> is enabled then <port_number> is converted into network
   * byte order, otherwise it is assumed to be in network byte order
   * already and are passed straight through.
   */
  void set_port_number (u_short,
			int encode = 1);

  /// Return the port number, converting it into host byte order.
  u_short get_port_number (void) const;

  /**
   * Return the character representation of the name of the host,
   * storing it in the <hostname> (which is assumed to be
   * <hostnamelen> bytes long).  This version is reentrant.
   */
  int get_host_name (char hostname[],
                     size_t hostnamelen) const;

#if defined (ACE_HAS_WCHAR)
  int get_host_name (wchar_t hostname[],
                     size_t hostnamelen) const;
#endif /* ACE_HAS_WCHAR */

  /**
   * Return the character representation of the hostname (this version
   * is non-reentrant since it returns a pointer to a static data
   * area).
   */
  const char *get_host_name (void) const;

  /// Return the "dotted decimal" Internet address.
  const char *get_host_addr (void) const;

  /// Return the "dotted decimal" Internet address into the string provided.
  const char *get_host_addr (char *dst, int size) const;

  /// Return the 4-byte IP address, converting it into host byte
  /// order.
  // XXXXXXXXXXX This needs to have something done with it for IPv6 addressing
  // Multicast depends on this.  We need to figure out what to do with this.
  ACE_UINT32 get_ip_address (void) const;

  /**
   * Returns true if <this> is less than <rhs>.  In this context,
   * "less than" is defined in terms of IP address and TCP port
   * number.  This operator makes it possible to use <ACE_INET_Addr>s
   * in STL maps.
   */
  int operator < (const ACE_INET_Addr &rhs) const;

  /// Compare two addresses for equality.  The addresses are considered
  /// equal if they contain the same IP address and port number.
  int operator == (const ACE_INET_Addr &SAP) const;

  /// Compare two addresses for inequality.
  int operator != (const ACE_INET_Addr &SAP) const;

  /// Computes and returns hash value.
  virtual u_long hash (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

 protected:

  /**
   * Initializes an <ACE_INET_Addr> from a <port_number> and an
   * Internet <ip_addr>.  If <encode> is enabled then <port_number>
   * and <ip_addr> are converted into network byte order, otherwise
   * they are assumed to be in network byte order already and are
   * passed straight through.
   */
#if defined (ACE_HAS_IPV6)
  // We need this method for IPv6.  It is defined up above in the public
  // section for IPv4.
  int set (u_short port_number,
           ace_in_addr_t ip_addr,
           int encode = 1);
  /// Creates an <ACE_INET_Addr> from a sockaddr_in structure.
  int set (const ace_sockaddr_in_t *,
	   int len);
#endif

  /**
   * Uses <getservbyname> to initialize an <ACE_INET_Addr> from a
   * <port_name>, an <ip_addr>, and the <protocol>.  This assumes that
   * <ip_addr> is already in network byte order.
   */
  int set (const char port_name[],
	   ace_in_addr_t ip_addr,
           const char protocol[] = "tcp");

private:
  /// Underlying representation.
  ace_sockaddr_in_t inet_addr_;
};

#if defined (__ACE_INLINE__)
#include "ace/INET_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_INET_ADDR_H */

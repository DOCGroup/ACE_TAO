/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    INET_Addr.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_INET_ADDR_H
#define ACE_INET_ADDR_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"

class ACE_Export ACE_INET_Addr : public ACE_Addr
{
  // = TITLE
  //    Defines a C++ wrapper facade for the Internet domain address
  //    family format.
public:
  // = Initialization methods.
  ACE_INET_Addr (void);
  // Default constructor.

  ACE_INET_Addr (const ACE_INET_Addr &);
  // Copy constructor.

  ACE_INET_Addr (const sockaddr_in *, int len);
  // Creates an <ACE_INET_Addr> from a sockaddr_in structure.

  ACE_INET_Addr (u_short port_number,
                 const ACE_TCHAR host_name[]);
  // Creates an <ACE_INET_Addr> from a <port_number> and the remote
  // <host_name>.

  ACE_INET_Addr (const ACE_TCHAR address[]);
  // Initializes an <ACE_INET_Addr> from the <address>, which can be
  // "ip-number:port-number" (e.g., "tango.cs.wustl.edu:1234" or
  // "128.252.166.57:1234").  If there is no ':' in the <address> it
  // is assumed to be a port number, with the IP address being
  // INADDR_ANY.

  ACE_INET_Addr (u_short port_number,
		 ACE_UINT32 ip_addr = INADDR_ANY);
  // Creates an <ACE_INET_Addr> from a <port_number> and an Internet
  // <ip_addr>.  This method assumes that <port_number> and <ip_addr>
  // are in host byte order.

  ACE_INET_Addr (const ACE_TCHAR port_name[],
		 const ACE_TCHAR host_name[],
                 const ACE_TCHAR protocol[] = ACE_TEXT ("tcp"));
  // Uses <getservbyname> to create an <ACE_INET_Addr> from a
  // <port_name>, the remote <host_name>, and the <protocol>.

  ACE_INET_Addr (const ACE_TCHAR port_name[],
		 ACE_UINT32 ip_addr,
                 const ACE_TCHAR protocol[] = ACE_TEXT ("tcp"));
  // Uses <getservbyname> to create an <ACE_INET_Addr> from a
  // <port_name>, an Internet <ip_addr>, and the <protocol>.  This
  // method assumes that <ip_addr> is in host byte order.

  ~ACE_INET_Addr (void);
  // Default dtor.

  // = Direct initialization methods.

  // These methods are useful after the object has been constructed.

  int set (const ACE_INET_Addr &);
  // Initializes from another <ACE_INET_Addr>.

  int set (u_short port_number,
	   const ACE_TCHAR host_name[],
           int encode = 1);
  // Initializes an <ACE_INET_Addr> from a <port_number> and the
  // remote <host_name>.  If <encode> is enabled then <port_number> is
  // converted into network byte order, otherwise it is assumed to be
  // in network byte order already and are passed straight through.

  int set (u_short port_number,
           ACE_UINT32 ip_addr = INADDR_ANY,
           int encode = 1);
  // Initializes an <ACE_INET_Addr> from a <port_number> and an
  // Internet <ip_addr>.  If <encode> is enabled then <port_number>
  // and <ip_addr> are converted into network byte order, otherwise
  // they are assumed to be in network byte order already and are
  // passed straight through.

  int set (const ACE_TCHAR port_name[],
	   const ACE_TCHAR host_name[],
           const ACE_TCHAR protocol[] = ACE_TEXT ("tcp"));
  // Uses <getservbyname> to initialize an <ACE_INET_Addr> from a
  // <port_name>, the remote <host_name>, and the <protocol>.

  int set (const ACE_TCHAR port_name[],
	   ACE_UINT32 ip_addr,
           const ACE_TCHAR protocol[] = ACE_TEXT ("tcp"));
  // Uses <getservbyname> to initialize an <ACE_INET_Addr> from a
  // <port_name>, an <ip_addr>, and the <protocol>.  This assumes that
  // <ip_addr> is already in network byte order.

  int set (const ACE_TCHAR addr[]);
  // Initializes an <ACE_INET_Addr> from the <addr>, which can be
  // "ip-number:port-number" (e.g., "tango.cs.wustl.edu:1234" or
  // "128.252.166.57:1234").  If there is no ':' in the <address> it
  // is assumed to be a port number, with the IP address being
  // INADDR_ANY.

  int set (const sockaddr_in *,
	   int len);
  // Creates an <ACE_INET_Addr> from a sockaddr_in structure.

  virtual void *get_addr (void) const;
  // Return a pointer to the underlying network address.

  virtual void set_addr (void *, int len);
  // Set a pointer to the address.

  virtual int addr_to_string (ACE_TCHAR buffer[],
                              size_t size,
                              int ipaddr_format = 1) const;
  // Transform the current <ACE_INET_Addr> address into string format.
  // If <ipaddr_format> is non-0 this produces "ip-number:port-number"
  // (e.g., "128.252.166.57:1234"), whereas if <ipaddr_format> is 0
  // this produces "ip-name:port-number" (e.g.,
  // "tango.cs.wustl.edu:1234").  Returns -1 if the <size> of the
  // <buffer> is too small, else 0.

  virtual int string_to_addr (const ACE_TCHAR address[]);
  // Initializes an <ACE_INET_Addr> from the <address>, which can be
  // "ip-addr:port-number" (e.g., "tango.cs.wustl.edu:1234"),
  // "ip-addr:port-name" (e.g., "tango.cs.wustl.edu:telnet"),
  // "ip-number:port-number" (e.g., "128.252.166.57:1234"), or
  // "ip-number:port-name" (e.g., "128.252.166.57:telnet").  If there
  // is no ':' in the <address> it is assumed to be a port number,
  // with the IP address being INADDR_ANY.

  void set_port_number (u_short,
			int encode = 1);
  // Sets the port number without affecting the host name.  If
  // <encode> is enabled then <port_number> is converted into network
  // byte order, otherwise it is assumed to be in network byte order
  // already and are passed straight through.

  u_short get_port_number (void) const;
  // Return the port number, converting it into host byte order.

  int get_host_name (ACE_TCHAR hostname[],
                     size_t hostnamelen) const;
  // Return the character representation of the name of the host,
  // storing it in the <hostname> (which is assumed to be
  // <hostnamelen> bytes long).  This version is reentrant.

  const ACE_TCHAR *get_host_name (void) const;
  // Return the character representation of the hostname (this version
  // is non-reentrant since it returns a pointer to a static data
  // area).

  const ACE_TCHAR *get_host_addr (void) const;
  // Return the "dotted decimal" Internet address.

  ACE_UINT32 get_ip_address (void) const;
  // Return the 4-byte IP address, converting it into host byte
  // order.

  int operator == (const ACE_INET_Addr &SAP) const;
  // Compare two addresses for equality.  The addresses are considered
  // equal if they contain the same IP address and port number.

  int operator != (const ACE_INET_Addr &SAP) const;
  // Compare two addresses for inequality.

  virtual u_long hash (void) const;
  // Computes and returns hash value.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  sockaddr_in inet_addr_;
  // Underlying representation.
};

#if defined (__ACE_INLINE__)
#include "ace/INET_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_INET_ADDR_H */

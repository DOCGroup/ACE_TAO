/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    MEM_Addr.h
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_MEM_ADDR_H
#define ACE_MEM_ADDR_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

class ACE_Export ACE_MEM_Addr : public ACE_Addr
{
  // = TITLE
  //    Defines a C++ wrapper facade for the shared memory transport
  //    address family format.
public:
  // = Initialization methods.
  ACE_MEM_Addr (void);
  // Default constructor.

  ACE_MEM_Addr (const ACE_MEM_Addr &);
  // Copy constructor.

  ACE_MEM_Addr (u_short port_number);
  // Creates an <ACE_INET_Addr> from a <port_number> and the remote
  // <host_name>.

  ACE_MEM_Addr (const ASYS_TCHAR port_name[]);
  // Creates an <ACE_INET_Addr> from a <port_name>.

  ~ACE_MEM_Addr (void);
  // Default dtor.

  // = Direct initialization methods.

  int initialize_local (u_short port);
  // default initialization routine.

  int same_host (const ACE_INET_Addr& sap);
  // Check if <sap> designate an enpoint withing the same host.

  // These methods are useful after the object has been constructed.

  int set (u_short port_number,
           int encode = 1);
  // Initializes an <ACE_INET_Addr> from a <port_number> and the
  // remote <host_name>.  If <encode> is enabled then <port_number> is
  // converted into network byte order, otherwise it is assumed to be
  // in network byte order already and are passed straight through.

  int set (const ASYS_TCHAR port_name[]);
  // Uses <getservbyname> to initialize an <ACE_INET_Addr> from a
  // <port_name>, the remote <host_name>, and the <protocol>.

  virtual void *get_addr (void) const;
  // Return a pointer to the underlying network address.

  virtual void set_addr (void *, int len);
  // Set a pointer to the address.

  virtual int addr_to_string (ASYS_TCHAR buffer[],
                              size_t size,
                              int ipaddr_format = 1) const;
  // Transform the external <ACE_INET_Addr> address into string
  // format.

  virtual int string_to_addr (const ASYS_TCHAR address[]);
  // Initializes the external <ACE_INET_Addr> from the <address>.

  void set_port_number (u_short,
                        int encode = 1);
  // Sets the port number.

  u_short get_port_number (void) const;
  // Return the port number, converting it into host byte order.

  int get_host_name (ASYS_TCHAR hostname[],
                     size_t hostnamelen) const;
  // Return the character representation of the hostname.

  const ASYS_TCHAR *get_host_name (void) const;
  // Return the character representation of the hostname (this version
  // is non-reentrant since it returns a pointer to a static data
  // area).

  const char *get_host_addr (void) const;
  // Return the "dotted decimal" external address.

  ACE_UINT32 get_ip_address (void) const;
  // Return the 4-byte external IP address, converting it into host byte
  // order.

  const ACE_INET_Addr &get_remote_addr (void) const;
  const ACE_INET_Addr &get_local_addr (void) const;

  int operator == (const ACE_MEM_Addr &SAP) const;
  int operator == (const ACE_INET_Addr &SAP) const;
  // Compare two addresses for equality.  The addresses are considered
  // equal if they contain the same IP address and port number.

  int operator != (const ACE_MEM_Addr &SAP) const;
  int operator != (const ACE_INET_Addr &SAP) const;
  // Compare two addresses for inequality.

  virtual u_long hash (void) const;
  // Computes and returns hash value.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_INET_Addr external_;
  // External INET addr used for identifying host.

  ACE_INET_Addr internal_;
  // Internal INET addr for accepting/connecting.
};

#if defined (__ACE_INLINE__)
#include "ace/MEM_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_MEM_ADDR_H */

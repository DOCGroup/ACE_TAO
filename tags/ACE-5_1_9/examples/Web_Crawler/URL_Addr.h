/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Web_Crawler
//
// = FILENAME
//    URL_Addr.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_URL_ADDR_H
#define ACE_URL_ADDR_H

#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_URL_Addr : public ACE_INET_Addr
{
  // = TITLE
  //    Defines a URL address family address format.
public:
  // = Initialization and termination methods.
  ACE_URL_Addr (void);
  // Constructor.

  ACE_URL_Addr (const ACE_TCHAR *host_name,
                const ACE_TCHAR *path_name,
                u_short port = ACE_DEFAULT_HTTP_PORT);

  ACE_URL_Addr (const ACE_URL_Addr &addr);
  // Copy constructor.

  int set (const ACE_URL_Addr &addr);
  // Essentially the copy constructor.

  virtual int string_to_addr (const ACE_TCHAR *address);
  // Initializes an <ACE_URL_Addr> from the <address>, which can be
  // "ip-number:port-number/path-name" (e.g.,
  // "www.cs.wustl.edu:1234/~schmidt/" "ip-number:port-number/path-name"
  // (e.g., "128.252.166.57:1234/~schmidt").  If there is no ':' in
  // the <address> it is assumed to be an ip-number or ip-address
  // number, with the port number <ACE_DEFAULT_HTTP_PORT>.

  virtual int addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int ipaddr_format = 1) const;
  // Transform the current <ACE_INET_Addr> address into string format.
  // If <ipaddr_format> is non-0 this produces
  // "ip-number:port-number/path-name" (e.g.,
  // "128.252.166.57:80/~schmidt/"), whereas if <ipaddr_format> is 0
  // this produces "ip-name:port-number" (e.g.,
  // "www.cs.wustl.edu:80/~schmidt/").  Returns -1 if the <size> of
  // the <buffer> is too small, else 0.

  virtual const ACE_TCHAR *addr_to_string (int ipaddr_format = 1) const;
  // Transform the current <ACE_INET_Addr> address into string format.
  // If <ipaddr_format> is non-0 this produces
  // "ip-number:port-number/path-name" (e.g.,
  // "128.252.166.57:80/~schmidt/"), whereas if <ipaddr_format> is 0
  // this produces "ip-name:port-number" (e.g.,
  // "www.cs.wustl.edu:80/~schmidt/").  Uses dynamic memory, which
  // is allocated on demand and deallocated when the object is
  // destroyed.  Returns -1 if dynamic memory fails, else 0.

  void operator= (const ACE_URL_Addr &addr);
  // Assignment operator.

  ~ACE_URL_Addr (void);
  // Destructor.

  int operator == (const ACE_URL_Addr &SAP) const;
  // Compare two addresses for equality.  The addresses are considered
  // equal if they contain the same IP address, port number, and path
  // name.

  int operator != (const ACE_URL_Addr &SAP) const;
  // Compare two addresses for inequality.

  virtual u_long hash (void) const;
  // Computes and returns hash value.

  const ACE_TCHAR *get_path_name (void) const;
  // Return the path name.

  int destroy (void);
  // Commit suicide.
private:
  ACE_TCHAR *path_name_;
  // Our path name.

  ACE_TCHAR *addr_string_;
  // The dynamically address string that's used for the
  // <addr_to_string> method.

  size_t addr_string_len_;
  // Current length of the <addr_string_>
};

#endif /* ACE_URL_ADDR_H */

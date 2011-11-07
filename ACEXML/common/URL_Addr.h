/* -*- C++ -*- */

/** @file   URL_Addr.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 *
 */

#ifndef _ACEXML_URL_ADDR_H
#define _ACEXML_URL_ADDR_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Default_Constants.h"
#include "ace/INET_Addr.h"
#include "ACEXML/common/XML_Types.h"
#include "ACEXML/common/ACEXML_Export.h"

/**
 *  @class ACEXML_URL_Addr
 *
 *  Defines a URL address family address format.
 */
class ACEXML_Export ACEXML_URL_Addr : public ACE_INET_Addr
{
public:
  /// Initialization and termination methods.
  ACEXML_URL_Addr (void);

  /// Constructor.
  ACEXML_URL_Addr (const ACEXML_Char *host_name,
                   const ACEXML_Char *path_name,
                   unsigned short port = ACE_DEFAULT_HTTP_PORT);

  /// Copy constructor.
  ACEXML_URL_Addr (const ACEXML_URL_Addr &addr);

  /// Essentially the copy constructor.
  int set (const ACEXML_URL_Addr &addr);

  /**
   *  Initializes an <ACEXML_URL_Addr> from the <address>, which can be
   *  "ip-number:port-number/path-name" (e.g.,
   *  "www.cs.wustl.edu:1234/~schmidt/" "ip-number:port-number/path-name"
   *  (e.g., "128.252.166.57:1234/~schmidt"). If there is no ':' in the
   *  <address> it is assumed to be an ip-number or ip-address number, with
   *  the port number <ACE_DEFAULT_HTTP_PORT>.
   */
  virtual int string_to_addr (const ACEXML_Char* address,
                              int address_family = AF_UNSPEC);

  //  Function to shut up Borland C++
#if defined (ACE_USES_WCHAR)
  virtual int string_to_addr (const char* addresss,
                              int address_family = AF_UNSPEC);
#endif

  /**
   *  Transform the current <ACE_INET_Addr> address into string format. If
   *  <ipaddr_format> is non-0 this produces
   *  "ip-number:port-number/path-name" (e.g.,
   *  "128.252.166.57:80/~schmidt/"), whereas if <ipaddr_format> is 0 this
   *  produces "ip-name:port-number" (e.g.,
   *  "www.cs.wustl.edu:80/~schmidt/"). Returns -1 if the <size> of the
   *  <buffer> is too small, else 0.
   */
  virtual int addr_to_string (ACEXML_Char *s,
                              size_t size,
                              int ipaddr_format = 1) const;

  /**
   *  Transform the current <ACE_INET_Addr> address into string format. If
   *  <ipaddr_format> is non-0 this produces
   *  "ip-number:port-number/path-name" (e.g.,
   *  "128.252.166.57:80/~schmidt/"), whereas if <ipaddr_format> is 0 this
   *  produces "ip-name:port-number" (e.g.,
   *  "www.cs.wustl.edu:80/~schmidt/"). Uses dynamic memory, which is
   *  allocated on demand and deallocated when the object is destroyed.
   *  Returns -1 if dynamic memory fails, else 0.
   */
  virtual const ACEXML_Char *addr_to_string (int ipaddr_format = 1);

  /// Assignment operator.
  void operator= (const ACEXML_URL_Addr &addr);

  /// Destructor.
  ~ACEXML_URL_Addr (void);

  /**
   *  Compare two addresses for equality.  The addresses are considered
   *  equal if they contain the same IP address, port number, and path name.
   */
  bool operator == (const ACEXML_URL_Addr &SAP) const;

  /// Compare two addresses for inequality.
  bool operator != (const ACEXML_URL_Addr &SAP) const;

  /// Computes and returns hash value.
  virtual u_long hash (void) const;

  /// Return the path name.
  const ACEXML_Char *get_path_name (void) const;

  /// Commit suicide.
  int destroy (void);

private:

  /// Calculate the maximum length of the address string
  size_t calculate_length (int ipaddr_format) const;

  /// Our path name.
  ACEXML_Char *path_name_;

  /// The dynamically created address string that's used for the
  /// <addr_to_string> method.
  ACEXML_Char *addr_string_;

  /// Current length of the <addr_string_>
  size_t addr_string_len_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/URL_Addr.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_URL_ADDR_H */

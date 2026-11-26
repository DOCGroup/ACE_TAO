/** @file   URL_Addr.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
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
#include <memory>

/**
 *  @class ACEXML_URL_Addr
 *
 *  Defines a URL address family address format.
 */
class ACEXML_Export ACEXML_URL_Addr : public ACE_INET_Addr
{
public:
  /// Initialization and termination methods.
  ACEXML_URL_Addr ();

  ACEXML_URL_Addr (ACEXML_Char const *host_name,
                   ACEXML_Char const *path_name,
                   unsigned short port = ACE_DEFAULT_HTTP_PORT);

  ACEXML_URL_Addr (ACEXML_URL_Addr const &addr);

  /// Essentially the copy constructor.
  int set (ACEXML_URL_Addr const &addr);

  /**
   *  Initializes an @c ACEXML_URL_Addr from @a address, which can be
   *  "ip-number:port-number/path-name", e.g.,
   *  "128.252.166.57:1234/~schmidt/", or "host:port-number/path-name",
   *  e.g., "www.cs.wustl.edu:1234/~schmidt/". If there is no ':' in the
   *  @a address it is assumed to be an IP address, with the port number
   *  @c ACE_DEFAULT_HTTP_PORT.
   */
#if defined (ACE_USES_WCHAR)
  virtual int string_to_addr (ACEXML_Char const* address,
                              int address_family = AF_UNSPEC);
#else
  int string_to_addr (ACEXML_Char const* address,
                      int address_family = AF_UNSPEC) override;
#endif

#if defined (ACE_USES_WCHAR)
  int string_to_addr (char const* addresss,
                      int address_family = AF_UNSPEC) override;
#endif

  /**
   *  Transform the current @c ACE_INET_Addr address into string format. If
   *  @a ipaddr_format is nonzero, this produces
   *  "ip-number:port-number/path-name" (e.g.,
   *  "128.252.166.57:80/~schmidt/"), whereas if @a ipaddr_format is 0 this
   *  produces "ip-name:port-number" (e.g.,
   *  "www.cs.wustl.edu:80/~schmidt/"). Returns -1 if @a size is too small
   *  for @a s, else 0.
   */
  int addr_to_string (ACEXML_Char *s,
                      size_t size,
                      int ipaddr_format = 1) const override;

  /**
   *  Transform the current ACE_INET_Addr address into string format. If
   *  @a ipaddr_format is non-0 this produces
   *  "ip-number:port-number/path-name" (e.g.,
   *  "128.252.166.57:80/~schmidt/"), whereas if @a ipaddr_format is 0 this
   *  produces "ip-name:port-number" (e.g.,
   *  "www.cs.wustl.edu:80/~schmidt/"). Uses dynamic memory, which is
   *  allocated on demand and deallocated when the object is destroyed.
   *  Returns -1 if dynamic memory fails, else 0.
   */
  virtual ACEXML_Char const *addr_to_string (int ipaddr_format = 1);

  void operator= (ACEXML_URL_Addr const &addr);

  ~ACEXML_URL_Addr () override;

  /**
   *  Compare two addresses for equality.  The addresses are considered
   *  equal if they contain the same IP address, port number, and path name.
   */
  bool operator == (ACEXML_URL_Addr const &SAP) const;

  /// Compare two addresses for inequality.
  bool operator != (ACEXML_URL_Addr const &SAP) const;

  /// Computes and returns hash value.
  u_long hash () const override;

  /// Return the path name.
  ACEXML_Char const *get_path_name () const;

  /// Commit suicide.
  int destroy ();

private:
  struct Free_Deleter
  {
    void operator() (ACEXML_Char *ptr) const;
  };

  using ACEXML_Char_Ptr = std::unique_ptr<ACEXML_Char, Free_Deleter>;

  /// Calculate the maximum length of the address string
  size_t calculate_length (int ipaddr_format) const;

  /// Our path name.
  ACEXML_Char_Ptr path_name_ {};

  /// The dynamically created address string used by @c addr_to_string().
  ACEXML_Char_Ptr addr_string_ {};

  /// Current length of @c addr_string_.
  size_t addr_string_len_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/URL_Addr.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_URL_ADDR_H */

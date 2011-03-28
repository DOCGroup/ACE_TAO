/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL_Addr.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_URL_ADDR_H
#define ACE_URL_ADDR_H

#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "websvcs_export.h"
#include "ace/ACE.h"

class ACE_URL_Addr_Visitor;

/**
 * @class ACE_URL_Addr
 *
 * @brief Defines the interface for an URL
 *
 * All URLs derive from this class
 */
class ACE_WEBSVCS_Export ACE_URL_Addr : public ACE_Addr
{
public:
  /// Constructor.
  ACE_URL_Addr (void);

  /// The copy constructor.
  ACE_URL_Addr (const ACE_URL_Addr& address);

  /// The assignment operator
  ACE_URL_Addr& operator= (const ACE_URL_Addr& address);

  /// destructor
  virtual ~ACE_URL_Addr (void);

  /// Get the original URL
  const ACE_TCHAR *get_url (void) const;

  /// Essentially the copy contructor.
  int set (const ACE_URL_Addr& address);

  /**
   * Initializes from the scheme specific address, for instance: if
   * the address is an http URL it will initialize the address from
   * an string such as "www.cs.wustl.edu/~schmidt"
   */
  virtual int string_to_addr (const ACE_TCHAR *address);

  /**
   * Write the address in the scheme specific representation.
   * <flags> provides control over scheme specific features (such as
   * using numeric vs. fully qualified host names).
   */
  virtual int addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int flags = 0) const;

  /// The accept method in the Visitor Pattern. Should return 0 on
  /// success and not 0 on failure.
  virtual int accept (ACE_URL_Addr_Visitor* visitor);

  /// Create an address from a complete URL, such as "http://www/foo"
  /// or "ftp://ftp.here/get_this".
  static ACE_URL_Addr* create_address (const ACE_TCHAR *url);

  /// Returns 1 if the URL scheme is recognized, 0 otherwise.
  static int known_scheme (const ACE_TCHAR *url);

  /// Hash function
  u_long hash (void) const;

protected:
  /// Allows the derived classes to store the compact representation of
  /// the URL
  void set_url (ACE_TCHAR *url);

private:
  ACE_TCHAR *url_;
};

/**
 * @class ACE_Mailto_Addr;
 @@ TODO add more URL schemes as needed.
 * class ACE_File_Addr;
 * class ACE_AFS_Addr;
 * class ACE_News_Addr;
 * class ACE_NNTP_Addr;
 * class ACE_CID_Addr;
 * class ACE_MID_Addr;
 * class ACE_WAIS_Addr;
 * class ACE_Prospero_Addr;
 * class ACE_Telnet_Addr;
 * class ACE_Rlogin_Addr;
 * class ACE_TN3270_Addr;
 * class ACE_Gopher_Addr;
 */
class ACE_HTTP_Addr;
class ACE_FTP_Addr;
class ACE_Mailto_Addr;

/**
 * @class ACE_URL_Addr_Visitor
 *
 * @brief Implements a Visitor object for the ACE_URL hierarchy.
 *
 * The manipulation of URL objects is much simpler if we use the
 * Visitor pattern to solve the double dispatch problem between
 * the "what to do on a URL" vs. "what to do on each kind of
 * URL".
 */
class ACE_WEBSVCS_Export ACE_URL_Addr_Visitor
{
public:
  /// Destructor
  virtual ~ACE_URL_Addr_Visitor (void);

  /**
   * The visit methods for all the hierarchy.
   * The default implementation is a nop (instead of a pure virtual
   * function) to facilitate the addition of new members in the
   * hierarchy.
   * virtual int visit (ACE_AFS_Addr*);
   * virtual int visit (ACE_News_Addr*);
   * virtual int visit (ACE_NNTP_Addr*);
   */
  virtual int visit (ACE_URL_Addr*);
  virtual int visit (ACE_HTTP_Addr*);
  virtual int visit (ACE_FTP_Addr*);
  virtual int visit (ACE_Mailto_Addr*);
};

// ****************************************************************

/**
 * @class ACE_HTTP_Addr
 *
 * @brief Defines the HTTP scheme addresses
 *
 * Encapsulates an HTTP URL; the most general form is:
 * http://host:port/path?query
 * but these are also accepted:
 * http://host/path?query
 * http://host:port/path
 * http://host/path
 */
class ACE_WEBSVCS_Export ACE_HTTP_Addr : public ACE_URL_Addr
{
public:
  /// Constructor
  ACE_HTTP_Addr (void);

  /// Construct an HTTP URL from the host, path, query and port.
  ACE_HTTP_Addr (const ACE_TCHAR *host_name,
                 const ACE_TCHAR *path,
                 const ACE_TCHAR *query = 0,
                 u_short port = ACE_DEFAULT_HTTP_PORT);

  /// Essentially the constructor above.
  int set (const ACE_TCHAR *host_name,
           const ACE_TCHAR *path,
           const ACE_TCHAR *query = 0,
           u_short port = ACE_DEFAULT_HTTP_PORT);

  /// Copy constructor.
  ACE_HTTP_Addr (const ACE_HTTP_Addr &addr);

  /// Assignment operator.
  ACE_HTTP_Addr& operator= (const ACE_HTTP_Addr &addr);

  /// Essentially the copy constructor.
  int set (const ACE_HTTP_Addr &addr);

  /// Destructor
  virtual ~ACE_HTTP_Addr (void);

  /**
   * Build the INET_Address implicit in the URL, notice that we
   * maintain the hostname in its string representation, because the
   * URL can be can be refering to an hostname that cannot be
   * validated at this point.
   */
  ACE_INET_Addr get_inet_address (void) const;

  /// Get the name of the host.
  const ACE_TCHAR *get_hostname (void) const;

  /// Get the port number.
  u_short get_port_number (void) const;

  /// Get the path component in the URL
  const ACE_TCHAR *get_path (void) const;

  /// Get the query component in the URL
  const ACE_TCHAR *get_query (void) const;

  /**
   * Create an address from a (possibly) relative URL, such as
   * "../foo.html", or "/icons/bar.gif"
   * If the URL is absolute (like "http://www/foo" or "ftp:host/bar")
   * it simply returns the correct ACE_URL_Addr object; but if the URL
   * is not absolute then it is interpreted as relative from the
   * current address.  In that case url is just a path, if it is a
   * relative path the new address simply concatenates the path and
   * uses the same host:port; if it is an absolute path only the host
   * and port are used.
   */
  ACE_URL_Addr* create_relative_address (const ACE_TCHAR *url) const;

  // = The ACE_URL methods, see the documentation above.
  virtual int string_to_addr (const ACE_TCHAR *address);
  virtual int addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int flags = 0) const;
  virtual int accept (ACE_URL_Addr_Visitor* visitor);

private:
  /// Compute the size required to store the URL in a string
  /// representation.
  size_t url_size (int flags = 0) const;

  /// Helper method to cleanup resources
  void clear (void);

private:
  /// The host:port component in the URL
  ACE_TCHAR *hostname_;
  u_short port_number_;

  /// The path component in the URL
  ACE_TCHAR *path_;

  /// The query component in the URL
  ACE_TCHAR *query_;
};

// ****************************************************************

/**
 * @class ACE_FTP_Addr
 *
 * @brief Defines the FTP scheme addresses
 *
 * Encapsulates an FTP URL; usually an FTP URL is of the form:
 * ftp://hostname/path
 * but the most general form is:
 * ftp://user:password@hostname/path
 * the [:password] part can be omitted too.
 */
class ACE_WEBSVCS_Export ACE_FTP_Addr : public ACE_URL_Addr
{
public:
  /// Constructor
  ACE_FTP_Addr (void);

  /// Construct an FTP URL from the host_name, the path, the username
  /// and the password.
  ACE_FTP_Addr (const ACE_TCHAR *host_name,
                const ACE_TCHAR *path,
                const ACE_TCHAR *user = 0,
                const ACE_TCHAR *password = 0);

  /// Essentially the constructor above.
  int set (const ACE_TCHAR *host_name,
           const ACE_TCHAR *path,
           const ACE_TCHAR *user = 0,
           const ACE_TCHAR *password = 0);

  /// Copy constructor.
  ACE_FTP_Addr (const ACE_FTP_Addr &addr);

  /// Assignment operator
  ACE_FTP_Addr& operator= (const ACE_FTP_Addr &addr);

  /// Essentially the copy constructor.
  int set (const ACE_FTP_Addr &addr);

  /// Destructor
  virtual ~ACE_FTP_Addr (void);

  /// Get the host name component in the URL
  const ACE_TCHAR *get_hostname (void) const;

  /// Get the username component in the URL
  const ACE_TCHAR *get_user (void) const;

  /// Get the password component in the URL
  const ACE_TCHAR *get_passwd (void) const;

  /// Get the path component in the URL
  const ACE_TCHAR *get_path (void) const;

  /// Obtain the INET_Address implicit in the URL, can be used to
  /// obtain the host and the port.
  ACE_INET_Addr get_inet_address (void) const;

  // = The ACE_Addr methods, see the documentation above.
  virtual int string_to_addr (const ACE_TCHAR *address);
  virtual int addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int flags = 0) const;
  virtual int accept (ACE_URL_Addr_Visitor* visitor);

private:
  /// Compute the size required to store the URL in a string
  /// representation.
  size_t url_size (int flags = 0) const;

  /// Helper method to release the internal resources
  void clear (void);

private:
  /// The login name
  ACE_TCHAR *user_;
  ACE_TCHAR *password_;

  /// The hostname part.
  ACE_TCHAR *hostname_;

  /// The other components.
  ACE_TCHAR *path_;
};

// ****************************************************************

/**
 * @class ACE_Mailto_Addr
 *
 * @brief Defines the mailto scheme addresses
 *
 * Encapsulates an URL that refers to an email address.
 */
class ACE_WEBSVCS_Export ACE_Mailto_Addr : public ACE_URL_Addr
{
public:
  /// Constructor
  ACE_Mailto_Addr (void);

  /// Construct an FTP URL from the host, path and headers.
  ACE_Mailto_Addr (const ACE_TCHAR *user,
                   const ACE_TCHAR *hostname,
                   const ACE_TCHAR *headers = 0);

  /// Essentially the constructor above.
  int set (const ACE_TCHAR *user,
           const ACE_TCHAR *hostname,
           const ACE_TCHAR *headers = 0);

  /// Copy constructor.
  ACE_Mailto_Addr (const ACE_Mailto_Addr &addr);

  /// Assignment operator
  ACE_Mailto_Addr& operator= (const ACE_Mailto_Addr &addr);

  /// Essentially the copy constructor.
  int set (const ACE_Mailto_Addr &addr);

  /// Destructor
  virtual ~ACE_Mailto_Addr (void);

  /// Get the username component in the URL
  const ACE_TCHAR *get_user (void) const;

  /// Get the hostname component in the URL
  const ACE_TCHAR *get_hostname (void) const;

  /// Get the headers as a single string
  const ACE_TCHAR *get_headers (void) const;

  // @@ TODO A mailto: URL can contain multiple headers, an iterator
  // over them would be a good idea. Similarly a method to *add*
  // headers would be nice also.

  // = The ACE_URL methods, see the documentation above.
  virtual int string_to_addr (const ACE_TCHAR *address);
  virtual int addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int flags = 0) const;
  virtual int accept (ACE_URL_Addr_Visitor* visitor);

private:
  /// Compute the size required to store the URL in a string
  /// representation.
  size_t url_size (int flags = 0) const;

  /// Helper method to cleanup resources
  void clear (void);

private:
  ACE_TCHAR *user_;
  ACE_TCHAR *hostname_;
  ACE_TCHAR *headers_;
};

#if defined (__ACE_INLINE__)
#include "URL_Addr.inl"
#endif /* __ACE_INLINE__ */

#endif /* ACE_URL_ADDR_H */

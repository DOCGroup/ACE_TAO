// $Id$

// by James Hu
// Borrowed from HTTP_Headers.*, which appears to be irrelevent now anyway.

#if !defined (HTTPU_HTTP_HEADERS_H)
#define HTTPU_HTTP_HEADERS_H

#include "ace/RB_Tree.h"
#include "ace/Singleton.h"

#include "JAWS/Parse_Headers.h"
#include "HTTPU/http_export.h"

class HTTP_Headers;

class HTTPU_Export HTTP_Hdr_Node
{
  // Constructor should be passed literal strings.
  friend class HTTP_HCodes;

public:
  operator const int (void) const;
  operator const char * (void) const;
  const char * format (void) const;

private:
  HTTP_Hdr_Node (const char *token, const char *format);

private:
  int index_;
  const char *token_;
  const char *format_;
};


class HTTP_HCodes;

class HTTPU_Export HTTP_Header_Nodes : public RB_Tree<int, const HTTP_Hdr_Node *>
{
  friend class HTTP_HCodes;
  friend class HTTP_Hdr_Node;

public:
  HTTP_Header_Nodes (void);

private:
  int num_header_strings_;
};

typedef ACE_Singleton<HTTP_Header_Nodes, ACE_SYNCH_MUTEX>
        HTTP_Header_Nodes_Singleton;

class HTTPU_Export HTTP_HCodes
{
public:
  HTTP_HCodes (void);

  enum {
    REPLACE_HEADER = 1,  // Remove any existing header that matches first
    APPEND_HEADER = 2,   // Unconditionally append the header
    INSERT_HEADER = 4,   // Insert header if one does not already exist
    APPENDTO_HEADER = 8  // Concatenate data to existing header value
  };

  static HTTP_Hdr_Node
    HTTP,
    ACCEPT,
    ACCEPTCHARSET,
    ACCEPTENCODING,
    ACCEPTLANGUAGE,
    ACCEPTRANGES,
    AGE,
    ALLOW,
    AUTHORIZATION,
    CACHECONTROL,
    CONNECTION,
    CONTENTENCODING,
    CONTENTLENGTH,
    CONTENTLOCATION,
    CONTENTMD5,
    CONTENTRANGE,
    CONTENTTYPE,
    DATE,
    ETAG,
    EXPECT,
    EXPIRES,
    FROM,
    HOST,
    IFMATCH,
    IFMODIFIEDSINCE,
    IFNONEMATCH,
    IFRANGE,
    IFUNMODIFIEDSINCE,
    LASTMODIFIED,
    LOCATION,
    MAXFORWARDS,
    PRAGMA,
    PROXYAUTHENTICATE,
    PROXYAUTHORIZATION,
    RANGE,
    REFERER,
    RETRYAFTER,
    SERVER,
    TE,
    TRAILER,
    TRANSFERENCODING,
    UPGRADE,
    USERAGENT,
    VARY,
    VIA,
    WARNING,
    WWWAUTHENTICATE,
    GET,
    HEAD,
    POST,
    PUT,
    QUIT,
    DUNNO,
    META,
    A,
    SCRIPT,
    APPLET
  ;

  static const int &NUM_HEADER_STRINGS;

protected:

  const HTTP_Hdr_Node &hcode (int type) const;

protected:

  HTTP_Header_Nodes *header_nodes_;
};

class HTTPU_Export HTTP_Headers : public JAWS_Header_Info, public HTTP_HCodes
{
public:
  const char *header( int name ) const;
  const char *value( int name );
  const char *value_next( int name );
  void value_reset ( void );

public:
  HTTP_Headers (void);

  const char *header_token (int name) const;
  const char *header_strings (int name) const;

};


#if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "HTTPU/http_headers.i"
# endif /* ACE_HAS_INLINED_OSCALLS */

#endif /* !defined (HTTPU_HTTP_HEADERS_HPP) */

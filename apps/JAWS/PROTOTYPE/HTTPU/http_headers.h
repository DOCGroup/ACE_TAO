// $Id$

// by James Hu
// Borrowed from HTTP_Headers.*, which appears to be irrelevent now anyway.

#if !defined (HTTPU_HTTP_HEADERS_H)
#define HTTPU_HTTP_HEADERS_H

#include "ace/RB_Tree.h"
#include "ace/Singleton.h"

#include "JAWS/Parse_Headers.h"

class HTTP_Headers;

class HTTP_Hdr_Node
{
  // Constructor should be passed literal strings.
  friend class HTTP_HCodes;

public:
  operator const int (void) const { return this->index_; }
  operator const char * (void) const { return this->token_; }
  const char * format (void) const { return this->format_; }

private:
  HTTP_Hdr_Node (const char *token, const char *format);

private:
  int index_;
  const char *token_;
  const char *format_;
};


class HTTP_HCodes;

class HTTP_Header_Nodes : public RB_Tree<int, const HTTP_Hdr_Node *>
{
  friend class HTTP_HCodes;
  friend class HTTP_Hdr_Node;

public:
  HTTP_Header_Nodes (void) : num_header_strings_ (0) {}

private:
  int num_header_strings_;
};

typedef ACE_Singleton<HTTP_Header_Nodes, ACE_SYNCH_MUTEX>
        HTTP_Header_Nodes_Singleton;

class HTTP_HCodes
{
public:
  HTTP_HCodes (void)
    : header_nodes_ (HTTP_Header_Nodes_Singleton::instance ())
    {}

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

  const HTTP_Hdr_Node &hcode (int type) const
    {
      const HTTP_Hdr_Node **hn = this->header_nodes_->find (type);

      // No error checking!
      return **hn;
    }

protected:

  HTTP_Header_Nodes *header_nodes_;
};

class HTTP_Headers : public JAWS_Header_Info, public HTTP_HCodes
{
public:
  const char *header( int name ) const;
  const char *value( int name );
  const char *value_next( int name );
  void value_reset ( void );

public:
  HTTP_Headers (void) {}

  const char *header_token (int name) const
    {
      const HTTP_Hdr_Node **hn = this->header_nodes_->find (name);
      return ((hn && *hn) ? (const char *)**hn : 0);
    }

  const char *header_strings (int name) const
    {
      const HTTP_Hdr_Node **hn = this->header_nodes_->find (name);
      return ((hn && *hn) ? (*hn)->format () : 0);
    }

};

#endif /* !defined (HTTPU_HTTP_HEADERS_HPP) */

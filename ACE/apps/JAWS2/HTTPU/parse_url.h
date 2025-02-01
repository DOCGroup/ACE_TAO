/* -*- c++ -*- */
#ifndef HTTPU_HTTP_PARSE_H
#define HTTPU_HTTP_PARSE_H

#include "HTTPU/http_export.h"

class HTTPU_Export HTTP_Parse_URL
{
  // CAVEAT:

  // The user of the class is responsible for testing the difference
  // between a missing username versus an empty one.  Same goes for
  // password The RFC (1738) makes the differentiation for username
  // and password.  If the hostname is missing (or empty), this class
  // always returns a null value for the host.

public:
  HTTP_Parse_URL (const char *url = 0);
  ~HTTP_Parse_URL ();

  void init (const char *url);

  enum URL_SCHEME { HTTP, FTP };

  const char *scheme () const;
  const char *user () const;
  const char *passwd () const;
  const char *host () const;
  int port () const;
  const char *url_path () const;

  enum URL_ERROR { URL_ERROR_NONE, URL_ERROR_STRDUP, URL_ERROR_SCHEME, URL_ERROR_SLASHSLASH };

  int error () const { return( error_ ); }

  int is_cgi () const;

private:
  void parse_url ();
  void parse_scheme (char *&p);
  void parse_host (char *&p);
  void parse_url_path (char *&p);
  void is_cgi (const char *path);

  void set_port_from_scheme ();

private:
  char *url_;

  const char *scheme_;
  const char *user_;
  const char *passwd_;
  const char *host_;
  int port_;
  const char *url_path_;

  int error_;
  int is_cgi_;
};

#endif /* !defined (HTTPU_HTTP_PARSE_H) */

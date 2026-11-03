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
  explicit HTTP_Parse_URL (char const *url = nullptr);
  ~HTTP_Parse_URL ();
  HTTP_Parse_URL (HTTP_Parse_URL const &) = delete;
  HTTP_Parse_URL (HTTP_Parse_URL &&) = delete;
  HTTP_Parse_URL &operator= (HTTP_Parse_URL const &) = delete;
  HTTP_Parse_URL &operator= (HTTP_Parse_URL &&) = delete;

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

  bool is_cgi () const;

private:
  void parse_url ();
  void parse_scheme (char *&p);
  void parse_host (char *&p);
  void parse_url_path (char *&p);
  void is_cgi (const char *path);

  void set_port_from_scheme ();

private:
  char *url_; // Owns the strdup buffer; the remaining string members point into it.

  const char *scheme_;
  const char *user_;
  const char *passwd_;
  const char *host_;
  int port_;
  const char *url_path_;

  int error_;
  bool is_cgi_;
};

#endif /* !defined (HTTPU_HTTP_PARSE_H) */

/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
//
// = FILENAME
//    HTTP_Helpers.h
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#ifndef HTTP_HELPERS_H
#define HTTP_HELPERS_H

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class HTTP_Helper
// Static functions to enhance the lives of HTTP programmers everywhere.
{
public:

  // Convert and HTTP-date into a time_t
  static time_t HTTP_mktime (const char *httpdate);

  // Create today's date
  static const char *HTTP_date (void);
  static const char *HTTP_date (char *s);

  // Month conversions (ascii <--> numeric)
  static int HTTP_month (const char *month);
  static const char *HTTP_month (int month);

  static char *HTTP_decode_string (char *path);

  // Encode/Decode base64 stuff (weak security model)
  static char *HTTP_decode_base64 (char *data);
  static char *HTTP_encode_base64 (char *data);

private:

  static int fixyear (int year);

private:
  static const char *const months_[12];
  static char const *alphabet_;

  static char *date_string_;
  static ACE_SYNCH_MUTEX mutex_;
  // Use this sometimes (e.g. HTTP_date)
};

// Design around the Singleton pattern

class HTTP_Status_Code
  // = TITLE
  //     Go from numeric status codes to descriptive strings.
  //
  // = DESCRIPTION
  //     Design around the Singleton pattern
{
public:
  static const char **instance (void);
  // Singleton access point.

  enum STATUS_CODE
  {
    STATUS_OK = 200,
    STATUS_CREATED = 201,
    STATUS_ACCEPTED = 202,
    STATUS_NO_CONTENT = 204,
    STATUS_MOVED_PERMANENTLY = 301,
    STATUS_MOVED_TEMPORARILY = 302,
    STATUS_NOT_MODIFIED = 304,
    STATUS_BAD_REQUEST = 400,
    STATUS_UNAUTHORIZED = 401,
    STATUS_FORBIDDEN = 403,
    STATUS_NOT_FOUND = 404,
    STATUS_INTERNAL_SERVER_ERROR = 500,
    STATUS_NOT_IMPLEMENTED = 501,
    STATUS_BAD_GATEWAY = 502,
    STATUS_SERVICE_UNAVAILABLE = 503,
    STATUS_INSUFFICIENT_DATA = 399
  };

  enum
  {
    MAX_STATUS_CODE = 599
  };

private:
  // Singleton pattern is afoot here.
  static const char *Reason[MAX_STATUS_CODE + 1];
  static int instance_;
  static ACE_SYNCH_MUTEX lock_;
};

#endif /* HTTP_HELPERS_H */


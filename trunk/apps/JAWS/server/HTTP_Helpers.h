// HTTP_Helpers.h

#if !defined (HTTP_HELPERS_H)
#define HTTP_HELPERS_H

#include "ace/Synch.h"

class HTTP_Status_Code
  // = TITLE
  //    Simplify interface to HTTP_types:
  //     -> type(path) returns the type of the path matched by extension.
  //     -> app(path) returns the app for the path matched by extension.
  //
  // = DESCRIPTION
  //     Design around the Singleton pattern
{
public:
  static const char **instance(void);
  // Singleton access point.

  enum STATUS_CODE {
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

  enum {MAX_STATUS_CODE=599};

private:
  static const char * Reason[MAX_STATUS_CODE + 1];
  static int instance_;
  static ACE_Thread_Mutex lock_;
};

#endif /* HTTP_HELPERS_H */


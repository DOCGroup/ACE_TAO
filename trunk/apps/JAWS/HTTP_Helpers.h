// HTTP_Helpers.h

// Design around the Singleton pattern
// Simplify interface to HTTP_types:
//  -> type(path) returns the type of the path matched by extension.
//  -> app(path) returns the app for the path matched by extension.

#ifndef HTTP_HELPERS_H_
#define HTTP_HELPERS_H_

#include "ace/Log_Msg.h"
#include "ace/Synch_T.h"

class HTTP_Status_Code
{
public:
  static char const * const * const instance(void);

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
    STATUS_SERVICE_UNAVAILABLE = 503
  };

  enum {MAX_STATUS_CODE=599};

private:
  static char * Reason[MAX_STATUS_CODE+1];
  static int instance_;
  static ACE_Thread_Mutex lock_;

};

#endif

/// Local Variables:
/// mode: c++
/// End:

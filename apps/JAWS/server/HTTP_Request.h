#if !defined (HTTP_REQUEST_H)
#define HTTP_REQUEST_H

#include "ace/OS.h"

class HTTP_Request 
{
public:
  HTTP_Request (void);
  // Default construction.

  void init (const char *buffer, int buflen);
  // Initialize the request object with the buffer.  This will parse
  // the buffer and prepare for the accessors.

  u_long type (void);
  // GET or PUT.

  char *data (void);

  int data_length (void);

  int content_length (void);

  char *filename (void);

  int status (void);

  char *status_string (void);

  void dump (void);
  // Dump the state of the request.

  enum 
  {
    OK = 1, 
    NO_FILENAME, 
    NO_CONTENT_LENGTH, 
    NO_HEADER,
    // **************
    NO_TYPE,
    GET,
    PUT
  };

protected:
  void parse_GET (char *lasts);
  void parse_PUT (char *lasts);
  
private:
  char *data_;
  int datalen_;
  int content_length_;
  char *filename_;
  int status_;
  u_long type_;
};

#endif /* HTTP_REQUEST_H */

#include "JAWS/server/HTTP_Request.h"

// for reasons of efficiency, this class expects buffer to be null-terminated, 
// and buflen does NOT include the \0
HTTP_Request::HTTP_Request (void)
{
}

void
HTTP_Request::init (const char *buffer, int buflen)
{
  // Initialize these every time.
  content_length_ = -1;
  filename_ = "";
  status_ = OK;
  type_ = NO_TYPE;

  // Extract the data pointer.
  data_ = NULL;
  datalen_ = 0;

  if ((data_ = ACE_OS::strstr (buffer,"\r\n\r\n")) != NULL) 
    data_ += 4;
  else if ((data_ = ACE_OS::strstr (buffer, "\n\n")) != NULL)
    data_ += 2;
  else
    // Keep going even without a header?
    status_ = NO_HEADER;

  // set the datalen
  if (data_ != NULL)
    datalen_ = (buffer + buflen) - data_;
  else
    datalen_ = 0;

  char *lasts; // for strtok_r

  // Get the request type.
  char *token = ACE_OS::strtok_r ((char *) buffer, " \t", &lasts);

  // Delegate according to the request type.
  if (ACE_OS::strcmp (token, "PUT") == 0)
    this->parse_PUT (lasts);
  else if (ACE_OS::strcmp (token, "GET") == 0)
    this->parse_GET (lasts);
  else
    {
      status_ = NO_FILENAME;
      return;
    }
}

static void
HTTP_fix_path (char *path)
{
  // fix the path if it needs fixing/is fixable

  // replace the percentcodes with the actual character
  int i,j;
  char percentcode[3];
  
  for (i = j = 0; path[i] != '\0'; i++,j++) {
    if (path[i] == '%') {
      percentcode[0] = path[++i];
      percentcode[1] = path[++i];
      percentcode[2] = '\0';
      path[j] = ACE_OS::strtol(percentcode, (char **)0, 16);
    }
    else path[j] = path[i];
  }
  path[j] = path[i];
}

void
HTTP_Request::parse_GET (char *lasts)
{
  type_ = GET;

  // next look for filename
  filename_ = ACE_OS::strtok_r (NULL, " \t\r\n", &lasts);
  if (filename_ == NULL) 
    {
      status_ = NO_FILENAME;
      return;
    }

  HTTP_fix_path (filename_);

  // Ok, I'm punting here, screw the rest of the request.  I've got
  // what I want.
}

void
HTTP_Request::parse_PUT (char *lasts)
{
  type_ = PUT;

  // next look for filename
  filename_ = ACE_OS::strtok_r (NULL, " \t\r\n", &lasts);
  if (filename_ == NULL) 
    {
      status_ = NO_FILENAME;
      return;
    }

  HTTP_fix_path (filename_);

  // now loop, looking for Content-length:

  char *token = ACE_OS::strtok_r (NULL, "\n\r:", &lasts);
  while (token) 
    {
      if (ACE_OS::strcmp (token, "Content-length") == 0) 
	{
	  token = ACE_OS::strtok_r (NULL, " \t\r\n", &lasts);
	  content_length_ = ACE_OS::atoi (token);
	  break;
	}
      token = ACE_OS::strtok_r (NULL, "\n\r:", &lasts);
    }

  if (content_length_ == -1) 
    {
      status_ = NO_CONTENT_LENGTH;
      return;
    }
}

u_long
HTTP_Request::type (void)
{
  return type_;
}

char *
HTTP_Request::data (void)
{
  return data_;
}

int
HTTP_Request::data_length (void)
{
  return datalen_;
}

char *
HTTP_Request::filename (void)
{
  return filename_;
}

int
HTTP_Request::content_length (void)
{
  return content_length_;
}

int
HTTP_Request::status (void)
{
  return status_;
}

char *
HTTP_Request::status_string (void)
{
  switch (status_) 
    {
    case OK:
      return "All is well";
    case NO_CONTENT_LENGTH:
      return "Content-length: field missing in the header";
    case NO_FILENAME:
      return "Filename missing in the header";
    case NO_HEADER:
    default:
      return "No header";
    }
}

void
HTTP_Request::dump (void)
{
  switch (this->type ())
    {
    case GET :
      ACE_DEBUG ((LM_DEBUG, "GET command.\n"
		  "filename is %s,"
		  " length of the file is %d,"
		  " data string is %s,"
		  " datalen is %d,"
		  " status is %d, which is %s\n\n", 
		  this->filename() ? this->filename() : "EMPTY",
		  this->content_length(),
		  this->data() ? this->data() : "EMPTY",
		  this->data_length(),
		  this->status(),
		  this->status_string()));
      break;

    case PUT :
      ACE_DEBUG ((LM_DEBUG, "PUT command.\n"
		  "filename is %s,"
		  " length of the file is %d,"
		  " data string is %s,"
		  " datalen is %d,"
		  " status is %d, which is %s\n\n", 
		  this->filename() ? this->filename() : "EMPTY",
		  this->content_length(),
		  this->data() ? this->data() : "EMPTY",
		  this->data_length(),
		  this->status(),
		  this->status_string()));
      break;

    case NO_TYPE :
    default:
      break;
    }
}


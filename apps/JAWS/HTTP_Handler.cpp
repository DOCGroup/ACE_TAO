// HTTP_Service.cpp -- simple implementation of the HTTP protocol

#include "HTTP_Handler.h"

HTTP_Handler::HTTP_Handler(void)
{
  sockbufp_ = sockbuf_;
  sockbufn_ = 0;
}

int
HTTP_Handler::open (void *)
{
  this->svc();
  this->close();
  return 0;
}

int
HTTP_Handler::svc (void)
{
  int result;
  int serve_result = 1;
  const int OK = HTTP_Status_Code::STATUS_OK;
  HTTP_VFS_Node *handle;

  if ((result = this->parse_request()) == OK
      && (result = HTTP_VFS::open(this->requestURI_, handle)) == OK) {
    serve_result = this->serve_file(handle);
    this->peer().close();
    HTTP_VFS::close(handle);
  }
  else {
    serve_result = this->serve_error(result);
    this->peer().close();
  }

  return serve_result;
}

int
HTTP_Handler::parse_request (void)
{  
  // read in the HTTP request
  // --
  // What is really needed here is a real tokenizer and parser.
  // (1) It would allow someone to more easily plug in protocol revisions.
  // (2) Code would be easier to understand and maintain.

  int found = 0;
  int first = 1;

  method_[0] = '\0';
  requestURI_[0] = '\0';
  HTTPversion_[0] = '\0';


  //  ACE_HANDLE handle = this->peer().get_handle();
  //  ACE_Handle_Set handle_set;
  //  handle_set.set_bit(handle);

  //  if (ACE_OS::select(handle+1, handle_set, 0, 0, (ACE_Time_Value *)0) == -1)
  //    return HTTP_Status_Code::STATUS_BAD_REQUEST;

  while (this->sockgets(buf_, sizeof(buf_)) == 0)
    ;

  do {

    // Blank line means we are done
    if (ACE_OS::strcmp(buf_, "\r\n") == 0
        || ACE_OS::strcmp(buf_, "\n") == 0) {
      if (first) ACE_DEBUG((LM_DEBUG, "Whups\n"));
      break;
    }

    // if (!found && requestRE_.regex(buf_, path_)) found = 1;
    // ^^
    // Old way of doing it.  Maybe below will be a little faster.

    if (first) {
      char *lasts;
      char *tok;
      if ((tok = ACE_OS::strtok_r(buf_, " \r\n\t", &lasts)) != 0) {
        ACE_OS::strcpy(method_, tok);
        if ((tok = ACE_OS::strtok_r(0, " \r\n\t", &lasts)) != 0) {
          ACE_OS::strcpy(requestURI_, tok);
          if ((tok = ACE_OS::strtok_r(0, " \r\n\t", &lasts)) != 0) {
            ACE_OS::strcpy(HTTPversion_, tok+5);
            found = 1;
          }
          else {
            if (ACE_OS::strcmp(method_, "GET") == 0) {
              ACE_OS::strcpy(HTTPversion_, "0.9");
              found = 2;
            }
          }
        }
      }

      // Request must be the first line of HTTP request
      first = 0;
    }

  } while (this->sockgets(buf_, sizeof(buf_)) != 0);


  if (found == 0) {
    return HTTP_Status_Code::STATUS_BAD_REQUEST;
  }
  if (ACE_OS::strcmp(method_, "GET") != 0)
    return HTTP_Status_Code::STATUS_NOT_IMPLEMENTED;

  fix_path(requestURI_);

  return HTTP_Status_Code::STATUS_OK;
}

int
HTTP_Handler::serve_error (int status_code)
{
  static char const errormessage[] =
    "HTTP/1.0 %d %s\r\n"
    "Content-type: text/html\r\n"
    "\r\n"
    "<html>\n"
    "<head><title>Error message</title></head>\n"
    "<body>\n"
    "<h1>Error %d: %s</h1>"
    "Could not access file: %s.\n"
    "</body>\n"
    "</html>\n"
    ;

  int len = sprintf(this->buf_, errormessage,
                    status_code, HTTP_Status_Code::instance()[status_code],
                    status_code, HTTP_Status_Code::instance()[status_code],
                    requestURI_);

  this->peer().send_n(this->buf_, len);
  ACE_DEBUG((LM_DEBUG, "in HTTP_Handler::serve_error(%d), %s\n",
             status_code, this->path_));
  return 0;
}

int
HTTP_Handler::serve_directory (void)
{
  // We'll just forbid it for now.
  return this->serve_error(403);
}

int
HTTP_Handler::serve_file (HTTP_VFS_Node * &vf)
{
  // Just a simple response for this simple server.
  // --
  // In a real server, detect what kind of request we got,
  // and respond in kind.
  //   simple-request -> simple-response
  //   full-request -> full-response

  this->peer().send_n(vf->File(), vf->Size());
  this->peer().send_n("\r\n", 2);

  return 0;
}

void
HTTP_Handler::fix_path (char *path)
{
  // fix the path if it needs fixing/is fixable

  // replace the percentcodes with the actual character
  int i,j;
  char percentcode[3];
  
  for (i = j = 0; path_[i] != '\0'; i++,j++) {
    if (path_[i] == '%') {
      percentcode[0] = path[++i];
      percentcode[1] = path[++i];
      percentcode[2] = '\0';
      path[j] = strtol(percentcode, (char **)0, 16);
    }
    else path[j] = path[i];
  }
  path[j] = path[i];
}

// ---------------------------------------- //

int
HTTP_Handler::sockgetc(void)
{
  if (sockbufn_ == 0) {
    // buffer is empty
    sockbufn_ = this->peer().recv(sockbuf_, sizeof(sockbuf_));
    sockbufp_ = sockbuf_;
  }
  return (--sockbufn_ >= 0) ? (unsigned char) *sockbufp_++ : -1;
}

int
HTTP_Handler::sockputc(char const c)
{
  //  int n = this->peer().tv_send_n(&c, 1, ACE_Time_Value(5));
  int n = this->peer().send_n(&c, 1);
  return (n == 1) ? c : EOF;
}

int
HTTP_Handler::sockputs(char const *s)
{
  int len = ACE_OS::strlen(s);
  //  int n = this->peer().tv_send_n(s, len, ACE_Time_Value(5));
  int n = this->peer().send_n(s, len);
  return (n >= 0) ? n : EOF;
}

char *
HTTP_Handler::sockgets(char *buf, int n)
{
  int c;
  char *cbuf;

  cbuf = buf;
  while (--n > 0 && (c = this->sockgetc()) != -1) {
    if ((*cbuf++ = c) == '\n')
      break;
  }
  *cbuf = '\0';
  return (c == EOF && cbuf == buf) ? NULL : buf;
}

// ---------------------------------------- //


// This is necessary for gcc to work with templates
#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Acceptor<HTTP_Handler, ACE_SOCK_ACCEPTOR>;
template class Sock_Acceptor_Adapter<HTTP_Handler>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

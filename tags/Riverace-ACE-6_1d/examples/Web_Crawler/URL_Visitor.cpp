// $Id$

#include "ace/OS_NS_string.h"
#include "ace/Truncate.h"
#include "URL_Visitor.h"
#include "Command_Processor.h"



URL_Processing_Strategy::URL_Processing_Strategy (URL &url,
                                                  URL_Iterator &iterator)
  : url_ (url),
    iterator_ (iterator)
{
}

URL_Processing_Strategy::~URL_Processing_Strategy (void)
{
}

int
URL_Processing_Strategy::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}

URL_Download_Strategy::URL_Download_Strategy (URL &url,
                                              URL_Iterator &iterator)
  : URL_Processing_Strategy (url, iterator)
{
}

int
URL_Download_Strategy::execute (void)
{
  ACE_CString buffer;

  // Extract all the contents of the Stream and print them to the
  // file.
  while (this->iterator_.next (buffer) != 0)
    ACE_DEBUG ((LM_DEBUG,
                "%s",
                buffer.c_str ()));

  return 0;
}

HTTP_Header_Processing_Strategy::HTTP_Header_Processing_Strategy (URL &url,
                                                                  URL_Iterator &iterator)
  : URL_Processing_Strategy (url, iterator)
{
}

int
HTTP_Header_Processing_Strategy::execute (void)
{
  // Set the get() position.Necessary since later a peek is done.
  if (this->url_.stream ().get_char () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n","Header Not Found"),
                      -1);
  char line_buf[BUFSIZ + 1];
  ACE_CString line (line_buf);
  // Get the lines in the header iteratively and check for status info.
  int result = 1, i = 0;
  for (i = 0, result = this->iterator_.next (line);
       result > 0;
       ++i, result = this->iterator_.next (line))
    {
      if (i == 0)
        {
          // Assuming that the status-no is a space away.
          int status_index =
            ACE_Utils::truncate_cast<int> (line.find ("HTTP", 0));
          ACE_CString status = line.substring (status_index + 9, //HTTP/1.1 200
                                               3);

           URL_Status *url_status = 0;
          ACE_NEW_RETURN (url_status,
                          URL_Status,
                          0);
          Auto_Destroyer<URL_Status> url_status_ptr (url_status);
          url_status_ptr->status (ACE_OS::atoi (status.c_str ()));
          this->url_.reply_status (**url_status_ptr);
          // Invalid url.
          if (url_status_ptr->status () != 200)
            return -1;
        }
      else
        {

          if (line.find ("text/html") != ACE_CString::npos)
            {
              ACE_CString url_content_type("text/html");
              this->url_.content_type (url_content_type);
            }
        }
    }
  return 0;

}

HTML_Body_Validation_Strategy::HTML_Body_Validation_Strategy (URL &url,
                                                              URL_Iterator &iterator,
                                                              URL_Validation_Visitor &context)
  : URL_Processing_Strategy (url, iterator),
    visitor_context_ (context)
{
}

int
HTML_Body_Validation_Strategy::execute (void)
{
  char host_name_buf[BUFSIZ + 1];
  ACE_CString host_name (host_name_buf);
  host_name.set (url_.url_addr ().get_host_name (),1);

  // All to facilitate relative paths
  char temp[BUFSIZ + 1];
  ACE_CString prev_location (temp);

  prev_location.set (ACE_TEXT_ALWAYS_CHAR (this->url_.url_addr ().get_path_name ()),
                     ACE_OS::strlen (this->url_.url_addr ().get_path_name ()),
                     1);
  int index =
    ACE_Utils::truncate_cast<int> (
      prev_location.rfind ('/', prev_location.length ()));
  ACE_CString str = prev_location.substring (0, index + 1);
  prev_location.set (str.c_str (), 1);

  // Note: prev_location always ends with '/'
  if (prev_location[0] != '/')
    prev_location = "/" + prev_location;

  // Build the url portion which can be attached to teh relative paths.
  prev_location = host_name + prev_location;

  char url_string[BUFSIZ + 1];
  ACE_CString url (url_string);

  while (this->iterator_.next (url) > 0)
    {
      // Check for relative urls.Strip out "http://" if its there.
      if (url.find ("http") == url.npos)
        {
          if (url[0] == '.' && url[1] == '.')
           {
             url.set (&url[3], 1);
             int i =
               ACE_Utils::truncate_cast<int> (
                 prev_location.rfind ('/', prev_location.length () - 1));
             prev_location = prev_location.substring (0, i+1);
           }
          if (url[0] == '.' && url[1] == '/')
            url.set (&url[2], 1);

          url = prev_location + url;
        }
      else
        url.set (&url[7], 1);
      // Double slash at the end works!e.g www.cs.wustl.edu/~kirthika//
      if (url.find (".html") == url.npos)
        url = url + "/";

      // Create the new URL address.
      ACE_URL_Addr *url_addr;
      ACE_NEW_RETURN (url_addr,
                      ACE_URL_Addr,
                      0);
      Auto_Destroyer<ACE_URL_Addr> url_addr_ptr (url_addr);
      if (url_addr_ptr->string_to_addr (ACE_TEXT_CHAR_TO_TCHAR (url.c_str ())) == 0)
        {
          HTTP_URL *http_url;
          ACE_NEW_RETURN (http_url,
                          HTTP_URL (**url_addr_ptr,
                                    dynamic_cast<HTTP_URL *> (&this->url_)),
                          0);
          URL_Command *url_command;
          ACE_NEW_RETURN (url_command,
                          URL_Command (http_url),
                          0);

          OPTIONS::instance ()->command_processor ()->insert (url_command);
        }
    }
  return 0;
}

URL_Iterator *
URL_Validation_Visitation_Strategy_Factory::make_header_iterator (void)
{
  URL_Iterator *i;
  ACE_NEW_RETURN (i,
                  HTTP_Header_Iterator (*this->url_),
                  0);
  return i;
}

URL_Iterator *
URL_Validation_Visitation_Strategy_Factory::make_body_iterator (void)
{
  URL_Iterator *i;
  ACE_NEW_RETURN (i,
                  HTML_Body_Iterator (*this->url_),
                  0);
  return i;
}

URL_Processing_Strategy *
URL_Validation_Visitation_Strategy_Factory::make_header_strategy (URL_Iterator &iterator)
{
  URL_Processing_Strategy *ps;
  ACE_NEW_RETURN (ps,
                  HTTP_Header_Processing_Strategy (*this->url_,
                                                   iterator),
                  0);
  return ps;
}

URL_Processing_Strategy *
URL_Validation_Visitation_Strategy_Factory::make_body_strategy (URL_Iterator &iterator)
{
  URL_Processing_Strategy *ps;
  ACE_NEW_RETURN (ps,
                  HTML_Body_Validation_Strategy (*this->url_,
                                                 iterator,
                                                 this->visitor_context_),
                  0);
  return ps;
}

int
URL_Validation_Visitation_Strategy_Factory::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}

URL_Visitor::~URL_Visitor (void)
{
}

URL_Validation_Visitor::URL_Validation_Visitor (void)
{
  ACE_NEW (this->caching_connect_strategy_,
           CACHED_CONNECT_STRATEGY (this->caching_strategy_));
  ACE_NEW (this->strat_connector_,
           STRATEGY_CONNECTOR(0,
                           &creation_strategy_,
                           caching_connect_strategy_,
                           &activation_strategy_));
  if (strat_connector_ == 0)
    ACE_ERROR ((LM_ERROR,
                "%p %s\n"
                "strategy connector creation failed"));


}

URL_Validation_Visitor::~URL_Validation_Visitor (void)
{
  this->strat_connector_ = 0;
  if (this->caching_connect_strategy_ != 0)
    delete this->caching_connect_strategy_;
}

URL_Validation_Visitor::URL_CACHE &
URL_Validation_Visitor::url_cache (void)
{
  return this->url_cache_;
}

int
URL_Validation_Visitor::in_cache (const ACE_URL_Addr &url_addr)
{
  URL_Status reply_status (URL_Status::STATUS_CODE (1));

  if (this->url_cache_.find (url_addr, reply_status) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "status %d for URL %s (cached)\n",
                  reply_status.status (),
                  url_addr.addr_to_string (0)));

      // Invalid status.
      if (reply_status.status () != 200)
        return -1;

      return 1;
    }
  else
    return 0;
}

URL_Visitation_Strategy_Factory *
URL_Validation_Visitor::make_visitation_strategy_factory (URL &url)
{
  // Since this is HTTP 1.1 we'll need to establish a connection
  // only once. Trying for relative paths.

  if (url.stream ().open (this->strat_connector_,
                          url.url_addr ()) == -1)
    return 0;

  // See if we can get connected and send the GET request via the
  // <HTTP_URL>.
  int result = url.send_request ();
  if (result == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "send_request"));
      if (this->url_cache_.bind (url.url_addr (),
                                 URL_Status (URL_Status::STATUS_SERVICE_UNAVAILABLE)) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "bind"));
      return 0;
    }
  // @@ Here's where we could check to see if the <url> was HTTP or
  // FTP, etc.  But for now we'll just assume that everything is an
  // HTTP URL.
  else
    {

      URL_Visitation_Strategy_Factory *vs;
      ACE_NEW_RETURN (vs,
                      URL_Validation_Visitation_Strategy_Factory (&url,
                                                                  *this),
                      0);
      return vs;
    }
}

int
URL_Validation_Visitor::destroy (void)
{
  delete this->strat_connector_;
  // Commit suicide.
  delete this;
  return 0;
}

int
URL_Validation_Visitor::visit (HTTP_URL &http_url)
{
  int result = this->in_cache (http_url.url_addr ());
  if (result == 0)
    {
      Auto_Destroyer <URL_Visitation_Strategy_Factory> vs (this->make_visitation_strategy_factory (http_url));

      if (*vs == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "make_visitation_strategy_factory"),
                          -1);

      Auto_Destroyer <URL_Iterator> ihs (vs->make_header_iterator ());
      if (*ihs == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "make_header_iterator"),
                          -1);
      Auto_Destroyer <URL_Processing_Strategy> phs (vs->make_header_strategy (**ihs));
      if (*phs == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "make_header_strategy"),
                          -1);
      int phs_result = phs->execute ();
      if (phs_result == -1)
        ACE_DEBUG ((LM_DEBUG,
                    "Invalid "));

      ACE_DEBUG ((LM_DEBUG,
                  "URL with status %d %s\n",
                  http_url.reply_status ().status (),
                  http_url.url_addr().addr_to_string (0)));

      // Store the http url in the cache.
      if (this->url_cache ().bind (http_url.url_addr (),
                                   http_url.reply_status ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n","url_cache.bind"),
                          -1);

      // Since it is invalid dont go further.
      if (phs_result == -1)
        return 0;

      // Get back if the recurse option isnt set.
      if (OPTIONS::instance ()->recurse () != 1)
        return 0;

      Auto_Destroyer <URL_Iterator> is (vs->make_body_iterator ());
      if (*is == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "make_body_iterator"),
                          -1);

      Auto_Destroyer <URL_Processing_Strategy> ps (vs->make_body_strategy (**is));
      if (*ps == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "make_body_strategy"),
                          -1);

      if (ps->execute () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "body execute"),
                          -1);

    }
  return 0;
}

int
URL_Download_Visitation_Strategy_Factory::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}

URL_Iterator *
URL_Download_Visitation_Strategy_Factory::make_header_iterator (void)
{
  return 0;
}

URL_Iterator *
URL_Download_Visitation_Strategy_Factory::make_body_iterator (void)
{
  URL_Iterator *i;
  ACE_NEW_RETURN (i,
                  URL_Download_Iterator (*this->url_),
                  0);
  return i;
}

URL_Processing_Strategy *
URL_Download_Visitation_Strategy_Factory::make_header_strategy (URL_Iterator &iterator)
{
  // You fill in here.
  ACE_UNUSED_ARG (iterator);

  return 0;
}

URL_Processing_Strategy *
URL_Download_Visitation_Strategy_Factory::make_body_strategy (URL_Iterator &iterator)
{
  URL_Processing_Strategy *ps;
  ACE_NEW_RETURN (ps,
                  URL_Download_Strategy (*this->url_,
                                         iterator),
                  0);
  return ps;
}

URL_Visitation_Strategy_Factory::URL_Visitation_Strategy_Factory (URL *url)
  : url_ (url)
{
}

URL_Visitation_Strategy_Factory::~URL_Visitation_Strategy_Factory (void)
{
}

URL_Download_Visitation_Strategy_Factory::URL_Download_Visitation_Strategy_Factory (URL *url)
  : URL_Visitation_Strategy_Factory (url)
{
}

URL_Validation_Visitation_Strategy_Factory::URL_Validation_Visitation_Strategy_Factory (URL *url,
                                                                                        URL_Validation_Visitor &visitor_context)
  : URL_Visitation_Strategy_Factory (url),
    visitor_context_ (visitor_context)
{
}

URL_Visitation_Strategy_Factory *
URL_Download_Visitor::make_visitation_strategy_factory (URL &url)
{
  // See if we can get connected and send the GET request via the
  // <HTTP_URL>.
  while (1)
    {
      int retval = url.send_request ();
      if (retval != -1)
        break;

    }
  // @@ Here's where we could check to see if the <url> was HTTP or
  // FTP, etc.  But for now we'll just assume that everything is an
  // HTTP URL.
  URL_Visitation_Strategy_Factory *vs;
  ACE_NEW_RETURN (vs,
                  URL_Download_Visitation_Strategy_Factory (&url),
                  0);
  return vs;

}

int
URL_Download_Visitor::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}

int
URL_Download_Visitor::visit (HTTP_URL &http_url)
{
  Auto_Destroyer <URL_Visitation_Strategy_Factory> vs (this->make_visitation_strategy_factory (http_url));

  if (*vs == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "make_visitation_strategy_factory"),
                      -1);

  Auto_Destroyer <URL_Iterator> is (vs->make_body_iterator ());
  if (*is == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "make_body_iterator"),
                      -1);

  Auto_Destroyer <URL_Processing_Strategy> ps (vs->make_body_strategy (**is));
  if (*ps == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "make_body_strategy"),
                      -1);

  if (ps->execute () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "body execute"),
                      -1);
  return 0;
}

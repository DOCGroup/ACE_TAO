// $Id$

#include "Options.h"
#include "URL_Visitor_Factory.h"
#include "Web_Crawler.h"



Web_Crawler::~Web_Crawler (void)
{
  delete this->url_visitor_factory_;
}

Web_Crawler::Web_Crawler (void)
  : url_visitor_factory_ (0)
{
}

int
Web_Crawler::open (int argc, ACE_TCHAR *argv[])
{
  if (OPTIONS::instance ()->parse_args (argc, argv) == -1)
    return -1;
  // @@ Put the ACE_Service_Config::open() stuff here somewhere...
  else
    {
      // For now just hardcode this to create "validation" visitors.
      ACE_NEW_RETURN (this->url_visitor_factory_,
                      URL_Validation_Visitor_Factory,
                      -1);
      return 0;
    }
}

int
Web_Crawler::run (void)
{
  // Make the appropriate <URL_Visitor>.
  Auto_Destroyer<URL_Visitor> visitor (this->url_visitor_factory_->make_visitor ());

  if (*visitor == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "make_visitor"),
                      -1);

  // Make the appropriate <Command_Processor>.
  Auto_Destroyer<Command_Processor> cp (this->url_visitor_factory_->make_command_processor ());

  if (*cp == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "make_command_processor"),
                      -1);

  // Set the <Command_Processor> in the <Options> to make it visible.
  OPTIONS::instance ()->command_processor (*cp);

  // Set the <URL_Visitor> in the <Options> to make it visible.
  OPTIONS::instance ()->visitor (*visitor);

  // @@ You fill in here...
  ACE_URL_Addr *url_addr;
  ACE_NEW_RETURN (url_addr,
                  ACE_URL_Addr (OPTIONS::instance()->hostname (),
                                OPTIONS::instance()->path_name (),
                                OPTIONS::instance()->port_no ()), //KIRTHIKA
                  0);
  Auto_Destroyer<ACE_URL_Addr> url_addr_ptr (url_addr);

  HTTP_URL *http_url;
  ACE_NEW_RETURN (http_url,
                  HTTP_URL (**url_addr_ptr),
                  0);

  Auto_Destroyer<HTTP_URL> http_url_ptr (http_url);

  URL_Command *url_command;
  ACE_NEW_RETURN (url_command,
                  URL_Command (*http_url_ptr),
                  0);
  // Auto_Destroyer<URL_Command> url_command_ptr (url_command);

  if (cp->insert (url_command) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", "insert"),
                      -1);

 if (cp->execute () != 0)
   ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", "execute"),
                      -1);
  return 0;
}


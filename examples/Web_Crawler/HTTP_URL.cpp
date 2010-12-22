// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Auto_Ptr.h"
#include "URL_Visitor.h"
#include "Options.h"
#include "HTTP_URL.h"



const ACE_URL_Addr &
HTTP_URL::url_addr (void) const
{
  return this->url_addr_;
}

HTTP_URL::HTTP_URL (const ACE_URL_Addr &url_addr,
                    HTTP_URL *cp)
  : url_addr_ (url_addr),
    containing_page_ (cp == 0 ? this : cp)
{
  ACE_DEBUG ((LM_DEBUG, "HTTP_URL %s\n", url_addr.addr_to_string ()));
}

ssize_t
HTTP_URL::send_request (void)
{
  size_t commandsize =
    ACE_OS::strlen (this->url_addr ().get_path_name ())
    + ACE_OS::strlen (this->url_addr ().get_host_name ())
    + 20 // Extra
    + 1 // NUL byte
    + 16; // Protocol filler...

  char *command;
  ACE_NEW_RETURN (command,
                  char[commandsize],
                  -1);

  // Ensure that the <command> memory is deallocated.
  ACE_Auto_Basic_Array_Ptr<char> cmd_ptr (command);

  ACE_OS::sprintf (cmd_ptr.get (),
                   "GET /%s HTTP/1.1\r\n",
                   ACE_TEXT_ALWAYS_CHAR (this->url_addr ().get_path_name ()));

  // Send the GET command to the connected server.
  if (this->stream ().send_n (cmd_ptr.get (),
                              ACE_OS::strlen (cmd_ptr.get ()),
                              const_cast<ACE_Time_Value *>
                                (OPTIONS::instance ()->timeout ()))  > 0)
    {
      ACE_OS::sprintf (cmd_ptr.get (),
                       "Host: %s\r\n\r\n",
                       this->url_addr ().get_host_name ());

      // IMP: The length of teh command has to be sent!
      ssize_t retval =
        this->stream ().send_n (cmd_ptr.get (),
                                ACE_OS::strlen (cmd_ptr.get ()),
                                const_cast<ACE_Time_Value *>
                                  (OPTIONS::instance ()->timeout ()));
      this->stream ().svc_handler ()->idle (0);
      if (retval <= 0)
        return -1;
      else
        return retval;
    }
  else
    return -1;
}

int
HTTP_URL::accept (URL_Visitor *visitor)
{
  // This is part of the visitor pattern.
  return visitor->visit (*this);
}

int
HTTP_URL::destroy (void)
{
  delete this;
  return 0;
  // Commit suicide!
}

// -*- C++ -*-
// Ossama Othman <ossama@uci.edu>

#include "Push_Iterator_Factory_i.h"
#include "Callback_Handler.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_strings.h"

#include <array>
#include <ctime>
#include <string>

Web_Server::Metadata_Type *
Push_Iterator_Factory_i::register_callback
  (char const *pathname,
   Web_Server::Callback_ptr client_callback)
{
  if (CORBA::is_nil (client_callback))  // @@ Will it ever be nil?
    throw CORBA::BAD_PARAM ();

  if (pathname == nullptr)
    throw Web_Server::Error_Result (500);

  // What goes on in this method is a bit strange at first glance
  // since the client can potentially receive all of the data before
  // it ever receives the metadata it needs to display the content of
  // the data.  It is up to the client to ensure that both the content
  // and the metadata are received prior to attempting to display the
  // content.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Received request for file: <%s>\n"),
              pathname));

  std::string const filename (pathname);
  ACE_stat file_status {};
  if (ACE_OS::stat (filename.c_str (), &file_status) == -1)
    // HTTP 1.1 "Internal Server Error".
    throw Web_Server::Error_Result (500);

  Web_Server::Metadata_Type *metadata_ptr {};
  ACE_NEW_THROW_EX (metadata_ptr,
                    Web_Server::Metadata_Type,
                    CORBA::NO_MEMORY ());
  Web_Server::Metadata_Type_var metadata (metadata_ptr);

  if (this->modification_date (file_status,
                               *metadata) != 0)
    // HTTP 1.1 "Internal Server Error.
    throw Web_Server::Error_Result (500);

  if (this->content_type (filename,
                          *metadata) != 0)
    // HTTP 1.1 "Internal Server Error.
    throw Web_Server::Error_Result (500);

  // Start pushing data asynchronously so the server can handle other requests.
  Callback_Handler *handler {};
  ACE_NEW_THROW_EX (handler,
                    Callback_Handler (filename.c_str (), client_callback),
                    CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var servant_owner (handler);
  handler->run ();

  return metadata._retn ();
}

int
Push_Iterator_Factory_i::modification_date
  (ACE_stat const& file_status,
   Web_Server::Metadata_Type & metadata)
{
  // Get the modification time from the file status structure/
  struct tm const *const t_gmt = ACE_OS::gmtime (&file_status.st_mtime);
  if (t_gmt == nullptr)
    return -1;

  // A time string is probably never going to exceed 256 bytes.
  constexpr size_t buflen = 256;
  std::array<char, buflen> buf {};

  // Format the time to conform to RFC 1123.
  if (std::strftime (buf.data (),
                     buf.size (),
                     "%a, %d %b %Y %H:%M:%S GMT",
                     t_gmt) == 0)
    return -1;

  metadata.modification_date = CORBA::string_dup (buf.data ());

  return 0;
}

int
Push_Iterator_Factory_i::content_type (std::string const& filename,
                                       Web_Server::Metadata_Type &metadata)
{
  size_t const extension_start = filename.find_last_of ('.');
  std::string const extension =
    filename.substr (extension_start == std::string::npos ? 0 : extension_start + 1);

  if (ACE_OS::strcasecmp (extension.c_str (), "htm") == 0
      || ACE_OS::strcasecmp (extension.c_str (), "html") == 0)
    metadata.content_type = CORBA::string_dup ("text/html");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "txt") == 0)
    metadata.content_type = CORBA::string_dup ("text/plain");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "ps") == 0)
    metadata.content_type =
      CORBA::string_dup ("application/postscript");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "pdf") == 0)
    metadata.content_type = CORBA::string_dup ("application/pdf");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "jpeg") == 0
           || ACE_OS::strcasecmp (extension.c_str (),
                                  "jpg") == 0)
    metadata.content_type = CORBA::string_dup ("image/jpeg");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "tiff") == 0)
    metadata.content_type = CORBA::string_dup ("image/tiff");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "gif") == 0)
    metadata.content_type = CORBA::string_dup ("image/gif");
  else if (ACE_OS::strcasecmp (extension.c_str (),
                               "png") == 0)
    metadata.content_type = CORBA::string_dup ("image/png");
  else
    {
      metadata.content_type = CORBA::string_dup ("text/html");
      ACE_ERROR ((LM_WARNING,
                  ACE_TEXT ("\n  ")
                  ACE_TEXT ("Unknown file type.  ")
                  ACE_TEXT ("Using \"text/html\" content type.\n")));
    }

  return 0;
}

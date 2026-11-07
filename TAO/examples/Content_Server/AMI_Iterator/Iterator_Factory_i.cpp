// -*- C++ -*-
// Ossama Othman <ossama@uci.edu>

#include "Content_Iterator_i.h"
#include "Iterator_Factory_i.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_strings.h"

#include <array>
#include <ctime>
#include <string>

void
Iterator_Factory_i::get_iterator (char const *pathname,
                                  Web_Server::Content_Iterator_out contents,
                                  Web_Server::Metadata_Type_out metadata)
{
  if (pathname == nullptr)
    throw Web_Server::Error_Result (500);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Received request for file: <%s>\n"),
              pathname));

  std::string const filename (pathname);
  ACE_stat file_status {};
  if (ACE_OS::stat (filename.c_str (), &file_status) == -1)
    // HTTP 1.1 "Internal Server Error".
    throw Web_Server::Error_Result (500);

  Content_Iterator_i *iterator_servant {};
  ACE_NEW_THROW_EX (iterator_servant,
                    Content_Iterator_i (filename.c_str (),
                                        file_status.st_size),
                    CORBA::NO_MEMORY ());
  // This reference is released at function exit; _this() adds the POA reference.
  PortableServer::ServantBase_var servant_owner (iterator_servant);

  if (iterator_servant->init () != 0)
    {
      if (errno == EACCES)
        // HTTP 1.1 "Forbidden".
        throw Web_Server::Error_Result (403);
      else
        // HTTP 1.1 "Internal Server Error".
        throw Web_Server::Error_Result (500);
    }


  // Activate the Content_Iterator object.
  Web_Server::Content_Iterator_var iterator =
    iterator_servant->_this ();

  Web_Server::Metadata_Type_var metadata_value;
  ACE_NEW_THROW_EX (metadata_value.out (),
                    Web_Server::Metadata_Type,
                    CORBA::NO_MEMORY ());

  if (this->modification_date (file_status,
                               *metadata_value) != 0)
    // HTTP 1.1 "Internal Server Error.
    throw Web_Server::Error_Result (500);

  if (this->content_type (filename,
                          *metadata_value) != 0)
    // HTTP 1.1 "Internal Server Error.
    throw Web_Server::Error_Result (500);

  contents = iterator._retn ();
  metadata = metadata_value._retn ();
}

int
Iterator_Factory_i::modification_date (ACE_stat const& file_status,
                                       Web_Server::Metadata_Type& metadata)
{
  // Get the modification time from the file status structure/
  struct tm const *const t_gmt = ACE_OS::gmtime (&file_status.st_mtime);

  // A time string is probably never going to exceed 256 bytes.
  constexpr size_t buflen = 256;
  std::array<char, buflen> buf {};

  // Format the time to conform to RFC 1123.
  if (std::strftime (buf.data (),
                     buf.size (),
                     "%a, %d %b %Y %H:%M:%S GMT",
                     t_gmt) == 0)
    return -1;
  else
    metadata.modification_date = CORBA::string_dup (buf.data ());

  return 0;
}

int
Iterator_Factory_i::content_type (std::string const& filename,
                                  Web_Server::Metadata_Type& metadata)
{
  // Use the final extension when a filename contains multiple periods.
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

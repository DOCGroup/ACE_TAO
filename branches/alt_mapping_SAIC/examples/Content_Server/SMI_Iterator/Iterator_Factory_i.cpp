// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "Content_Iterator_i.h"
#include "Iterator_Factory_i.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

void
Iterator_Factory_i::get_iterator (const char *pathname,
                                  Web_Server::Content_Iterator_out contents,
                                  Web_Server::Metadata_Type_out metadata)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Received request for file: <%s>\n"),
              pathname));

  ACE_stat file_status;
  if (ACE_OS::stat (pathname, &file_status) == -1)
    {
      throw Web_Server::Error_Result (500);
      // HTTP 1.1 "Internal Server Error"
    }

  Content_Iterator_i *iterator_servant = 0;
  ACE_NEW_THROW_EX (iterator_servant,
                    Content_Iterator_i (pathname,
                                        file_status.st_size),
                    CORBA::NO_MEMORY ());

  if (iterator_servant->init () != 0)
    {
      if (errno == EACCES)
        {
          throw Web_Server::Error_Result (403);
          // HTTP 1.1 "Forbidden"
        }
      else
        {
          throw Web_Server::Error_Result (500);
          // HTTP 1.1 "Internal Server Error"
        }
    }


  // Activate the Content_Iterator object.
  Web_Server::Content_Iterator_var iterator =
    iterator_servant->_this ();

  Web_Server::Metadata_Type *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Web_Server::Metadata_Type,
                    CORBA::NO_MEMORY ());

  metadata = tmp;

  if (this->modification_date (&file_status,
                               metadata) != 0)
    {
      throw Web_Server::Error_Result (500);
      // HTTP 1.1 "Internal Server Error
    }

  if (this->content_type (pathname,
                          metadata) != 0)
    {
      throw Web_Server::Error_Result (500);
      // HTTP 1.1 "Internal Server Error
    }

  contents = iterator._retn (); // Make a copy
}

int
Iterator_Factory_i::modification_date (ACE_stat *file_status,
                                       Web_Server::Metadata_Type_out metadata)
{
  // Get the modification time from the file status structure/
  struct tm *t_gmt = ACE_OS::gmtime (&(file_status->st_mtime));

  // A time string is probably never going to exceed 256 bytes.
  const size_t buflen = 256;
  char buf[buflen];

  // Format the time to conform to RFC 1123.
  if (ACE_OS::strftime (buf,
                        buflen,
                        "%a, %d %b %Y %H:%M:%S GMT",
                        t_gmt) == 0)
    return -1;
  else
    metadata->modification_date = CORBA::string_dup (buf);

  return 0;
}

int
Iterator_Factory_i::content_type (const char *filename,
                                  Web_Server::Metadata_Type_out metadata)
{
  if (filename == 0)
    return -1;

  // @@ There are a bunch of const_casts in this method.  It's ugly.
  //    I know.
  //        -Ossama

  size_t const len = ACE_OS::strlen (filename);

  // Search for extension
  // Handle the case where multiple periods exists in the filename,
  // e.g.:  foo.bar.ps
  char * extension = 0;
  for (char * tmp = const_cast<char *> (filename);
       tmp != 0 && tmp != tmp + len;
       )
    {
      tmp = const_cast<char *> (ACE_OS::strchr (tmp, '.'));

      if (tmp != 0)
        extension = ++tmp;  // Skip over the '.'
    }

  if (extension == 0)
    extension = const_cast<char *> (filename);  // No extension!

  if (ACE_OS::strcasecmp (extension, "htm") == 0
      || ACE_OS::strcasecmp (extension, "html") == 0)
    {
      metadata->content_type = CORBA::string_dup ("text/html");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "txt") == 0)
    {
      metadata->content_type = CORBA::string_dup ("text/plain");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "ps") == 0)
    {
      metadata->content_type =
        CORBA::string_dup ("application/postscript");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "pdf") == 0)
    {
      metadata->content_type = CORBA::string_dup ("application/pdf");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "jpeg") == 0
           || ACE_OS::strcasecmp (extension,
                                  "jpg") == 0)
    {
      metadata->content_type = CORBA::string_dup ("image/jpeg");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "tiff") == 0)
    {
      metadata->content_type = CORBA::string_dup ("image/tiff");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "gif") == 0)
    {
      metadata->content_type = CORBA::string_dup ("image/gif");
    }
  else if (ACE_OS::strcasecmp (extension,
                               "png") == 0)
    {
      metadata->content_type = CORBA::string_dup ("image/png");
    }
  else
    {
      metadata->content_type = CORBA::string_dup ("text/html");
      ACE_ERROR ((LM_WARNING,
                  ACE_TEXT ("\n  ")
                  ACE_TEXT ("Unknown file type.  ")
                  ACE_TEXT ("Using \"text/html\" content type.\n")));
    }

  return 0;
}

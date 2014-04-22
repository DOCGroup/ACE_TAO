// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "Push_Iterator_Factory_i.h"
#include "Callback_Handler.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

Web_Server::Metadata_Type *
Push_Iterator_Factory_i::register_callback
  (const char *pathname,
   Web_Server::Callback_ptr client_callback)
{
  if (CORBA::is_nil (client_callback))  // @@ Will it ever be nil?
    throw CORBA::BAD_PARAM ();

  // What goes on in this method is a bit strange at first glance
  // since the client can potentially receive all of the data before
  // it ever receives the metadata it needs to display the content of
  // the data.  It is up to the client to ensure that both the content
  // and the metadata are received prior to attempting to display the
  // content.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Received request for file: <%s>\n"),
              pathname));

  // Send the file to the client asynchronously.  This allows the
  // server to service multiple file requests from clients.
  Callback_Handler *handler = 0;
  ACE_NEW_THROW_EX (handler,
                    Callback_Handler (pathname,
                                      client_callback),
                    CORBA::NO_MEMORY ());

  // Transfer ownership to the POA.
  PortableServer::ServantBase_var tmp (handler);

  // Start sending data to the client callback object.
  handler->run ();

  ACE_stat file_status;
  if (ACE_OS::stat (pathname,
                    &file_status) == -1)
    // HTTP 1.1 "Internal Server Error".
    throw Web_Server::Error_Result (500);

  Web_Server::Metadata_Type *meta_tmp = 0;
  ACE_NEW_THROW_EX (meta_tmp,
                    Web_Server::Metadata_Type,
                    CORBA::NO_MEMORY ());

  Web_Server::Metadata_Type_var metadata = meta_tmp;

  if (this->modification_date (&file_status,
                               metadata.inout ()) != 0)
    // HTTP 1.1 "Internal Server Error.
    throw Web_Server::Error_Result (500);

  if (this->content_type (pathname,
                          metadata.inout ()) != 0)
    // HTTP 1.1 "Internal Server Error.
    throw Web_Server::Error_Result (500);

  return metadata._retn ();
}

int
Push_Iterator_Factory_i::modification_date
  (ACE_stat *file_status,
   Web_Server::Metadata_Type & metadata)
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
    metadata.modification_date = CORBA::string_dup (buf);

  return 0;
}

int
Push_Iterator_Factory_i::content_type (const char *filename,
                                       Web_Server::Metadata_Type &metadata)
{
  if (filename == 0)
    return -1;

  // @@ There are a bunch of const_casts in this method.  It's ugly.
  //    I know.
  //        -Ossama

  size_t len = ACE_OS::strlen (filename);

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
    metadata.content_type = CORBA::string_dup ("text/html");
  else if (ACE_OS::strcasecmp (extension,
                               "txt") == 0)
    metadata.content_type = CORBA::string_dup ("text/plain");
  else if (ACE_OS::strcasecmp (extension,
                               "ps") == 0)
    metadata.content_type =
      CORBA::string_dup ("application/postscript");
  else if (ACE_OS::strcasecmp (extension,
                               "pdf") == 0)
    metadata.content_type = CORBA::string_dup ("application/pdf");
  else if (ACE_OS::strcasecmp (extension,
                               "jpeg") == 0
           || ACE_OS::strcasecmp (extension,
                                  "jpg") == 0)
    metadata.content_type = CORBA::string_dup ("image/jpeg");
  else if (ACE_OS::strcasecmp (extension,
                               "tiff") == 0)
    metadata.content_type = CORBA::string_dup ("image/tiff");
  else if (ACE_OS::strcasecmp (extension,
                               "gif") == 0)
    metadata.content_type = CORBA::string_dup ("image/gif");
  else if (ACE_OS::strcasecmp (extension,
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

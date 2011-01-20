// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/FILE_Connector.h"
#include "ace/Process_Manager.h"
#include "orbsvcs/CosNamingC.h"
#include "Web_ServerC.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

// Retrieve the data from the server
int retrieve_data (const char *content_type,
                   Web_Server::Content_Iterator_ptr contents);

// Map content type to viewer.
int external_viewer (const char *content_type,
                     char *viewer,
                     size_t length);

// Spawn an external viewer
int spawn_viewer (const char *content_type,
                  const char *filename);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (argc < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: client filename\n")),
                          -1);

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "Mighty ORB");

      // Get a reference to the Name Service.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("NameService");

      // Narrow to a Naming Context
      CosNaming::NamingContext_var nc =
        CosNaming::NamingContext::_narrow (obj.in ());

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil reference to ")
                             ACE_TEXT ("Name Service\n")),
                            -1);
        }

      // Create a name.
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("Iterator_Factory");
      name[0].kind = CORBA::string_dup ("");

      obj = nc->resolve (name);

      // Now narrow to an Iterator_Factory reference.
      Web_Server::Iterator_Factory_var factory =
        Web_Server::Iterator_Factory::_narrow (obj.in ());
      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object pointed to by:\n")
                             ACE_TEXT ("%s\n")
                             ACE_TEXT ("is not an Iterator_Factory ")
                             ACE_TEXT ("object.\n"),
                             argv[1]),
                            -1);
        }

      // Get a Content_Iterator
      Web_Server::Content_Iterator_var contents;
      Web_Server::Metadata_Type_var metadata;
      factory->get_iterator (ACE_TEXT_ALWAYS_CHAR(argv[1]),
                             contents,
                             metadata);

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("File <%s> has the following ")
                  ACE_TEXT ("characteristics:\n")
                  ACE_TEXT ("  Modification Date: %s\n")
                  ACE_TEXT ("  Content Type: %s\n"),
                  argv[1],
                  metadata->modification_date.in (),
                  metadata->content_type.in ()));

      int result = ::retrieve_data (metadata->content_type.in (),
                                    contents.in ());

      if (result != 0)
        return -1;

      // Done with the Content_Iterator, so destroy it.
      contents->destroy ();

      orb->shutdown (0);

      // orb->destroy ();
    }
  catch (const Web_Server::Error_Result& exc)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Caught Web Server exception ")
                         ACE_TEXT ("with status %d\n"),
                         exc.status),
                        -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught unexpected exception:"));

      return -1;
    }

  // Wait for all children to exit.
  ACE_Process_Manager::instance ()->wait ();

  return 0;
}


int retrieve_data (const char *content_type,
                   Web_Server::Content_Iterator_ptr iterator)
{
  Web_Server::Content_Iterator_var contents =
    Web_Server::Content_Iterator::_duplicate (iterator);

  // Create a temporary file where the retrieved data will be stored.
  ACE_FILE_Addr file_addr (ACE_sap_any_cast (const ACE_FILE_Addr &));
  ACE_FILE_IO file_io;
  ACE_FILE_Connector connector;

  if (connector.connect (file_io,
                         file_addr,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_CREAT | O_TRUNC | O_WRONLY,
                         ACE_DEFAULT_FILE_PERMS) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Could not open file \"%s\"%p\n"),
                  file_addr.get_path_name ()));
    }

  // Retrieve and store chunks of data.
  Web_Server::Chunk_Type_var chunk;
  CORBA::ULong offset = 0;
  int rc;

  for (;;)
    {
      rc = contents->next_chunk (offset, chunk);

      if (!rc)
        break;

      // Write the received data to a file.
      if (file_io.send (chunk->get_buffer (),
                        chunk->length ()) == -1)
        {
          (void) file_io.close ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("Unable to write retrieved ")
                             ACE_TEXT ("data to file %s\n"),
                             file_addr.get_path_name ()),
                            -1);
        }
      else
        offset += chunk->length ();
    }

  // Done writing to the file.
  (void) file_io.close ();

  // Now spawn a view to display the retrieved data.
  if (::spawn_viewer (content_type,
                      ACE_TEXT_ALWAYS_CHAR(file_addr.get_path_name ())) != 0)
    return -1;

  return 0;
}

int external_viewer (const char *content_type,
                     char *viewer,
                     size_t length)
{
  if (content_type == 0)
    return -1;

  if (ACE_OS::strcasecmp (content_type, "text/html") == 0)
    {
      const char lynx[] = "lynx";
      if (length <= sizeof (lynx))
        return -1;
      else
        ACE_OS::strcpy (viewer, lynx);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "text/plain") == 0)
    {
      const char more[] = "more";
      if (length <= sizeof (more))
        return -1;
      else
        ACE_OS::strcpy (viewer, more);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "application/postscript") == 0)
    {
      const char ghostview[] = "ghostview";
      if (length <= sizeof (ghostview))
        return -1;
      else
        ACE_OS::strcpy (viewer, ghostview);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "application/pdf") == 0)
    {
      const char acroread[] = "acroread";
      if (length <= sizeof (acroread))
        return -1;
      else
        ACE_OS::strcpy (viewer, acroread);
    }
  else if (ACE_OS::strcasecmp (content_type,
                               "image/jpeg") == 0
           || ACE_OS::strcasecmp (content_type,
                                  "image/gif") == 0
           || ACE_OS::strcasecmp (content_type,
                                  "image/tiff") == 0
           || ACE_OS::strcasecmp (content_type,
                                  "image/png") == 0)
    {
      const char xv[] = "xv";
      if (length <= sizeof (xv))
        return -1;
      else
        ACE_OS::strcpy (viewer, xv);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unsupported MIME type: <%s>\n"),
                       content_type),
                      -1);

  return 0;
}

int
spawn_viewer (const char *content_type,
              const char *filename)
{
  char viewer[BUFSIZ];

  if (::external_viewer (content_type,
                         viewer,
                         sizeof (viewer)) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Problem determining which external ")
                       ACE_TEXT ("viewer to use.\n")),
                      -1);

  // Set up the command line that will be used when spawning the
  // external viewer.
  ACE_Process_Options opts;
  opts.command_line (ACE_TEXT ("%s %s"),
                     viewer,
                     filename);

  pid_t result = ACE_Process_Manager::instance ()->spawn (opts);

  switch (result)
    {
    case 0:
      // Child
      return 0;
    case ACE_INVALID_PID:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error during viewer spawn of %p\n"),
                         opts.command_line_buf ()),
                        -1);
    default:
      // Parent
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Spawned viewer <%s> with PID <%d>.\n"),
                  viewer,
                  result));
      break;
    }

  return 0;
}

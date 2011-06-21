// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/FILE_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Process_Manager.h"
#include "Callback_i.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

Callback_i::Callback_i (int *request_count)
  : file_ (ACE_sap_any_cast (ACE_FILE_Addr &)),
    file_io_ (),
    ami_handler_ (),
    metadata_ (),
    last_chunk_ (0),
    lock_ (),
    request_count_ (request_count)
{
  // Create a temporary file to store the retrieved data.
  ACE_FILE_Connector connector;

  if (connector.connect (this->file_io_,
                         this->file_,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_CREAT | O_TRUNC | O_WRONLY,
                         ACE_DEFAULT_FILE_PERMS) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Could not open file \"%s\"%p\n"),
                  this->file_.get_path_name ()));
    }
  else
    (*this->request_count_)++;
}

Callback_i::~Callback_i (void)
{
  (void) this->file_io_.close ();
}

void
Callback_i::next_chunk (const Web_Server::Chunk_Type & chunk_data,
                        CORBA::Boolean last_chunk)
{
  if (!last_chunk)
    {
      Web_Server::Chunk_Type_var chunk = chunk_data;

      // Append the received data to the corresponding
      // buffer/temporary file.
      if (this->file_io_.send (chunk->get_buffer (),
                               chunk->length ()) == -1)
        {
          (*this->request_count_)--;  // Don't wait for more data.

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Unable to write retrieved data to ")
                      ACE_TEXT ("file <%s>"),
                      this->file_.get_path_name ()));
          return;
        }
    }
  else
    {
      {
        ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                           guard,
                           this->lock_));

        this->last_chunk_ = 1;  // Received entire content.
      }

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Wrote retrieved data to file <%s>\n"),
                  this->file_.get_path_name ()));

      (*this->request_count_)--;  // No more data.

      // File retrieval has completed, so spawn an external viewer to
      // display its contents.

      // If the entire metadata has been received, then spawn an
      // external viewer to display the received file.
      if (this->metadata_received ())
        {
          (void) this->file_io_.close ();
          this->deactivate ();

          (void) this->spawn_viewer ();
        }
    }
}

void
Callback_i::metadata (const Web_Server::Metadata_Type &metadata)
{
  try
    {
      {
        ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                           guard,
                           this->lock_));
        this->metadata_ = metadata;
      }

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Retrieved file has the following ")
                  ACE_TEXT ("characteristics:\n")
                  ACE_TEXT ("  Modification Date: %s\n")
                  ACE_TEXT ("  Content Type: %s\n"),
                  this->metadata_.modification_date.in (),
                  this->metadata_.content_type.in ()));

      // If the entire content of the data has been received, then spawn
      // an external viewer to display it.
      if (this->content_received ())
        {
          this->deactivate ();

          (void) this->spawn_viewer ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Caught unexpected exception ")
        ACE_TEXT ("in Callback_i::metdata(...):"));
    }
}

int
Callback_i::metadata_received (void)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            0));

  return (this->metadata_.content_type.in () != 0);
}

int
Callback_i::content_received (void)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            0));

  return this->last_chunk_;
}

int
Callback_i::get_viewer (char *viewer,
                        size_t length)
{
  const char *content_type =
    this->metadata_.content_type.in ();

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
Callback_i::spawn_viewer (void)
{
  char viewer[BUFSIZ];

  if (this->get_viewer (viewer,
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
                     this->file_.get_path_name ());

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

void
Callback_i::deactivate (void)
{
  // Get the POA used when activating the Reply Handler object.
  PortableServer::POA_var poa = this->_default_POA ();

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);

  // Now deactivate the iterator object.
  poa->deactivate_object (oid.in ());
}

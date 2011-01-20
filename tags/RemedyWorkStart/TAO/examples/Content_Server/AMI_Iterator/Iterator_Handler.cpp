// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/FILE_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/Process_Manager.h"
#include "Iterator_Handler.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

Iterator_Handler::Iterator_Handler (void)
  : file_ (ACE_sap_any_cast (const ACE_FILE_Addr &)),
    file_io_ (),
    contents_ (),
    metadata_ (),
    offset_ (0),
    ami_handler_ (),
    request_count_ (0)
{
  // Nothing else
}

Iterator_Handler::~Iterator_Handler (void)
{
  (void) this->file_io_.close ();
}

void
Iterator_Handler::next_chunk (CORBA::Boolean pending_data,
                              const Web_Server::Chunk_Type &chunk_data)
{
  if (pending_data)
    {
      Web_Server::Chunk_Type_var chunk = chunk_data;

      // Append the received data to the corresponding
      // buffer/temporary file.
      if (this->file_io_.send (chunk->get_buffer (),
                               chunk->length ()) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Unable to write retrieved data to ")
                      ACE_TEXT ("file")));
          return;
        }
      else
        this->offset_ += chunk->length ();

      this->contents_->sendc_next_chunk (this->ami_handler_.in (),
                                         this->offset_);

    }
  else
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Wrote retrieved data to file <%s>\n"),
                  this->file_.get_path_name ()));

      (*this->request_count_)--;  // No more data.

      // Done with the iterator, so destroy it.
      this->contents_->sendc_destroy (this->ami_handler_.in ());

      // File retrieval has completed, so spawn an external viewer to
      // display its contents.

      (void) this->spawn_viewer ();
    }
}
void
Iterator_Handler::destroy (void)
{
  // Deactivate this reply handler.
  this->deactivate ();
}


void
Iterator_Handler::run (int *request_count,
                       const char *pathname,
                       Web_Server::Iterator_Factory_ptr factory)
{
  if (request_count != 0)
      this->request_count_ = request_count;
  else
    // @@ Application code shouldn't throw system exceptions.
    throw CORBA::BAD_PARAM ();
  // Initialize the Content Iterator
  this->initialize_content_iterator (pathname,
                                     factory);

  // Activate this Reply Handler.
  this->ami_handler_ = this->_this ();

  // Begin the asynchronous invocation.
  this->contents_->sendc_next_chunk (this->ami_handler_.in (),
                                     this->offset_);
}

void
Iterator_Handler::initialize_content_iterator
  (const char *pathname,
   Web_Server::Iterator_Factory_ptr factory)
{
  // Obtain a Content Iterator for the desired file.
  factory->get_iterator (pathname,
                         this->contents_,
                         this->metadata_);

  // Create a temporary file to store the retrieved data.
  ACE_FILE_Connector connector;

  if (connector.connect (this->file_io_,
                         this->file_,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_CREAT | O_TRUNC | O_WRONLY,
                         ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Could not open file %p\n"),
                this->file_.get_path_name ()));
  else
    (*this->request_count_)++;
}

void
Iterator_Handler::deactivate (void)
{
  // Get the POA used when activating the Reply Handler object.
  PortableServer::POA_var poa =
    this->_default_POA ();

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);

  // Now deactivate the iterator object.
  poa->deactivate_object (oid.in ());
}


int
Iterator_Handler::get_viewer (char *viewer,
                              size_t length)
{
  const char *content_type =
    this->metadata_->content_type.in ();

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
Iterator_Handler::spawn_viewer (void)
{
  char viewer[BUFSIZ];

  if (this->get_viewer (viewer,
                        sizeof viewer) != 0)
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

// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/FILE_Connector.h"
#include "ace/Log_Msg.h"
#include "Callback_Handler.h"

Callback_Handler::Callback_Handler (const char *pathname,
                                    Web_Server::Callback_ptr client_callback)
  : file_ (ACE_TEXT_CHAR_TO_TCHAR(pathname)),
    file_io_ (),
    callback_ (Web_Server::Callback::_duplicate (client_callback)),
    ami_handler_ (),
    chunk_ (new Web_Server::Chunk_Type),
    chunk_index_ (0),
    last_chunk_ (0)
{
  // Nothing else
}

Callback_Handler::~Callback_Handler (void)
{
  // Nothing else
}

void
Callback_Handler::next_chunk (void)
{
  if (this->last_chunk_ == 1)
    {
      this->deactivate ();

      return;
    }

  // Allocate a buffer for the file being read.
  CORBA::Octet *buf =
    Web_Server::Chunk_Type::allocbuf (BUFSIZ);

  if (buf == 0)
    throw CORBA::NO_MEMORY ();

  ssize_t bytes_read = this->file_io_.recv (buf,
                                            BUFSIZ);
  if (bytes_read == -1)
    {
      Web_Server::Chunk_Type::freebuf (buf);

      throw CORBA::INTERNAL ();
    }
  else if (bytes_read == 0)
    {
      Web_Server::Chunk_Type::freebuf (buf);
      this->last_chunk_ = 1;
    }
  else
    {
      this->chunk_index_++;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Sending chunk %d from %s of size <%d>\n"),
                  this->chunk_index_,
                  this->file_.get_path_name (),
                  bytes_read));

      // Place the contents of the buffer into the outgoing octet
      // sequence.  Only replace the amount of data actually read.
      this->chunk_->replace (bytes_read,
                             bytes_read,
                             buf,
                             1);  // The sequence releases memory for us.
    }

  this->callback_->sendc_next_chunk (this->ami_handler_.in (),
                                     this->chunk_.in (),
                                     this->last_chunk_);
}

void
Callback_Handler::next_chunk_excep
  (::Messaging::ExceptionHolder *excep_holder)
{
  this->last_chunk_ = 1;

  try
    {
      this->deactivate ();

      excep_holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Exception occured during ")
        ACE_TEXT ("sendc_next_chunk() call:"));
    }
}

void
Callback_Handler::run (void)
{
  // Open the file to be downloaded
  this->open_file ();

  // Activate this Reply Handler.
  this->ami_handler_ = this->_this ();

  // Begin the asynchronous invocation.  Note that the AMI
  // "sendc_next_chunk()" call is done within the following call,
  // since data must first be read into the Chunk.
  this->next_chunk ();
}

void
Callback_Handler::open_file (void)
{
  // Create a temporary file to store the retrieved data.
  ACE_FILE_Connector connector;

  if (connector.connect (this->file_io_,
                         this->file_,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_RDONLY) == -1)
    // HTTP 1.1 "Not Found"
    throw Web_Server::Error_Result (404);
}

void
Callback_Handler::deactivate (void)
{
  // Close the file that was sent to the client.
  (void) this->file_io_.close ();

  // Get the POA used when activating the Reply Handler object.
  PortableServer::POA_var poa = this->_default_POA ();

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);

  // Now deactivate the AMI_CallbackHandler object.
  poa->deactivate_object (oid.in ());
}

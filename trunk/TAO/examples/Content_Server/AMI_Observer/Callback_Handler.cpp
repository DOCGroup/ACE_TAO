// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/FILE_Connector.h"
#include "ace/Log_Msg.h"
#include "Callback_Handler.h"

ACE_RCSID(AMI_Observer, Callback_Handler, "$Id$")

Callback_Handler::Callback_Handler (const char * pathname,
                                    Web_Server::Callback_ptr client_callback)
  : file_ (pathname),
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
Callback_Handler::next_chunk (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->last_chunk_ == 1)
    {
      this->deactivate (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

  // Allocate a buffer for the file being read.
  CORBA::Octet * buf =
    Web_Server::Chunk_Type::allocbuf (BUFSIZ);

  ssize_t bytes_read = this->file_io_.recv (buf,
                                            BUFSIZ);

  if (bytes_read == -1)
    {
      Web_Server::Chunk_Type::freebuf (buf);

      ACE_THROW (CORBA::INTERNAL ());
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
                                     this->last_chunk_,
                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
Callback_Handler::next_chunk_excep (
    Web_Server::AMI_CallbackExceptionHolder *excep_holder,
    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->last_chunk_ = 1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->deactivate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      excep_holder->raise_next_chunk (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Exception occured during ")
                           ACE_TEXT ("sendc_next_chunk() call:"));
    }
  ACE_ENDTRY;
}

ACE_HANDLE
Callback_Handler::run (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Web_Server::Error_Result))
{
  // Open the file to be downloaded
  this->open_file (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  // Activate this Reply Handler.
  this->ami_handler_ = this->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  // Begin the asynchronous invocation.
  // Note that the AMI "sendc_next_chunk()" call is done within the
  // following call, since data must first be read into the Chunk.
  this->next_chunk (ACE_TRY_ENV);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  return this->file_io_.get_handle ();
}

void
Callback_Handler::open_file (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Web_Server::Error_Result))
{
  // Create a temporary file to store the retrieved data.
  ACE_FILE_Connector connector;

  if (connector.connect (this->file_io_,
                         this->file_,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_RDONLY) == -1)
    {
      ACE_THROW (Web_Server::Error_Result (404));
      // HTTP 1.1 "Not Found"
    }
}


void
Callback_Handler::deactivate (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Close the file that was sent to the client.
  (void) this->file_io_.close ();

  // Get the POA used when activating the Reply Handler object.
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this,
                        ACE_TRY_ENV);
  ACE_CHECK;

  // Now deactivate the AMI_CallbackHandler object.
  poa->deactivate_object (oid.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "Content_Iterator_i.h"

ACE_RCSID(Content_Server, Content_Iterator_i, "$Id$")


Content_Iterator_i::Content_Iterator_i (ACE_HANDLE handle,
                                        CORBA::ULong file_size)
  : handle_ (handle),
    file_size_ (file_size),
    chunk_index_ (1)
{
  // Nothing else
}

Content_Iterator_i::~Content_Iterator_i (void)
{
  if (this->handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->handle_);
}

CORBA::Boolean
Content_Iterator_i::next_chunk (CORBA::ULong offset,
                                Web_Server::Chunk_Type_out chunk,
                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Initialize/allocate the Chunk_Type sequence
  chunk = new Web_Server::Chunk_Type;

  if (offset >= this->file_size_
      || this->handle_ == ACE_INVALID_HANDLE)
    return 0;  // Applications shouldn't throw system exceptions.

  off_t real_offset =
    ACE_OS::lseek (this->handle_,
                   offset,
                   SEEK_SET);

  if (real_offset == (off_t) -1)
    {
      // Invalid supplied offset?

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "Error during lseek"),
                        0);
    }
  else if (offset != ACE_static_cast (CORBA::ULong, real_offset))
    {
      // Didn't get the desired offset.

      // @@ Is this precaution necessary?

      // Reset the file offset back to its original value.
      
      (void) ACE_OS::lseek (this->handle_,
                            real_offset,
                            SEEK_SET);

      ACE_DEBUG ((LM_ERROR,
                  "Unable to reposition to desired offset.\n"));

      return 0;
    }

  // Allocate a buffer for the file being read.
  CORBA::Octet * buf =
    Web_Server::Chunk_Type::allocbuf (BUFSIZ);

  ssize_t bytes_read = ACE_OS::read (this->handle_,
                                     buf,
                                     BUFSIZ);

  if (bytes_read == -1)
    {
      Web_Server::Chunk_Type::freebuf (buf);

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "Error during read"),
                        0);
    }

  ACE_DEBUG ((LM_DEBUG,
              "Sending chunk %d at offset <%u> of size <%u>\n",
              this->chunk_index_,
              offset,
              bytes_read));

  // Place the contents of the buffer into the outgoing octet
  // sequence.  Only replace the amount of data actually read.
  chunk->replace (bytes_read,
                  bytes_read,
                  buf,
                  1);  // The sequence releases memory for us.

  this->chunk_index_++;

  return 1;
}

void
Content_Iterator_i::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::close (this->handle_);
      this->handle_ = ACE_INVALID_HANDLE;
    }

  // Get the POA used when activating the Content_Iterator object.
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid = poa->servant_to_id (this,
                                                         ACE_TRY_ENV);
  ACE_CHECK;

  // Now deactivate the iterator object.
  poa->deactivate_object (oid.in (), ACE_TRY_ENV);
  ACE_CHECK;

  // Decrease the reference count on our selves.
  this->_remove_ref ();
}


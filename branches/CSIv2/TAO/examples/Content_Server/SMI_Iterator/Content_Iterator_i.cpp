// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/FILE_Connector.h"
#include "Content_Iterator_i.h"

ACE_RCSID(SMI_Iterator, Content_Iterator_i, "$Id$")


Content_Iterator_i::Content_Iterator_i (const char *pathname,
                                        CORBA::ULong file_size)
  : file_ (pathname),
    file_io_ (),
    file_size_ (file_size),
    chunk_index_ (1)
{
  // Nothing else
}

Content_Iterator_i::~Content_Iterator_i (void)
{
  (void) this->file_io_.close ();
}

CORBA::Boolean
Content_Iterator_i::next_chunk (CORBA::ULong offset,
                                Web_Server::Chunk_Type_out chunk
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Initialize/allocate the Chunk_Type sequence
  Web_Server::Chunk_Type *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Web_Server::Chunk_Type,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  chunk = tmp;

  if (offset >= this->file_size_)
    return 0;  // Applications shouldn't throw system exceptions.

  off_t real_offset =
    ACE_OS::lseek (this->file_io_.get_handle (),
                   offset,
                   SEEK_SET);

  if (real_offset == (off_t) -1)
    {
      // Invalid supplied offset?

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Error during lseek")),
                        0);
    }
  else if (offset != ACE_static_cast (CORBA::ULong, real_offset))
    {
      // Didn't get the desired offset.

      // @@ Is this precaution necessary?

      // Reset the file offset back to its original value.

      (void) ACE_OS::lseek (this->file_io_.get_handle (),
                            real_offset,
                            SEEK_SET);

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to reposition to desired ")
                         ACE_TEXT ("offset.\n")),
                        0);
    }

  // Allocate a buffer for the file being read.
  CORBA::Octet * buf =
    Web_Server::Chunk_Type::allocbuf (BUFSIZ);

  if (buf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Could not allocate chunk buffer\n")),
                        0);
    }

  ssize_t bytes_read = this->file_io_.recv (buf,
                                            BUFSIZ);

  if (bytes_read == -1)
    {
      Web_Server::Chunk_Type::freebuf (buf);

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Error during read")),
                        0);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sending chunk %d at offset <%u> of size <%u>\n"),
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
Content_Iterator_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  (void) this->file_io_.close ();

  // Get the POA used when activating the Content_Iterator object.
  PortableServer::POA_var poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid = poa->servant_to_id (this
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Now deactivate the iterator object.
  poa->deactivate_object (oid.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Decrease the reference count on our selves.
  this->_remove_ref ();
}

int
Content_Iterator_i::init (void)
{
  // Open the requested file.
  ACE_FILE_Connector connector;

  if (connector.connect (this->file_io_,
                         this->file_,
                         0,
                         ACE_Addr::sap_any,
                         0,
                         O_RDONLY) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%s %p\n"),
                         ACE_TEXT ("Could not open file"),
                         this->file_.get_path_name ()),
                        -1);
    }

  return 0;
}

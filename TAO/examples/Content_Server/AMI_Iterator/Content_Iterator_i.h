// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     ECE255
//
// = FILENAME
//     Content_Iterator_i.h
//
// = DESCRIPTION
//     Header file for the Web_Server::Content_Iterator implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef CONTENT_ITERATOR_I_H
#define CONTENT_ITERATOR_I_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Content_Iterator_i :
  public virtual POA_Web_Server::Content_Iterator,
  public virtual PortableServer::RefCountServantBase
  // = TITLE
  //Implement the Web_Server::Content_Iterator interface.
{
  friend class Iterator_Factory_i;
public:

  // Constructor
  Content_Iterator_i (const char *filename, CORBA::ULong file_size);

  // Destructor
  ~Content_Iterator_i (void);

  // This operation returns the next <chunk> of the file starting at
  // <offset>.  If there are no more bindings, false is returned.
  virtual CORBA::Boolean next_chunk (CORBA::ULong offset,
                                     Web_Server::Chunk_Type_out chunk,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Destroy the iterator.
  virtual void destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  int init (void);
  // Initialize the Content_Iterator.

private:

  ACE_FILE_Addr file_;
  // The Addr representing the requested file.

  ACE_FILE_IO file_io_;
  // Object that handles all IO operations on the requested file.

  CORBA::ULong file_size_;
  // The size of the file being iterated over.

  CORBA::ULong chunk_index_;
  // The number of the current chunk of data being sent.  (Used only
  // for debugging purposes.)

  int initialized_;
  // Flag that denotes that the Content Iterator has not been
  // initialized.
};

#endif  /* CONTENT_ITERATOR_I_H */

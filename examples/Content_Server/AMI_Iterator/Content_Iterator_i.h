// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     AMI_Iterator
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
  public virtual POA_Web_Server::Content_Iterator
{
  // = TITLE
  //    Implement the Web_Server::Content_Iterator interface.
  //
  // = DESCRIPTION
  //    This class implements the Iterator pattern to minimize memory
  //    requirements when retrieving data from a content server.
  //    Rather than retrieving one large chunk of data, this class
  //    iterates on the server so that smaller chunks of data are
  //    retrieved.

  friend class Iterator_Factory_i;
public:

  Content_Iterator_i (const char *filename, CORBA::ULongLong file_size);
  // Constructor

  ~Content_Iterator_i (void);
  // Destructor

  virtual CORBA::Boolean next_chunk (CORBA::ULongLong offset,
                                     Web_Server::Chunk_Type_out chunk);
  // This operation returns the next <chunk> of the file starting at
  // <offset>.  If there are no more bindings, false is returned.

  virtual void destroy (void);
  // Destroy the iterator.

private:

  int init (void);
  // Initialize the Content_Iterator.

private:

  ACE_FILE_Addr file_;
  // The Addr representing the requested file.

  ACE_FILE_IO file_io_;
  // Object that handles all IO operations on the requested file.

  CORBA::ULongLong file_size_;
  // The size of the file being iterated over.

  CORBA::ULongLong chunk_index_;
  // The number of the current chunk of data being sent.  (Used only
  // for debugging purposes.)
};

#endif  /* CONTENT_ITERATOR_I_H */

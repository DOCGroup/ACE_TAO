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

#include "Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Implement the Web_Server::Content_Iterator interface.
class Content_Iterator_i :
  public virtual POA_Web_Server::Content_Iterator,
  public virtual PortableServer::RefCountServantBase
{
public:

  // Constructor
  Content_Iterator_i (ACE_HANDLE handle, CORBA::ULong file_size);

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

  ACE_HANDLE handle_;
  // The file descriptor of the open file being iterated over.

  CORBA::ULong file_size_;
  // The size of the file being iterated over.

  CORBA::ULong chunk_index_;
  // The number of the current chunk of data being sent.  (Used only
  // for debugging purposes.)
};




#endif  /* CONTENT_ITERATOR_I_H */

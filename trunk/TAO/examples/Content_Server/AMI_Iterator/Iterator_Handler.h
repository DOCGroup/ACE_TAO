// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     AMI_Iterator
//
// = FILENAME
//     Iterator_Handler.h
//
// = DESCRIPTION
//     Header file for the Web_Server::AMI_Content_IteratorHandler
//     implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef ITERATOR_HANDLER_H
#define ITERATOR_HANDLER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Iterator_Handler
  : public virtual POA_Web_Server::AMI_Content_IteratorHandler
{
  // = TITLE
  //    The AMI reply handler for replies from the Content_Iterator.
  //
  // = DESCRIPTION
  //    This class handles all asynchronously received replies from
  //    the Content_Iterator on the server side.  Once a reply is
  //    handled, another asynchronous request is made to the
  //    Content_Iterator.  This ensures that chunks of data are
  //    received in the proper order, and also takes advantage of
  //    asyncronous method invocations.

  friend class Iterator_Handler_Friend;
  // Dummy friend class declaration to quiet down a warning.

public:

  Iterator_Handler (void);
  // Constructor that creates a content iterator corresponding to the
  // name of the file being retrieved from the web server.

  virtual void next_chunk (CORBA::Boolean pending_data,
                   const Web_Server::Chunk_Type & chunk);

  virtual void next_chunk_excep (
      ::Messaging::ExceptionHolder *)
    {}

  virtual void destroy (void);

  virtual void destroy_excep (
      ::Messaging::ExceptionHolder *)
    {}

  void run (int *request_count,
            const char *pathname,
            Web_Server::Iterator_Factory_ptr factory);
  // Activate and run this Reply Handler.  The contents (not the
  // pointer itself) of the <request_count> parameter will be
  // incremented when file retrieval begins, and decremented when file
  // retrieval completes.

private:
  ~Iterator_Handler (void);
  // Destructor (private to ensure that Iterator_Handler is allocated
  // on the heap).

  void initialize_content_iterator (const char *pathname,
                                    Web_Server::Iterator_Factory_ptr factory);
  // Initialize the Content Iterator used to retrieve the file.

  void deactivate (void);
  // Deactivate this reply handler.

  int get_viewer (char *viewer, size_t length);
  // Get the name of the viewer associated with the file being
  // retrieved.

  int spawn_viewer (void);
  // Spawn an external view to display the retrieved file.

private:

  ACE_FILE_Addr file_;
  // The Addr corresponding to the retrieved file.

  ACE_FILE_IO file_io_;
  // The object that provides all file related IO operations
  // (e.g. read, write, etc).

  Web_Server::Content_Iterator_var contents_;
  // The iterator used to obtain individual chunks of data from the
  // web server.

  Web_Server::Metadata_Type_var metadata_;
  // The metadata associated with the file being retrieved from the
  // web server.

  CORBA::ULong offset_;
  // The offset from the beginning of the file, the next chunk of data
  // should be read from.

  Web_Server::AMI_Content_IteratorHandler_var ami_handler_;
  // Reference to this Reply Handler's self.

  int *request_count_;
  // Pointer to external status monitoring variable. The contents (not
  // the pointer itself) of the<pending_data> parameter will be
  // decremented when file retrieval has completed.
};

#endif  /* ITERATOR_HAHNDLER_H */

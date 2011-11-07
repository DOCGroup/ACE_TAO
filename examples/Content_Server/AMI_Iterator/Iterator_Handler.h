// -*- C++ -*-

//=============================================================================
/**
 *  @file     Iterator_Handler.h
 *
 *  $Id$
 *
 *   Header file for the Web_Server::AMI_Content_IteratorHandler
 *   implementation.
 *
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


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

  /// Dummy friend class declaration to quiet down a warning.
  friend class Iterator_Handler_Friend;

public:

  /// Constructor that creates a content iterator corresponding to the
  /// name of the file being retrieved from the web server.
  Iterator_Handler (void);

  virtual void next_chunk (CORBA::Boolean pending_data,
                   const Web_Server::Chunk_Type & chunk);

  virtual void next_chunk_excep (
      ::Messaging::ExceptionHolder *)
    {}

  virtual void destroy (void);

  virtual void destroy_excep (
      ::Messaging::ExceptionHolder *)
    {}

  /**
   * Activate and run this Reply Handler.  The contents (not the
   * pointer itself) of the <request_count> parameter will be
   * incremented when file retrieval begins, and decremented when file
   * retrieval completes.
   */
  void run (int *request_count,
            const char *pathname,
            Web_Server::Iterator_Factory_ptr factory);

private:
  /// Destructor (private to ensure that Iterator_Handler is allocated
  /// on the heap).
  ~Iterator_Handler (void);

  /// Initialize the Content Iterator used to retrieve the file.
  void initialize_content_iterator (const char *pathname,
                                    Web_Server::Iterator_Factory_ptr factory);

  /// Deactivate this reply handler.
  void deactivate (void);

  /// Get the name of the viewer associated with the file being
  /// retrieved.
  int get_viewer (char *viewer, size_t length);

  /// Spawn an external view to display the retrieved file.
  int spawn_viewer (void);

private:

  /// The Addr corresponding to the retrieved file.
  ACE_FILE_Addr file_;

  /// The object that provides all file related IO operations
  /// (e.g. read, write, etc).
  ACE_FILE_IO file_io_;

  /// The iterator used to obtain individual chunks of data from the
  /// web server.
  Web_Server::Content_Iterator_var contents_;

  /// The metadata associated with the file being retrieved from the
  /// web server.
  Web_Server::Metadata_Type_var metadata_;

  /// The offset from the beginning of the file, the next chunk of data
  /// should be read from.
  CORBA::ULong offset_;

  /// Reference to this Reply Handler's self.
  Web_Server::AMI_Content_IteratorHandler_var ami_handler_;

  /**
   * Pointer to external status monitoring variable. The contents (not
   * the pointer itself) of the<pending_data> parameter will be
   * decremented when file retrieval has completed.
   */
  int *request_count_;
};

#endif  /* ITERATOR_HAHNDLER_H */

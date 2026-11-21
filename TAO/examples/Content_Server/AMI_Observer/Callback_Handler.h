// -*- C++ -*-

//=============================================================================
/**
 *  @file     Callback_Handler.h
 *
 *   Header file for the Web_Server::AMI_CallbackHandler
 *   implementation.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef CALLBACK_HANDLER_H
#define CALLBACK_HANDLER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Class that asynchronously sends chunks of data to the client-side
/// <Callback> object and handles all asynchronous replies from it.
///
/// The <Push_Iterator_Factory_i> object in the Content Server creates a
/// <Callback_Handler> instance for each requested file and executes its
/// run() method. To let the Content Server service other requests while a
/// file is being sent, run() invokes next_chunk(), which reads the first
/// chunk and sends it asynchronously to the client-side <Callback> object
/// using sendc_next_chunk().
///
/// The next_chunk() method also handles replies from sendc_next_chunk(). Its
/// initial invocation starts asynchronous chunk sending; subsequent calls
/// happen asynchronously. A new chunk is sent only after the client-side
/// <Callback> replies, which keeps chunks in order. This cycle continues
/// until the last chunk is sent.
///
/// No threads are explicitly created at the application level; concurrency
/// results from performing all operations asynchronously.
class Callback_Handler
  : public virtual POA_Web_Server::AMI_CallbackHandler
{
  /// Dummy friend class declaration to quiet down a warning.
  friend class Callback_Handler_Friend;

public:
  Callback_Handler (Callback_Handler const&) = delete;
  Callback_Handler (Callback_Handler&&) = delete;
  Callback_Handler& operator= (Callback_Handler const&) = delete;
  Callback_Handler& operator= (Callback_Handler&&) = delete;

  /// Constructor that creates a content iterator corresponding to the
  /// name of the file being retrieved from the web server.
  Callback_Handler (char const *pathname,
                    Web_Server::Callback_ptr callback);

  /// The callback for this reply handler.
  void next_chunk () override;

  void next_chunk_excep (::Messaging::ExceptionHolder *) override;

  /// Activate this Reply Handler and begin sending file chunks asynchronously.
  void run ();

private:
  /// Destructor (private to ensure that Callback_Handler is allocated
  /// on the heap).
  ~Callback_Handler () override = default;

  /// Open the file to be uploaded to the client callback.
  void open_file ();

  /// Deactivate this reply handler.
  void deactivate ();

private:
  /// The Addr corresponding to the retrieved file.
  ACE_FILE_Addr file_;

  /// The object that provides all file related IO operations
  /// (e.g. read, write, etc).
  ACE_FILE_IO file_io_;

  /// The iterator used to obtain individual chunks of data from the
  /// web server.
  Web_Server::Callback_var callback_;

  /// Reference to this Reply Handler's self.
  Web_Server::AMI_CallbackHandler_var ami_handler_;

  /// The chunk of data that is sent to the client callback during each
  /// callback invocation.
  Web_Server::Chunk_Type_var chunk_;

  /// The number of the current chunk of data being sent.  (Used only
  /// for debugging purposes.)
  CORBA::ULong chunk_index_ {};

  /// Flag that indicates all chunks of data have been sent.
  bool last_chunk_ {};
};

#endif  /* CALLBACK_HANDLER_H */

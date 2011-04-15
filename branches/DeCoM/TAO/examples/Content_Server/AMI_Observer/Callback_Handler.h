// -*- C++ -*-

//=============================================================================
/**
 *  @file     Callback_Handler.h
 *
 *  $Id$
 *
 *   Header file for the Web_Server::AMI_CallbackHandler
 *   implementation.
 *
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

class Callback_Handler
  : public virtual POA_Web_Server::AMI_CallbackHandler
{
  // = TITLE
  //    Class that asynchronously sends chunks of data to the
  //    client-side <Callback> object, and handles all asynchronous
  //    replies emanating from it.
  //
  // = DESCRIPTION
  //    The <Push_Iterator_Factory_i> object in the Content Server
  //    creates a <Callback_Handler> instance for each requested file,
  //    and executes the run() method in that instance (in this
  //    class).  To allow the Content Server to service other requests
  //    without having to wait for the requested file to be completely
  //    sent to the client, the run() method in this class issues the
  //    next_chunk() method, which reads the first chunk of data and
  //    sends it asynchronously to the client-side <Callback> object
  //    by issuing a sendc_next_chunk() call.
  //
  //    This may seem a bit odd since the next_chunk() method in this
  //    class is actually the reply handler method for the
  //    sendc_next_chunk() method.  The next_chunk() method is
  //    initially invoked as a means to bootstrap the process of
  //    asynchronously sending chunks of data to the client-side
  //    <Callback> object.  However, since the next_chunk() method
  //    actually invokes sendc_next_chunk(), all subsequent calls will
  //    be performed asynchronously.  This design also guarantees that
  //    the client-side <Callback> object will receive all chunks of
  //    data in the proper order since the next chunk of data will not
  //    be sent until this <Callback_Handler> receives the asynchronous
  //    reply from the client-side <Callback> object.  Again, that
  //    asynchronous reply is handled by the next_chunk() method, at
  //    which point the entire cycle is started again until the last
  //    chunk of data is sent.
  //
  //    Notice that no threads are explicitly created at the
  //    application level, yet concurrency is achieved due to the fact
  //    that all operations are performed asynchronously.

  /// Dummy friend class declaration to quiet down a warning.
  friend class Callback_Handler_Friend;

public:
  /// Constructor that creates a content iterator corresponding to the
  /// name of the file being retrieved from the web server.
  Callback_Handler (const char *pathname,
                    Web_Server::Callback_ptr callback);

  /// The callback for this reply handler.
  virtual void next_chunk (void);

  virtual void next_chunk_excep (::Messaging::ExceptionHolder *);

  /**
   * Activate and run this Reply Handler.  The contents (not the
   * pointer itself) of the <request_count> parameter will be
   * incremented when file retrieval begins, and decremented when file
   * retrieval completes.
   */
  void run (void);

private:
  /// Destructor (private to ensure that Callback_Handler is allocated
  /// on the heap).
  ~Callback_Handler (void);

  /// Open the file to be uploaded to the client callback.
  void open_file (void);

  /// Deactivate this reply handler.
  void deactivate (void);

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
  CORBA::ULong chunk_index_;

  /// Flag that indicates all chunks of data have been sent.
  int last_chunk_;
};

#endif  /* CALLBACK_HANDLER_H */

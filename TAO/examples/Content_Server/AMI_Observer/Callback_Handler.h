// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     AMI_Observer
//
// = FILENAME
//     Callback_Handler.h
//
// = DESCRIPTION
//     Header file for the Web_Server::AMI_CallbackHandler
//     implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef CALLBACK_HANDLER_H
#define CALLBACK_HANDLER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Callback_Handler
  : public virtual POA_Web_Server::AMI_CallbackHandler,
    public virtual PortableServer::RefCountServantBase
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

  friend class Callback_Handler_Friend;
  // Dummy friend class declaration to quiet down a warning.

public:
  Callback_Handler (const char *pathname,
                    Web_Server::Callback_ptr callback);
  // Constructor that creates a content iterator corresponding to the
  // name of the file being retrieved from the web server.

  virtual void next_chunk (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The callback for this reply handler.

  virtual void next_chunk_excep (Web_Server::AMI_CallbackExceptionHolder *
                                 TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void run (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Web_Server::Error_Result));
  // Activate and run this Reply Handler.  The contents (not the
  // pointer itself) of the <request_count> parameter will be
  // incremented when file retrieval begins, and decremented when file
  // retrieval completes.

private:
  ~Callback_Handler (void);
  // Destructor (private to ensure that Callback_Handler is allocated
  // on the heap).

  void open_file (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                   Web_Server::Error_Result));
  // Open the file to be uploaded to the client callback.

  void deactivate (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Deactivate this reply handler.

private:

  ACE_FILE_Addr file_;
  // The Addr corresponding to the retrieved file.

  ACE_FILE_IO file_io_;
  // The object that provides all file related IO operations
  // (e.g. read, write, etc).

  Web_Server::Callback_var callback_;
  // The iterator used to obtain individual chunks of data from the
  // web server.

  Web_Server::AMI_CallbackHandler_var ami_handler_;
  // Reference to this Reply Handler's self.

  Web_Server::Chunk_Type_var chunk_;
  // The chunk of data that is sent to the client callback during each
  // callback invocation.

  CORBA::ULong chunk_index_;
  // The number of the current chunk of data being sent.  (Used only
  // for debugging purposes.)

  int last_chunk_;
  // Flag that indicates all chunks of data have been sent.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* CALLBACK_HANDLER_H */

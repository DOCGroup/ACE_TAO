// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     Content_Server
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

#include "ace/pre.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Callback_Handler
  : public virtual POA_Web_Server::AMI_CallbackHandler,
    public virtual PortableServer::RefCountServantBase
{
public:

  Callback_Handler (const char * pathname,
                    Web_Server::Callback_ptr callback);
  // Constructor that creates a content iterator corresponding to the
  // name of the file being retrieved from the web server.

  virtual void next_chunk (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The callback for this reply handler.

  virtual void next_chunk_excep (Web_Server::AMI_CallbackExceptionHolder *,
                                 CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ACE_HANDLE run (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Web_Server::Error_Result));
  // Activate and run this Reply Handler.  The contents (not the
  // pointer itself) of the <request_count> parameter will be
  // incremented when file retrieval begins, and decremented when file
  // retrieval completes.

private:

  ~Callback_Handler (void);
  // Destructor  (private to ensure that Callback_Handler is allocated
  // on the heap).

  void open_file (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                   Web_Server::Error_Result));
  // Open the file to be uploaded to the client callback.

  void deactivate (CORBA::Environment &ACE_TRY_ENV)
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

#include "ace/post.h"

#endif  /* CALLBACK_HANDLER_H */

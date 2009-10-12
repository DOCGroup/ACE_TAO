// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     AMI_Observer
//
// = FILENAME
//     Push_Iterator_Handler.h
//
// = DESCRIPTION
//     Header file for the AMI_Iterator_FactoryHandler implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef PUSH_ITERATOR_HANDLER_H
#define PUSH_ITERATOR_HANDLER_H

#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
class Callback_i;

class Push_Iterator_Handler
  : public virtual POA_Web_Server::AMI_Iterator_FactoryHandler
{
  // = TITLE
  //    Implement the Web_Server::Iterator_Factory AMI Reply Handler.
  //
  // = DESCRIPTION
  //    This classes asynchronously registers a reference to the
  //    client-side <Callback> object in the Content Server.  Once
  //    registered, the <Callback> will begin receiving chunks of data
  //    pushed from the Content Server.
  //
  //    Since registration occurs asynchronously, it is possible that
  //    the metadata returned from the <Iterator_Factory> will be
  //    received after the content of the file has been received.  The
  //    <Callback> object is equipped to handle this possibility.
  //
  //    The register_callback() reply handler method in this class
  //    receives the metadata, and then passes it to the <Callback>
  //    object.  Once the metadata is received, there is no longer any
  //    use for this <Push_Iterator_Handler> object, and it is
  //    deactivated.

  friend class Push_Iterator_Handler_Friend;
  // Dummy friend class declaration to quiet down a warning.

public:
  Push_Iterator_Handler (void);
  // Constructor that creates a content iterator corresponding to the
  // name of the file being retrieved from the web server.

  void run (int *request_count,
            const char *pathname,
            Web_Server::Iterator_Factory_ptr factory);
  // Activate and run this Reply Handler.  The contents (not the
  // pointer itself) of the <request_count> parameter will be
  // incremented when file retrieval begins, and decremented when file
  // retrieval completes.

private:

  ~Push_Iterator_Handler (void);
  // Destructor  (private to ensure that Iterator_Handler is allocated
  // on the heap).

  virtual void register_callback
  (const Web_Server::Metadata_Type &metadata);
  // AMI callback that is invoked when a response from the
  // corresponding server method is received.

  virtual void register_callback_excep
  (::Messaging::ExceptionHolder *)
    {}

  void deactivate (void);
  // Deactivate this handler.

private:
  Callback_i *callback_servant_;
  // The servant that accepts data pushed from the content server.

  Web_Server::Callback_var callback_;
  // Reference to the callback object the server sends data to.

  Web_Server::AMI_Iterator_FactoryHandler_var ami_handler_;
  // Reference to this Reply Handler's self.
};

#endif  /* PUSH_ITERATOR_HANDLER_H */

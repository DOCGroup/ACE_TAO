// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

// ============================================================================
//
// = LIBRARY
//     ECE255
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

#include "ace/pre.h"

#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
class Callback_i;

class Push_Iterator_Handler
  : public virtual POA_Web_Server::AMI_Iterator_FactoryHandler,
    public virtual PortableServer::RefCountServantBase
{
public:

  Push_Iterator_Handler (void);
  // Constructor that creates a content iterator corresponding to the
  // name of the file being retrieved from the web server.

  void run (int * request_count,
            const char *pathname,
            Web_Server::Iterator_Factory_ptr factory,
            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Web_Server::Error_Result));
  // Activate and run this Reply Handler.  The contents (not the
  // pointer itself) of the <request_count> parameter will be
  // incremented when file retrieval begins, and decremented when file
  // retrieval completes.

private:

  ~Push_Iterator_Handler (void);
  // Destructor  (private to ensure that Iterator_Handler is allocated
  // on the heap).

  virtual void register_callback (
      const Web_Server::Metadata_Type & metadata,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AMI callback that is invoked when a response from the
  // corresponding server method is received.

  virtual void register_callback_excep (
      Web_Server::AMI_Iterator_FactoryExceptionHolder *,
      CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException)) {}

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate this handler.

private:
  Callback_i * callback_servant_;
  // The servant that accepts data pushed from the content server.

  Web_Server::Callback_var callback_;
  // Reference to the callback object the server sends data to.

  Web_Server::AMI_Iterator_FactoryHandler_var ami_handler_;
  // Reference to this Reply Handler's self.
};

#include "ace/post.h"

#endif  /* PUSH_ITERATOR_HANDLER_H */

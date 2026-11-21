// -*- C++ -*-

//=============================================================================
/**
 *  @file     Push_Iterator_Handler.h
 *
 *   Header file for the AMI_Iterator_FactoryHandler implementation.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef PUSH_ITERATOR_HANDLER_H
#define PUSH_ITERATOR_HANDLER_H

#include "Push_Web_ServerS.h"

#include <atomic>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration
class Callback_i;

/// Implements the Web_Server::Iterator_Factory AMI reply handler.
///
/// This class asynchronously registers a reference to the client-side
/// <Callback> object in the Content Server. Once registered, the <Callback>
/// begins receiving chunks pushed from the Content Server.
///
/// Since registration is asynchronous, metadata returned from the
/// <Iterator_Factory> may arrive after the file content. The <Callback>
/// handles this possibility. The register_callback() reply handler receives
/// the metadata and passes it to the <Callback>. After the metadata arrives,
/// this <Push_Iterator_Handler> is no longer needed and is deactivated.
class Push_Iterator_Handler
  : public virtual POA_Web_Server::AMI_Iterator_FactoryHandler
{
  /// Dummy friend class declaration to quiet down a warning.
  friend class Push_Iterator_Handler_Friend;

public:
  Push_Iterator_Handler (Push_Iterator_Handler const&) = delete;
  Push_Iterator_Handler (Push_Iterator_Handler&&) = delete;
  Push_Iterator_Handler& operator= (Push_Iterator_Handler const&) = delete;
  Push_Iterator_Handler& operator= (Push_Iterator_Handler&&) = delete;

  /// Constructor that creates a content iterator corresponding to the
  /// name of the file being retrieved from the web server.
  Push_Iterator_Handler () = default;

  /**
   * Activate and run this Reply Handler.  The contents (not the
   * pointer itself) of the <request_count> parameter will be
   * incremented when file retrieval begins, and decremented when file
   * retrieval completes.
   */
  void run (std::atomic<int>& request_count,
            char const *pathname,
            Web_Server::Iterator_Factory_ptr factory);

private:
  /// Destructor  (private to ensure that Iterator_Handler is allocated
  /// on the heap).
  ~Push_Iterator_Handler () override = default;

  /// AMI callback that is invoked when a response from the
  /// corresponding server method is received.
  void register_callback (Web_Server::Metadata_Type const& metadata) override;

  void register_callback_excep (::Messaging::ExceptionHolder *) override
    {}

  /// Deactivate this handler.
  void deactivate ();

private:
  /// Non-owning reference to the POA-managed servant, kept until metadata arrives.
  Callback_i *callback_servant_ {};

  /// Reference to the callback object the server sends data to.
  Web_Server::Callback_var callback_;

  /// Reference to this Reply Handler's self.
  Web_Server::AMI_Iterator_FactoryHandler_var ami_handler_;
};

#endif  /* PUSH_ITERATOR_HANDLER_H */

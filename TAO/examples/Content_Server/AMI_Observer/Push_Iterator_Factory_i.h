// -*- C++ -*-

//=============================================================================
/**
 *  @file     Push_Iterator_Factory_i.h
 *
 *   Header file for the push Web_Server::Iterator_Factory implementation.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef PUSH_ITERATOR_FACTORY_I_H
#define PUSH_ITERATOR_FACTORY_I_H

#include /**/ "ace/pre.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Push_Web_ServerS.h"
#include "ace/OS_NS_sys_stat.h"

#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implements the Web_Server::Iterator_Factory interface.
///
/// This class registers a client-side <Callback> object with the Content
/// Server, which is implemented by a <Callback_Handler> for each requested
/// file. register_callback() creates a <Callback_Handler> that pushes data
/// chunks asynchronously to the client. This allows the Content Server to
/// service multiple file requests concurrently without application-level
/// threads.
class Push_Iterator_Factory_i :
  virtual public POA_Web_Server::Iterator_Factory
{
public:
  Push_Iterator_Factory_i () = default;
  Push_Iterator_Factory_i (Push_Iterator_Factory_i const&) = delete;
  Push_Iterator_Factory_i (Push_Iterator_Factory_i&&) = delete;
  Push_Iterator_Factory_i& operator= (Push_Iterator_Factory_i const&) = delete;
  Push_Iterator_Factory_i& operator= (Push_Iterator_Factory_i&&) = delete;

  /**
   * This registration method passes a <Callback> that the Web server
   * uses to ``push'' data associated with <pathname> one ``chunk'' at
   * a time.  The <metadata> reports information about the <pathname>.
   */
  Web_Server::Metadata_Type *register_callback
    (char const *pathname,
     Web_Server::Callback_ptr client_callback) override;

  /// Set the file modification date in the metadata structure.
  int modification_date (ACE_stat const& file_status,
                         Web_Server::Metadata_Type &metadata);

  /// Set the type of file content in the metadata structure.
  int content_type (std::string const& filename,
                    Web_Server::Metadata_Type &metadata);
};

#include /**/ "ace/post.h"

#endif  /* PUSH_ITERATOR_FACTORY_I_H */

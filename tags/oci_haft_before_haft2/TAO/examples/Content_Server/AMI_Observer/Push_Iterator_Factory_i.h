// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     AMI_Observer
//
// = FILENAME
//     Push_Iterator_Factory_i.h
//
// = DESCRIPTION
//     Header file for the push Web_Server::Iterator_Factory implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef PUSH_ITERATOR_FACTORY_I_H
#define PUSH_ITERATOR_FACTORY_I_H

#include "ace/pre.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Push_Iterator_Factory_i :
  virtual public POA_Web_Server::Iterator_Factory
{
  // = TITLE
  //    Implement the Web_Server::Iterator_Factory interface.
  //
  // = DESCRIPTION
  //    This class is used to register a client-side <Callback> object
  //    with the Content Server, which is actually implemented by the
  //    <Callback_Handler> class for each requested file.
  //
  //    The client invokes the register_callback() method in this
  //    class.  That method then creates a <Callback_Handler> which
  //    will push chunks of data asynchronously to the client-side
  //    <Callback> object.  This allows the Content Server to
  //    concurrently service multiple file requests without the need
  //    to explicitly use threads at the application level.

public:
  Web_Server::Metadata_Type *register_callback
    (const char *pathname,
     Web_Server::Callback_ptr client_callback
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Web_Server::Error_Result));
  // This registration method passes a <Callback> that the Web server
  // uses to ``push'' data associated with <pathname> one ``chunk'' at
  // a time.  The <metadata> reports information about the <pathname>.

  int modification_date (ACE_stat *file_status,
                         Web_Server::Metadata_Type &metadata);
  // Set the file modification date in the metadata structure.

  int content_type (const char *filename,
                    Web_Server::Metadata_Type &metadata);
  // Set the type of file content in the metadata structure.

private:
  Web_Server::Callback_var client_callback_;
  // Reference to the object that is used by the server to push chunks
  // of data to the client.
};

#include "ace/post.h"

#endif  /* PUSH_ITERATOR_FACTORY_I_H */

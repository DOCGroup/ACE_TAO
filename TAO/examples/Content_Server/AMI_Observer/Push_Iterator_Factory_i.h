// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     Content_Server
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


// Implement the Web_Server::Iterator_Factory interface.
class Push_Iterator_Factory_i :
  virtual public POA_Web_Server::Iterator_Factory
{
public:

  // This registration method passes a <Callback> that
  // the Web server uses to ``push'' data associated with
  // <pathname> one ``chunk'' at a time.  The <metadata>
  // reports information about the <pathname>.
  Web_Server::Metadata_Type * register_callback (
      const char *pathname,
      Web_Server::Callback_ptr client_callback,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Web_Server::Error_Result));

  // Set the file modification date in the metadata structure.
  int modification_date (struct stat * file_status,
                         Web_Server::Metadata_Type & metadata);

  // Set the type of file content in the metadata structure.
  int content_type (const char * filename,
                    Web_Server::Metadata_Type & metadata);
private:

  Web_Server::Callback_var client_callback_;
  // Reference to the object that is used by the server to push chunks
  // of data to the client.

};

#include "ace/post.h"

#endif  /* PUSH_ITERATOR_FACTORY_I_H */

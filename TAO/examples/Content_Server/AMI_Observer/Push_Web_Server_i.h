// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     Content_Server
//
// = FILENAME
//     Push_Web_Server_i.h
//
// = DESCRIPTION
//     Header file for the CORBA push web server implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef PUSH_WEB_SERVER_I_H
#define PUSH_WEB_SERVER_I_H

#include "ace/pre.h"

#include "Push_Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations
struct stat;

// Implement the Web_Server interface.
class Push_Web_Server_i
{
public:
  // Download the <contents> associated with <pathname>.
  // The <metadata> reports information about the <contents>.
  virtual void get (const char *pathname,
                    Web_Server::Content_Type_out contents,
                    Web_Server::Metadata_Type_out metadata,
                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  int modification_date (struct stat * file_status,
                         Web_Server::Metadata_Type_out metadata);
  // Set the <modification_date_> field in the outgoing metadata.

  int content_type (const char *filename,
                    Web_Server::Metadata_Type_out metadata);
  // Set the <content_type_> field in the outgoing metadata.
};

// Implement the Web_Server::Content_Iterator interface.
class Content_Iterator_i : virtual public POA_Web_Server::Content_Iterator
{
public:
  // Constructor
  Content_Iterator_i (ACE_HANDLE handle);

  // Destructor
  ~Content_Iterator_i (void);

  // This operation returns the next <chunk> of the file starting at
  // <offset>.  If there are no more bindings, false is returned.
  virtual CORBA::Boolean next_chunk (CORBA::ULong offset,
                                     Chunk_Type_out chunk,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Destroy the iterator.
  virtual void destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_HANDLE handle_;
  // The file descriptor of the open file being iterated over.
};

// Implement the Web_Server::Iterator_Factory interface.
class Iterator_Factory_i : virtual public POA_Web_Server::Iterator_Factory
{
public:
  // This factory method returns a <Content_Iterator> that can be used
  // to read the <contents> associated with <pathname> one ``chunk''
  // at a time.  The <metadata> reports information about the
  // <contents>.
  virtual void get_iterator (const char * pathname,
                             Content_Iterator_out contents,
                             Metadata_Type_out metadata,,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     WebServer::Error_Result));

};

#include "ace/post.h"

#endif  /* PUSH_WEB_SERVER_I_H */

// -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     ECE255
//
// = FILENAME
//     Iterator_Factory_i.h
//
// = DESCRIPTION
//     Header file for the Web_Server::Iterator_Factory implementation.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef ITERATOR_FACTORY_I_H
#define ITERATOR_FACTORY_I_H

#include "Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// Forward declarations
struct stat;
class Content_Iterator_i;

// Implement the Web_Server::Iterator_Factory interface.
class Iterator_Factory_i : virtual public POA_Web_Server::Iterator_Factory
{
public:

  // This factory method returns a <Content_Iterator> that can be used
  // to read the <contents> associated with <pathname> one ``chunk''
  // at a time.  The <metadata> reports information about the
  // <contents>.
  virtual void get_iterator (const char *pathname,
                             Web_Server::Content_Iterator_out contents,
                             Web_Server::Metadata_Type_out metadata,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Web_Server::Error_Result));

  // Set the file modification date in the metadata structure.
  int modification_date (struct stat *file_status,
                         Web_Server::Metadata_Type_out metadata);

  // Set the type of file content in the metadata structure.
  int content_type (const char *filename,
                    Web_Server::Metadata_Type_out metadata);

};


#endif  /* ITERATOR_FACTORY_I_H */

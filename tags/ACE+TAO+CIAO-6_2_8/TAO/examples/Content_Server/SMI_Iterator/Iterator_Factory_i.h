// -*- C++ -*-


//=============================================================================
/**
 *  @file     Iterator_Factory_i.h
 *
 *  $Id$
 *
 *   Header file for the Web_Server::Iterator_Factory implementation.
 *
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef ITERATOR_FACTORY_I_H
#define ITERATOR_FACTORY_I_H

#include "Web_ServerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_NS_sys_stat.h"

// Forward declarations
class Content_Iterator_i;

/**
 * @class Iterator_Factory_i
 *
 * @brief Factory that creates a Content_Iterator that retrieves fixed
 * size chunks of data from Content server.
 *
 * This class creates a Content_Iterator that is set up to
 * iteratively download a specific file.  This same factory can
 * be used to create Content_Iterators for other files.
 */
class Iterator_Factory_i : virtual public POA_Web_Server::Iterator_Factory
{

public:

  /**
   * This factory method returns a <Content_Iterator> that can be used
   * to read the <contents> associated with <pathname> one ``chunk''
   * at a time.  The <metadata> reports information about the
   * <contents>.
   */
  virtual void get_iterator (const char *pathname,
                             Web_Server::Content_Iterator_out contents,
                             Web_Server::Metadata_Type_out metadata);

  /// Set the file modification date in the metadata structure.
  int modification_date (ACE_stat *file_status,
                         Web_Server::Metadata_Type_out metadata);

  /// Set the type of file content in the metadata structure.
  int content_type (const char *filename,
                    Web_Server::Metadata_Type_out metadata);

};


#endif  /* ITERATOR_FACTORY_I_H */

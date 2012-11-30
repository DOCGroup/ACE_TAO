// -*- C++ -*-

//=============================================================================
/**
 * @file  Storable_FlatFileStream.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@cs.wustl.edu>
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef STORABLE_FLATFILESTREAM_H
#define STORABLE_FLATFILESTREAM_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"

namespace TAO
{

  /**
   * @brief A Storable_Base derived class that works with a file stream.
   *
   * Based on Naming Server TAO_NS_FlatFileStream class.
   * @todo: Look into Having Naming Server code use this instead of
   * TAO_NS_FlatFileStream.
   */
  class TAO_Export Storable_FlatFileStream : public Storable_Base
  {
  public:

    Storable_FlatFileStream(const ACE_CString & file, const char * mode);
    virtual ~Storable_FlatFileStream();

    /// Remove a file by name (file is not open)
    virtual void remove();

    /// Check if a file exists on disk (file is not open)
    virtual int exists();

    /// Open a file (the remaining methods below all require an open file)
    virtual int open();

    /// Close an open file
    virtual int close();

    /// Acquire a file lock
    virtual int flock (int whence, int start, int len);

    /// Release a file lock
    virtual int funlock (int whence, int start, int len);

    /// Returns the last time an open file was changed
    virtual time_t last_changed(void);

    virtual Storable_Base& operator << (const ACE_CString& str);

    virtual Storable_Base& operator >> (ACE_CString& str);

  private:
    ACE_OS::ace_flock_t filelock_;
    FILE* fl_;
    ACE_CString file_;
    ACE_CString mode_;
  };

  class TAO_Export Storable_FlatFileFactory : public Storable_Factory
  {
  public:

	/// @param directory Directory to contain file passed in
	/// create_stream(). The directory is assumed to already exist.
    Storable_FlatFileFactory(const ACE_CString & directory);

    const ACE_CString & get_directory () const;

    ~Storable_FlatFileFactory();

  // Factory Methods

  /// Create the stream that can operate on a disk file
    virtual Storable_Base *create_stream(const ACE_CString & file,
                                         const ACE_TCHAR * mode);
  private:
  	ACE_CString directory_;

  };


}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#endif /* STORABLE_FLATFILESTREAM_H */

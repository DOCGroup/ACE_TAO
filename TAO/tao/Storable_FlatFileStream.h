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
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"
#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @brief A Storable_Base derived class that works with a file stream.
   *
   */
  class TAO_Export Storable_FlatFileStream : public Storable_Base
  {
  public:

    Storable_FlatFileStream(const ACE_CString & file, const char * mode,
                            bool use_backup =
                            Storable_Base::use_backup_default);

    virtual ~Storable_FlatFileStream();

    /// Check if a file exists on disk (file is not open)
    virtual int exists ();

    /// Open a file (the remaining methods below all require an open file)
    virtual int open ();

    /// Acquire a file lock
    virtual int close ();

    /// Acquire a file lock
    virtual int flock (int whence, int start, int len);

    /// Release a file lock
    virtual int funlock (int whence, int start, int len);

    /// Returns the last time an open file was changed
    virtual time_t last_changed (void);

    virtual void rewind (void);

    virtual bool flush (void);

    /// Force write of storable data to storage.
    /// Returns 0 on success, otherwise EOF
    virtual int sync (void);

    virtual Storable_Base& operator << (const ACE_CString& str);

    virtual Storable_Base& operator >> (ACE_CString& str);

    virtual Storable_Base& operator << (bool b);

    virtual Storable_Base& operator >> (bool &b);

    virtual Storable_Base& operator << (int i);

    virtual Storable_Base& operator >> (int &i);

    virtual Storable_Base& operator << (unsigned int i);

    virtual Storable_Base& operator >> (unsigned int &i);

    virtual Storable_Base& operator << (const TAO_OutputCDR & cdr);

    virtual size_t write (size_t size, const char * bytes);

    virtual size_t read (size_t size, char * bytes);

    virtual int restore_backup ();

  protected:

    virtual void do_remove ();

    virtual void remove_backup ();

    virtual int create_backup ();

  private:

    /// Throw a Storable_Read_Exception if the state
    /// is not good due to a read error.
    void throw_on_read_error (Storable_State state);

    /// Throw a Storable_Write_Exception if the state
    /// is not good due to a write error.
    void throw_on_write_error (Storable_State state);

    ACE_CString backup_file_name ();

    ACE_OS::ace_flock_t filelock_;
    FILE* fl_;
    ACE_CString file_;
    ACE_CString mode_;
  };

  class TAO_Export Storable_FlatFileFactory : public Storable_Factory
  {
  public:

    /// @param directory Directory to contain file passed in
    /// create_stream (). The directory is assumed to already exist.
    Storable_FlatFileFactory(const ACE_CString & directory);

    const ACE_CString & get_directory () const;

    ~Storable_FlatFileFactory ();

  // Factory Methods

  /// Create the stream that can operate on a disk file
    virtual Storable_Base *create_stream (const ACE_CString & file,
                                          const char * mode,
                                          bool use_backup =
                                          Storable_Base::use_backup_default);
  private:
    ACE_CString directory_;

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* STORABLE_FLATFILESTREAM_H */

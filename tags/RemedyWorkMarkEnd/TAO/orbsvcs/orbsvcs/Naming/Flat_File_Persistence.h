// -*- C++ -*-

//=============================================================================
/**
 *  @file   Flat_File_Persistence.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FLAT_FILE_PERSISTENCE_H
#define TAO_FLAT_FILE_PERSISTENCE_H

#include "orbsvcs/Naming/Storable.h"
#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//------------------------------------------------------------------------
// The Flat File concrete classes
//------------------------------------------------------------------------
class TAO_NS_FlatFileStream : public TAO_Storable_Base
{
public:

  TAO_NS_FlatFileStream(const ACE_CString & file, const char * mode);
  virtual ~TAO_NS_FlatFileStream();

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

  /// Write a header to disk
  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Header& header);

  /// Read a header from disk
  virtual TAO_Storable_Base& operator >> (
              TAO_NS_Persistence_Header& header);

  /// Write a record to disk
  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Record& record);

  /// Read a record from disk
  virtual TAO_Storable_Base& operator >> (
              TAO_NS_Persistence_Record& record);

  /// Write the global data to disk
  virtual TAO_Storable_Base& operator << (
              const TAO_NS_Persistence_Global& global);

  /// Read the global data from disk
  virtual TAO_Storable_Base& operator >> (
              TAO_NS_Persistence_Global& global);

private:
  ACE_OS::ace_flock_t filelock_;
  FILE* fl_;
  ACE_CString file_;
  ACE_CString mode_;
};

class TAO_NS_FlatFileFactory : public TAO_Naming_Service_Persistence_Factory
{
public:
  // Factory Methods

  /// Create the stream that can operate on a disk file
  virtual TAO_Storable_Base *create_stream(const ACE_CString & file,
                                           const ACE_TCHAR * mode);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

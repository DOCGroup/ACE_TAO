/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    JAWS_File.h
//
// = AUTHOR
//    James Hu
// 
// ============================================================================

#if !defined (JAWS_FILE_H)
#define JAWS_FILE_H

#include "ace/Mem_Map.h"
#include "ace/Synch.h"

// = Forward declarations.
class JAWS_File;
class JAWS_Virtual_Filesystem;

class JAWS_File_Handle
  // = TITLE
  //     Abstraction over a real file.  This is meant to be the entry
  //     point into the Cached Virtual Filesystem.  On creation, the
  //     cache is checked, and reference count is incremented.  On
  //     destruction, reference count is decremented.  If the
  //     reference count is 0, the file is removed from the cache.
  //
  //     E.g. 1,
  //       {
  //         JAWS_File_Handle foo("foo.html");
  //         this->peer ().send (foo.address (), foo.size ());
  //       }
  //
  //     E.g. 2,
  //       {
  //         JAWS_File_Handle foo("foo.html");
  //         io->transmitfile (foo.handle (), this->peer ().handle ());
  //       }
  //
  //     E.g. 3,
  //       {
  //         JAWS_File_Handle foo("foo.html", content_length);
  //         this->peer ().recv (foo.address (), content_length);
  //       }
{
public:

  JAWS_File_Handle (const char *filename);
  // Query cache for file, and acquire it assumes the file is being
  // opened for reading.

  JAWS_File_Handle (const char *filename,
		    int size);
  // Create new entry, and acquire it presence of SIZE assumes the
  // file is being opened for writing.

  ~JAWS_File_Handle (void);
  // Closes any open handles, release acquired file.

  void *address (void) const;
  // Base address of memory mapped file.

  ACE_HANDLE handle (void) const;
  // A handle (e.g., UNIX file descriptor, or NT file handle).

  int error (void) const;
  // Any associated error in handle creation and acquisition.

  size_t size (void) const;
  // The size of the file.

protected:

  JAWS_File_Handle (void);
  // Default do nothing constructor.  Prevent it from being called.

  void init (void);
  // James, please document this function.

private:
  // James, please document.
  JAWS_File *file_;

  ACE_HANDLE handle_;
};

// James, can you please replace this with ACE_Hash_Map_Manager?

class JAWS_Virtual_Filesystem
  // = TITLE
  //     A hash table holding the information about entry point into
  //     the Cached Virtual Filesystem.  On creation, the cache is
  //     checked, and reference count is incremented.  On destruction,
  //     reference count is decremented.  If the reference count is 0,
  //     the file is removed from the cache.
{
public:
  static JAWS_Virtual_Filesystem * instance (void);
  ~JAWS_Virtual_Filesystem (void);

  // James, please document these methods.
  JAWS_File *fetch (const char * filename);
  JAWS_File *remove (const char * filename);
  JAWS_File *insert (JAWS_File * new_file);
  JAWS_File *replace (JAWS_File * new_file);

protected:
  JAWS_Virtual_Filesystem (void);

private:

  // James, please document these methods.

  int fetch_i (const char * filename);
  JAWS_File * remove_i (int index);
  JAWS_File * insert_i (JAWS_File *new_file);

public:

  enum
  {
    DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE = 512,
    // For this stupid implementation, use an array.  Someday, use a
    // balanced search tree.

    DEFAULT_VIRTUAL_FILESYSTEM_CACHE_SIZE = 10
    // This determines the highwater mark in megabytes for the cache.
    // This will be ignored for now.
  };

private:

  JAWS_File *table_[DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE];
  int size_;

  static JAWS_Virtual_Filesystem *cvf_;
  static ACE_SYNCH_RW_MUTEX lock_;
};

class JAWS_File
  // = TITLE
  //     Abstraction over a real file.  This is what the Virtual
  //     Filesystem contains.  This class is not intended for general
  //     consumption.  Please consult a physician before attempting to
  //     use this class.
{
public:
  JAWS_File (const char *filename);
  // Creates a file for reading.

  JAWS_File (const char *filename,
	     int size);
  // Creates a file for writing.

  ~JAWS_File (void);
  // Only if reference count is zero should this be called.

  int acquire (void);
  // Increment the reference_count_.

  int release (void);
  // Decrement the reference_count_.

  // = action_ accessors
  int action (void) const;
  int action (int action_value);

  // = error_ accessors
  int error (void) const;
  int error (int error_value,
	     const char *s = "JAWS_File");

  const char *filename (void) const;
  // filename_ accessor

  ACE_HANDLE handle (void) const;
  // handle_ accessor.

  void *address (void) const;
  // Base memory address for memory mapped file.

  size_t size (void) const;
  // size_ accessor.

  int update (void) const;
  // True if file on disk is newer than cached file.

protected:
  JAWS_File (void);

  // James, please document...

  void init (void);

private:
  int error_i (int error_value, const char * s = "JAWS_File");

public:

  enum 
  { 
    IDLE = 0,
    READING = 1,
    WRITING = 2,
    WAITING = 4 
  };

  // = action status indicators

  // WAITING -- removed from Virtual Filesystem, but not deleted

  enum 
  {
    // James, please rename OKIE_DOKIE to SUCCESS or something boring.

    OKIE_DOKIE = 0,
    ACCESS_FAILED,
    OPEN_FAILED,
    COPY_FAILED,
    STAT_FAILED,
    MEMMAP_FAILED,
    WRITE_FAILED 
  };

private:
  //  James, please document these fields.
  char *tempname_;
  char filename_[MAXPATHLEN + 1];

  ACE_Mem_Map mmap_;
  ACE_HANDLE handle_;

  struct stat stat_;
  size_t size_;

  int action_;
  int error_;

  int reference_count_;
  ACE_SYNCH_RW_MUTEX lock_;
};

#endif /* JAWS_FILE_H */

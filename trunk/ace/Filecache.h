/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Filecache.h
//
// = AUTHOR
//    James Hu
// 
// ============================================================================

#if !defined (ACE_FILECACHE_H)
#define ACE_FILECACHE_H

#include "ace/Mem_Map.h"
#include "ace/Synch_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"

// = Forward declarations.
class ACE_Filecache_Object;
class ACE_Filecache;

class ACE_Export ACE_Filecache_Handle
  // = TITLE
  //     Abstraction over a real file.  This is meant to be the entry
  //     point into the Cached Virtual Filesystem.
  // 
  // = DESCRIPTION
  // This is a cached filesystem implementation based loosely on the
  // implementation of JAWS_File.  The interfaces will be nearly the
  // same.  The under-the-hood implementation should hopefully be a
  // much faster thing.
  //
  // These will be given their own implementations later. For now, we
  // borrow the implementation provided by JAWS.
  //
  // On creation, the cache is checked, and reference count is
  // incremented.  On destruction, reference count is decremented.  If
  // the reference count is 0, the file is removed from the cache.
  //
  //     E.g. 1,
  //       {
  //         ACE_Filecache_Handle foo("foo.html");
  //         this->peer ().send (foo.address (), foo.size ());
  //       }
  //
  //     E.g. 2,
  //       {
  //         ACE_Filecache_Handle foo("foo.html");
  //         io->transmitfile (foo.handle (), this->peer ().handle ());
  //       }
  //
  //     E.g. 3,
  //       {
  //         ACE_Filecache_Handle foo("foo.html", content_length);
  //         this->peer ().recv (foo.address (), content_length);
  //       }
  //
  // TODO:

  // (1) Get rid of the useless copying of files when reading.  Although
  // it does make sure the file you send isn't being changed, it doesn't
  // make sure the file is in a sensible state before sending it.

  // Alternative: if the file get's trashed while it is being shipped, let
  // the client request the file again.  The cache should have an updated
  // copy by that point.

  // (2) Use hashing for locating files.  This means I need a hastable
  // implementation with buckets.

  // (3) Only lock when absolutely necessary.  JAWS_Virtual_Filesystem was
  // rather conservative, but for some reason it still ran into problems.
  // Since this design should be simpler, problems should be easier to spot.
  //
{
public:

  ACE_Filecache_Handle (const char *filename);
  // Query cache for file, and acquire it.  Assumes the file is being
  // opened for reading.

  ACE_Filecache_Handle (const char *filename,
		    int size);
  // Create new entry, and acquire it.  Presence of SIZE assumes the
  // file is being opened for writing.

  ~ACE_Filecache_Handle (void);
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

  ACE_Filecache_Handle (void);
  // Default do nothing constructor.  Prevent it from being called.

  void init (void);
  // Common initializations for constructors.

public:

  // These come from ACE_Filecache_Object, which is an internal class.
  enum 
  {
    SUCCESS = 0,
    ACCESS_FAILED,
    OPEN_FAILED,
    COPY_FAILED,
    STAT_FAILED,
    MEMMAP_FAILED,
    WRITE_FAILED 
  };

private:
  ACE_Filecache_Object *file_;
  // A reference to the low level instance.

  ACE_HANDLE handle_;
  // A dup()'d version of the one from this->file_.
};

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
typedef ACE_Hash_Map_Entry<const char *, ACE_Filecache_Object *>
        ACE_Filecache_Hash_Entry;

typedef ACE_Hash_Map_Manager<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>
        ACE_Filecache_Hash;
#else
typedef ACE_Hash_Map_Entry<ACE_CString, ACE_Filecache_Object *>
        ACE_Filecache_Hash_Entry;

typedef ACE_Hash_Map_Manager<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>
        ACE_Filecache_Hash;
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

class ACE_Export ACE_Filecache
  // = TITLE
  //     A hash table holding the information about entry point into
  //     the Cached Virtual Filesystem. On insertion, the reference
  //     count is incremented. On destruction, reference count is
  //     decremented.
{
public:
  static ACE_Filecache *instance (void);
  // Singleton pattern.

  ~ACE_Filecache (void);

  int find (const char *filename);
  // Returns 0 if the file associated with ``filename'' is in the cache,
  // or -1 if not.

  ACE_Filecache_Object *fetch (const char *filename);
  // Return the file associated with ``filename'' if it is in the cache,
  // or create if not.

  ACE_Filecache_Object *remove (const char *filename);
  // Remove the file associated with ``filename'' from the cache.

  ACE_Filecache_Object *create (const char *filename, int size);
  // Create a new Filecache_Object, returns it.

  ACE_Filecache_Object *finish (ACE_Filecache_Object *&new_file);
  // Release an acquired Filecache_Object, returns it again or NULL if it
  // was deleted.

protected:
  ACE_Filecache_Object *insert_i (const char *filename, ACE_SYNCH_RW_MUTEX &);
  ACE_Filecache_Object *remove_i (const char *filename);
  ACE_Filecache_Object *update_i (const char *filename, ACE_SYNCH_RW_MUTEX &);

public:

  enum
  {
    DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE = 512,
    // For this stupid implementation, use an array.  Someday, use a
    // balanced search tree, or real hash table.

    DEFAULT_VIRTUAL_FILESYSTEM_CACHE_SIZE = 20
    // This determines the highwater mark in megabytes for the cache.
    // This will be ignored for now.
  };

protected:
  ACE_Filecache (void);
  // Prevent it from being called.

private:
  int size_;

  ACE_Filecache_Hash hash_;
  // The hash table

  static ACE_Filecache *cvf_;
  // The reference to the instance

  static ACE_SYNCH_RW_MUTEX lock_;
  static ACE_SYNCH_RW_MUTEX hash_lock_[DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE];
  static ACE_SYNCH_RW_MUTEX file_lock_[DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE];
  // Synchronization variables
};

#endif /* ACE_FILECACHE_H */

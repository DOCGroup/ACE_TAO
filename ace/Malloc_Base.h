/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Malloc_Base.h
//
// = AUTHOR
//    Doug Schmidt and Irfan Pyarali
//
// ============================================================================

#ifndef ACE_MALLOC_BASE_H
#define ACE_MALLOC_BASE_H

// The definition of this class is located in Malloc.cpp.

class ACE_Export ACE_Allocator
{
  // = TITLE
  //    Interface for a dynamic memory allocator that uses inheritance
  //    and dynamic binding to provide extensible mechanisms for
  //    allocating and deallocating memory.
public:
  // = Memory Management

  static ACE_Allocator *instance (void);
  // Get pointer to a default <ACE_Allocator>.

  static ACE_Allocator *instance (ACE_Allocator *);
  // Set pointer to a process-wide <ACE_Allocator> and return existing
  // pointer.

  static void close_singleton (void);
  // Delete the dynamically allocated Singleton

  ACE_Allocator (void);
  // "No-op" constructor (needed to make certain compilers happy).

  virtual ~ACE_Allocator (void);
  // Virtual destructor

  virtual void *malloc (size_t nbytes) = 0;
  // Allocate <nbytes>, but don't give them any initial value.

  virtual void *calloc (size_t nbytes, char initial_value = '\0') = 0;
  // Allocate <nbytes>, giving them <initial_value>.

  virtual void free (void *ptr) = 0;
  // Free <ptr> (must have been allocated by <ACE_Allocator::malloc>).

  virtual int remove (void) = 0;
  // Remove any resources associated with this memory manager.

  // = Map manager like functions

  virtual int bind (const char *name, void *pointer, int duplicates = 0) = 0;
  // Associate <name> with <pointer>.  If <duplicates> == 0 then do
  // not allow duplicate <name>/<pointer> associations, else if
  // <duplicates> != 0 then allow duplicate <name>/<pointer>
  // assocations.  Returns 0 if successfully binds (1) a previously
  // unbound <name> or (2) <duplicates> != 0, returns 1 if trying to
  // bind a previously bound <name> and <duplicates> == 0, else
  // returns -1 if a resource failure occurs.

  virtual int trybind (const char *name, void *&pointer) = 0;
  // Associate <name> with <pointer>.  Does not allow duplicate
  // <name>/<pointer> associations.  Returns 0 if successfully binds
  // (1) a previously unbound <name>, 1 if trying to bind a previously
  // bound <name>, or returns -1 if a resource failure occurs.  When
  // this call returns <pointer>'s value will always reference the
  // void * that <name> is associated with.  Thus, if the caller needs
  // to use <pointer> (e.g., to free it) a copy must be maintained by
  // the caller.

  virtual int find (const char *name, void *&pointer) = 0;
  // Locate <name> and pass out parameter via pointer.  If found,
  // return 0, Returns -1 if failure occurs.

  virtual int find (const char *name) = 0;
  // returns 0 if the name is in the mapping. -1, otherwise.

  virtual int unbind (const char *name) = 0;
  // Unbind (remove) the name from the map.  Don't return the pointer
  // to the caller

  virtual int unbind (const char *name, void *&pointer) = 0;
  // Break any association of name.  Returns the value of pointer in
  // case the caller needs to deallocate memory.

  // = Protection and "sync" (i.e., flushing memory to persistent
  // backing store).

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC) = 0;
  // Sync <len> bytes of the memory region to the backing store
  // starting at <this->base_addr_>.  If <len> == -1 then sync the
  // whole region.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC) = 0;
  // Sync <len> bytes of the memory region to the backing store
  // starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR) = 0;
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR) = 0;
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

#if defined (ACE_HAS_MALLOC_STATS)
  virtual void print_stats (void) const = 0;
  // Dump statistics of how malloc is behaving.
#endif /* ACE_HAS_MALLOC_STATS */

  virtual void dump (void) const = 0;
  // Dump the state of the object.
private:
  // DO NOT ADD ANY STATE (DATA MEMBERS) TO THIS CLASS!!!!
  // See the ACE_Allocator::instance () implementation for explanation.

  static ACE_Allocator *allocator_;
  // Pointer to a process-wide <ACE_Allocator> instance.

  static int delete_allocator_;
  // Must delete the <allocator_> if non-0.
};

#endif /* ACE_MALLOC_BASE_H */

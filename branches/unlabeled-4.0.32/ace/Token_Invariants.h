/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Token_Invariants
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//     Allows applications to test that invariants are always
//     satisfied.  Can test mutexes and readers/writer locks.  Does
//     not test recursive acquisition.
//
// ============================================================================

#if !defined (ACE_TOKEN_INVARIANTS_H)
#define ACE_TOKEN_INVARIANTS_H

#include "ace/Synch.h"
#include "ace/Map_Manager.h"
#include "ace/Local_Tokens.h"


class ACE_Mutex_Invariants
  // = TITLE
  //     Mutex Invariants
  //
  // = INVARIANTS
  //     1. Only one owner at a time.
{
public:
  ACE_Mutex_Invariants (void);
  // Default construction.

  int acquired (void);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  void releasing (void);
  // Updates internal database.

  // = Map_Manager operations.

  ACE_Mutex_Invariants (const ACE_Mutex_Invariants &rhs);
  // Copy construction.

  void operator= (const ACE_Mutex_Invariants &rhs);
  // Copy.

  void dump (void) const;
  // Dump the state of the class.

private:
  int owners_;
  // Number of owners.  This had better be 0 >= owners_ <= 1;
};

class ACE_RWLock_Invariants
  // = TITLE
  //     RWLock Invariants
  //
  // = INVARIANTS
  //     1. Only one writer at a time.
  //     2. If there is an owning writer, there are no owning readers.
{
public:
  ACE_RWLock_Invariants (void);
  // Default construction.

  int writer_acquired (void);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  int reader_acquired (void);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  void releasing (void);
  // Updates internal database.

  // = Map_Manager operations.

  ACE_RWLock_Invariants (const ACE_RWLock_Invariants &rhs);
  // Copy construction.

  void operator= (const ACE_RWLock_Invariants &rhs);
  // Copy.

  void dump (void) const;
  // Dump the state of the class.

private:
  int readers_;
  // Number of owning readers.

  int writers_;
  // Number of owning writers.
};

class ACE_Export ACE_Token_Invariant_Manager
  // = TITLE
  //     Token Invariants
  //
  // = DESCRIPTION
  //     The Token Invariant Manager allows applications to test that
  //     invariants are always satisfied.  Currently, Token_Invariants
  //     can test mutexes and readers/writer locks.  Does not test
  //     recursive acquisition.
  //
  //     Note that this class does not ever clean its database.  Until
  //     destroyed, it's size will forever increase.
{
public:

  static ACE_Token_Invariant_Manager *instance (void);
  // Singleton access point.

  // = Polymorphic methods.  Just pass in the proxy and the method
  // figures out the type of the token.

  int acquired (const ACE_Token_Proxy *proxy);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  void releasing (const ACE_Token_Proxy *proxy);
  // Updates internal database.

  // = Explicit methods.  These to not require actual proxies in order
  // to test a scenario.

  int mutex_acquired (const char *token_name);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  void mutex_releasing (const char *token_name);
  // Updates internal database.

  int reader_acquired (const char *token_name);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  int writer_acquired (const char *token_name);
  // Returns 1 on success, 0 when an invariant has been violated and
  // -1 on error.

  void rwlock_releasing (const char *token_name);
  // Updates internal database.

  void dump (void) const;
  // Dump the state of the class.

  // = The following two method should be in the protected part of the
  //   class.  Bugs with certain compilers preclude this. 
  ACE_Token_Invariant_Manager (void);
  // Prevent non-singleton construction.

  ~ACE_Token_Invariant_Manager (void);
  // Destruction.

protected:
  int get_mutex (const char *token_name, 
		 ACE_Mutex_Invariants *&inv);
  // Return or create.

  int get_rwlock (const char *token_name, 
		  ACE_RWLock_Invariants *&inv);
  // Return or create.

  ACE_TOKEN_CONST::MUTEX lock_;
  // ACE_Mutex_Token used to lock internal data structures.

  typedef ACE_Token_Name TOKEN_NAME;
  // This may be changed to a template type.

  typedef ACE_Map_Manager<TOKEN_NAME, ACE_Mutex_Invariants *, ACE_Null_Mutex>
    MUTEX_COLLECTION;
  // COLLECTION maintains a mapping from token names to mutexes.

  typedef ACE_Map_Iterator<TOKEN_NAME, ACE_Mutex_Invariants *, ACE_Null_Mutex>
    MUTEX_COLLECTION_ITERATOR;
  // Allows iterations through collection.

  typedef ACE_Map_Entry<TOKEN_NAME, ACE_Mutex_Invariants *>
    MUTEX_COLLECTION_ENTRY;
  // Allows iterations through collection.

  MUTEX_COLLECTION mutex_collection_;
  // MUTEX_COLLECTION maintains a mapping from token names to mutexes.

  typedef ACE_Map_Manager<TOKEN_NAME, ACE_RWLock_Invariants *, ACE_Null_Mutex>
    RWLOCK_COLLECTION;
  // COLLECTION maintains a mapping from token names to mutexes.

  typedef ACE_Map_Iterator<TOKEN_NAME, ACE_RWLock_Invariants *, ACE_Null_Mutex>
    RWLOCK_COLLECTION_ITERATOR;
  // Allows iterations through collection.

  typedef ACE_Map_Entry<TOKEN_NAME, ACE_RWLock_Invariants *>
    RWLOCK_COLLECTION_ENTRY;
  // Allows iterations through collection.

  RWLOCK_COLLECTION rwlock_collection_;
  // MUTEX_COLLECTION maintains a mapping from token names to mutexes.
};

#if defined (__ACE_INLINE__)
#include "ace/Token_Invariants.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TOKEN_INVARIANTS_H */

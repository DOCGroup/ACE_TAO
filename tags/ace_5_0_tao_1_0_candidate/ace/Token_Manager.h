/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Token_Manager.h
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// ============================================================================

#ifndef ACE_TOKEN_MANAGER_H
#define ACE_TOKEN_MANAGER_H

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Map_Manager.h"
#include "ace/Local_Tokens.h"

class ACE_Local_Mutex;
class ACE_Mutex_Token;

class ACE_Export ACE_Token_Manager : public ACE_Cleanup
{
  // = TITLE
  //   Manages all tokens in a process space.
  //
  // = DESCRIPTION
  //   Factory:  Proxies use the token manager to obtain token
  //   references.  This allows multiple proxies to reference the same
  //   logical token.
  //   Deadlock detection:  Tokens use the manager to check for
  //   deadlock situations during acquires.

  //   To add a new type of token (e.g. semaphore), do the following
  //   steps: 1. Create a new derivation of ACE_Token.  This class
  //   defines the semantics of the new Token.  2.  Create a
  //   derivation of ACE_Token_Manager.  You will only need to
  //   redefine make_mutex.
public:
  ACE_Token_Manager (void);
  virtual ~ACE_Token_Manager (void);

  // Set/get a pointer to token manager singleton.
  static ACE_Token_Manager *instance (void);
  void instance (ACE_Token_Manager *);

  void get_token (ACE_Token_Proxy *, const ASYS_TCHAR *token_name);
  // The Token manager uses ACE_Token_Proxy::token_id_ to look for
  // an existing token.  If none is found, the Token Manager calls
  // ACE_Token_Proxy::create_token to create a new one.  When
  // finished, sets ACE_Token_Proxy::token_.  <token_name> uniquely
  // id's the token name.

  int check_deadlock (ACE_Token_Proxy *proxy);
  int check_deadlock (ACE_Tokens *token, ACE_Token_Proxy *proxy);
  // returns 1 if the acquire will _not_ cause deadlock.
  // returns 0 if the acquire _will_ cause deadlock.
  // this method ignores recursive acquisition.  That is, it will not
  // report deadlock if the client holding the token requests the
  // token again.  Thus, it assumes recursive mutexes.

  void release_token (ACE_Tokens *&token);
  // notify the token manager that a token has been released.  If as a
  // result, there is no owner of the token, the token is deleted.

  ACE_TOKEN_CONST::MUTEX &mutex (void);
  // This is to allow Tokens to perform atomic transactions.  The
  // typical usage is to acquire this mutex, check for a safe_acquire,
  // perform some queueing (if need be) and then release the lock.
  // This is necessary since safe_acquire is implemented in terms of
  // the Token queues.

  void dump (void) const;
  // Dump the state of the class.

  void debug (int d);
  // Turn debug mode on/off.

private:
  int debug_;
  // Whether to print debug messages or not.

  static ACE_Token_Manager *token_manager_;
  // pointer to singleton token manager.

  ACE_Tokens *token_waiting_for (const ASYS_TCHAR *client_id);
  // return the token that the given client_id is waiting for, if any

  ACE_TOKEN_CONST::MUTEX lock_;
  // ACE_Mutex_Token used to lock internal data structures.

  typedef ACE_Token_Name TOKEN_NAME;
  // This may be changed to a template type.

  typedef ACE_Map_Manager<TOKEN_NAME, ACE_Tokens *, ACE_Null_Mutex>
  COLLECTION;
  // COLLECTION maintains a mapping from token names to ACE_Tokens*

  typedef ACE_Map_Iterator<TOKEN_NAME, ACE_Tokens *, ACE_Null_Mutex>
  COLLECTION_ITERATOR;
  // Allows iterations through collection_

  typedef ACE_Map_Entry<TOKEN_NAME, ACE_Tokens *>
  COLLECTION_ENTRY;
  // Allows iterations through collection_

  COLLECTION collection_;
  // COLLECTION maintains a mapping from token names to ACE_Tokens*.
};

#if defined (__ACE_INLINE__)
#include "ace/Token_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TOKEN_MANAGER_H */

/**
 *  @file    Token_Invariants.h
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 *
 *   Allows applications to test that invariants are always
 *   satisfied.  Can test mutexes and readers/writer locks.  Does
 *   not test recursive acquisition.
 */

#ifndef ACE_TOKEN_INVARIANTS_H
#define ACE_TOKEN_INVARIANTS_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_TOKENS_LIBRARY)

#include "ace/Map_Manager.h"
#include "ace/Local_Tokens.h"
#include "ace/Null_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Mutex_Invariants
 *
 * @brief Mutex Invariants
 * @par Invariants
 * 1. Only one owner at a time.
 */
class ACE_Export ACE_Mutex_Invariants
{
public:
  /// Default construction.
  ACE_Mutex_Invariants () = default;

  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int acquired ();

  /// Updates internal database.
  void releasing ();

  /// @name Copy operations
  /// @{
  /// Copy construction.
  ACE_Mutex_Invariants (ACE_Mutex_Invariants const &rhs) = default;

  /// Copy.
  ACE_Mutex_Invariants &operator= (ACE_Mutex_Invariants const &rhs) = default;
  /// @}

  /// Dump the state of the class.
  void dump () const;

private:
  /// Number of owners.  This had better be 0 >= owners_ <= 1;
  int owners_ {};
};

/**
 * @class ACE_RWLock_Invariants
 *
 * @brief RWLock Invariants
 *
 * Preserve the following invariants:
 * -# Only one writer at a time.
 * -# If there is an owning writer, there are no owning readers.
 */
class ACE_Export ACE_RWLock_Invariants
{
public:
  /// Default construction.
  ACE_RWLock_Invariants () = default;

  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int writer_acquired ();

  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int reader_acquired ();

  /// Updates internal database.
  void releasing ();

  /// @name Copy operations
  /// @{
  /// Copy construction.
  ACE_RWLock_Invariants (ACE_RWLock_Invariants const &rhs) = default;

  /// Copy.
  ACE_RWLock_Invariants &operator= (ACE_RWLock_Invariants const &rhs) = default;
  /// @}

  /// Dump the state of the class.
  void dump () const;

private:
  /// Number of owning writers.
  int writers_ {};

  /// Number of owning readers.
  int readers_ {};
};

/**
 * @class ACE_Token_Invariant_Manager
 *
 * @brief Token Invariants
 *
 * The Token Invariant Manager allows applications to test that
 * invariants are always satisfied.  Currently, Token_Invariants
 * can test mutexes and readers/writer locks.  Does not test
 * recursive acquisition.
 * @note This class does not ever clean its database.  Until
 * destroyed, it's size will forever increase.
 */
class ACE_Export ACE_Token_Invariant_Manager : public ACE_Cleanup
{
public:
  /// Singleton access point.
  static ACE_Token_Invariant_Manager *instance ();

  /// @name Proxy-based operations
  /// Just pass in the proxy; the operation determines the token type.
  /// @{
  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int acquired (ACE_Token_Proxy const *proxy);

  /// Updates internal database.
  void releasing (ACE_Token_Proxy const *proxy);
  /// @}

  /// @name Explicit operations
  /// These operations do not require proxies and allow testing a scenario.
  /// @{
  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int mutex_acquired (ACE_TCHAR const *token_name);

  /// Updates internal database.
  void mutex_releasing (ACE_TCHAR const *token_name);

  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int reader_acquired (ACE_TCHAR const *token_name);

  /// Returns 1 on success, 0 when an invariant has been violated and
  /// -1 on error.
  int writer_acquired (ACE_TCHAR const *token_name);

  /// Updates internal database.
  void rwlock_releasing (ACE_TCHAR const *token_name);
  /// @}

  /// Dump the state of the class.
  void dump () const;

  /// These members are public because of bugs in certain compilers, although
  /// they should be protected.
  /// Prevent non-singleton construction.
  ACE_Token_Invariant_Manager ();

  virtual ~ACE_Token_Invariant_Manager ();

protected:
  /// Return or create.
  int get_mutex (ACE_TCHAR const *token_name,
                 ACE_Mutex_Invariants *&inv);

  /// Return or create.
  int get_rwlock (ACE_TCHAR const *token_name,
                  ACE_RWLock_Invariants *&inv);

  /// ACE_Mutex_Token used to lock internal data structures.
  ACE_TOKEN_CONST::MUTEX lock_;

  /// This may be changed to a template type.
  using TOKEN_NAME = ACE_Token_Name;

  /// COLLECTION maintains a mapping from token names to mutexes.
  using MUTEX_COLLECTION =
    ACE_Map_Manager<TOKEN_NAME, ACE_Mutex_Invariants *, ACE_Null_Mutex>;

  /// MUTEX_COLLECTION maintains a mapping from token names to mutexes.
  MUTEX_COLLECTION mutex_collection_;

  /// COLLECTION maintains a mapping from token names to mutexes.
  using RWLOCK_COLLECTION =
    ACE_Map_Manager<TOKEN_NAME, ACE_RWLock_Invariants *, ACE_Null_Mutex>;

  /// MUTEX_COLLECTION maintains a mapping from token names to mutexes.
  RWLOCK_COLLECTION rwlock_collection_;

  /// Singleton pointer.
  static ACE_Token_Invariant_Manager *instance_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_TOKENS_LIBRARY */

#include /**/ "ace/post.h"
#endif /* ACE_TOKEN_INVARIANTS_H */

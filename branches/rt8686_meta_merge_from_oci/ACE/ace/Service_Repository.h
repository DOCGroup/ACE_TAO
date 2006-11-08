// -*- C++ -*-

//=============================================================================
/**
 *  @file    Service_Repository.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SERVICE_REPOSITORY_H
#define ACE_SERVICE_REPOSITORY_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Default_Constants.h"
#include "ace/Recursive_Thread_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Service_Type;
class ACE_DLL;

#define ACE_Component_Repository ACE_Service_Repository
/**
 * @class ACE_Service_Repository
 *
 * @brief Contains all the services offered by a Service
 * Configurator-based application.
 *
 * This class contains a vector of <ACE_Service_Types> *'s and
 * allows an administrative entity to centrally manage and
 * control the behavior of application services.  Note that if
 * services are removed from the middle of the repository the
 * order won't necessarily be maintained since the <remove>
 * method performs compaction.  However, the common case is not
 * to remove services, so typically they are deleted in the
 * reverse order that they were added originally.
 */
class ACE_Export ACE_Service_Repository
{
public:
  friend class ACE_Service_Repository_Iterator;

  enum
  {
    DEFAULT_SIZE = ACE_DEFAULT_SERVICE_REPOSITORY_SIZE
  };

  // = Initialization and termination methods.
  /// Initialize the repository.
  ACE_Service_Repository (void);

  /// Initialize the repository.
  ACE_Service_Repository (size_t size);

  /// Initialize the repository.
  int open (size_t size = DEFAULT_SIZE);

  /// Close down the repository and free up dynamically allocated
  /// resources.
  ~ACE_Service_Repository (void);

  /// Close down the repository and free up dynamically allocated
  /// resources.
  int close (void);

  /// Finalize all the services by calling <fini> and deleting
  /// dynamically allocated services.
  int fini (void);

  /// Get pointer to a process-wide ACE_Service_Repository.
  static ACE_Service_Repository * instance
    (size_t size = ACE_Service_Repository::DEFAULT_SIZE);

  /// Set pointer to a process-wide ACE_Service_Repository and return
  /// existing pointer.
  static ACE_Service_Repository *instance (ACE_Service_Repository *);

  /// Delete the dynamically allocated Singleton.
  static void close_singleton (void);

  // = Search structure operations (all acquire locks as necessary).

  /// Insert a new service record.  Returns -1 when the service repository
  /// is full and 0 on success.
  int insert (const ACE_Service_Type *);

  /**
   * Locate an entry with <name> in the table.  If <ignore_suspended>
   * is set then only consider services marked as resumed.  If the
   * caller wants the located entry, pass back a pointer to the
   * located entry via <srp>.  If <name> is not found, -1 is returned.
   * If <name> is found, but it is suspended and the caller wants to
   * ignore suspended services a -2 is returned.
   */
  int find (const ACE_TCHAR name[],
            const ACE_Service_Type **srp = 0,
            int ignore_suspended = 1) const;

  /// Remove an existing service record. If @a sr == 0, the service record
  /// is deleted before control is returned to the caller. If @a sr != 0,
  /// the service's record is removed from the repository, but not deleted;
  /// *sr receives the service record pointer and the caller is responsible
  /// for properly disposing of it.
  int remove (const ACE_TCHAR[], ACE_Service_Type **sr = 0);

  // = Liveness control
  /// Resume a service record.
  int resume (const ACE_TCHAR[], const ACE_Service_Type ** = 0);

  /// Suspend a service record.
  int suspend (const ACE_TCHAR[], const ACE_Service_Type ** = 0);

  /// Return the current size of the repository.
  size_t current_size (void) const;

  /// Return the total size of the repository.
  size_t total_size (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  friend class ACE_Service_Type_Dynamic_Guard;

  /// Remove an existing service record. It requires @a sr != 0, which
  /// receives the service record pointer and the caller is
  /// responsible for properly disposing of it.
  int remove_i (const ACE_TCHAR[], ACE_Service_Type **sr);

  /// Locates <service_name>.  Must be called without locks being
  /// held...

  int find_i (const ACE_TCHAR service_name[],
              const ACE_Service_Type ** = 0,
              int ignore_suspended = 1) const;

  /// @brief Relocate (static) services to another DLL.
  ///
  /// If any have been registered in the context of a "forward
  /// declaration" guard, those really aren't static services. Their
  /// code is in the DLL's code segment, or in one of the dependent
  /// DLLs. Therefore, such services need to be associated with the
  /// proper DLL in order to prevent failures upon finalization. The
  /// method locks the repo.
  ///
  /// Works by having the service type keep a reference to a specific
  /// DLL. No locking, caller makes sure calling it is safe. You can
  /// forcefully relocate any DLLs in the given range, not only the
  /// static ones - but that will cause Very Bad Things (tm) to happen.

  int relocate_i (size_t begin,
                  size_t end,
                  const ACE_DLL &adll,
                  bool static_only = true);

  /// Contains all the configured services.
  const ACE_Service_Type **service_vector_;

  /// Current number of services.
  size_t current_size_;

  /// Maximum number of services.
  size_t total_size_;

  /// Pointer to a process-wide ACE_Service_Repository.
  static ACE_Service_Repository *svc_rep_;

  /// Must delete the <svc_rep_> if non-0.
  static int delete_svc_rep_;

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  /// Synchronization variable for the MT_SAFE Repository
  mutable ACE_Recursive_Thread_Mutex lock_;
#endif /* ACE_MT_SAFE */
};

/**
 * @class ACE_Service_Repository_Iterator
 *
 * @brief Iterate through the ACE_Service_Repository.
 *
 * Make sure not to delete entries as the iteration is going on
 * since this class is not designed as a robust iterator.
 */
class ACE_Export ACE_Service_Repository_Iterator
{
public:
  // = Initialization and termination methods.
  /// Constructor initializes the iterator.
  ACE_Service_Repository_Iterator (ACE_Service_Repository &sr,
                                   int ignored_suspended = 1);

  /// Destructor.
  ~ACE_Service_Repository_Iterator (void);


public:
  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the repository.
  /// Returns 0 when all items have been seen, else 1.
  int next (const ACE_Service_Type *&next_item);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Move forward by one element in the repository.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  bool valid (void) const;

private:
  ACE_Service_Repository_Iterator (const ACE_Service_Repository_Iterator&);

  /// Reference to the Service Repository we are iterating over.
  ACE_Service_Repository &svc_rep_;

  /// Next index location that we haven't yet seen.
  size_t next_;

  /// Are we ignoring suspended services?
  int ignore_suspended_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Service_Repository.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _SERVICE_REPOSITORY_H */

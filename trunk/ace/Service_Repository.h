/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Service_Repository.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SERVICE_REPOSITORY_H)
#define ACE_SERVICE_REPOSITORY_H

#include "ace/Service_Record.h"

class ACE_Export ACE_Service_Repository
  // = TITLE
  //     Provide the abstract base class that supplies common server
  //     repository operations.
{
  friend class ACE_Service_Repository_Iterator;
public:
  enum {DEFAULT_SIZE = 50};
  // = Initialization and termination methods.
  ACE_Service_Repository (void);
  // Initialize the repository.

  ACE_Service_Repository (int size);
  // Initialize the repository.

  static ACE_Service_Repository *instance (int size = ACE_Service_Repository::DEFAULT_SIZE);
  // Get pointer to a process-wide <ACE_Service_Repository>.

  static ACE_Service_Repository *instance (ACE_Service_Repository *);
  // Set pointer to a process-wide <ACE_Service_Repository> and return
  // existing pointer.  

  static void close_singleton (void);
  // Delete the dynamically allocated Singleton

  int open (int size = DEFAULT_SIZE);
  // Initialize the repository.

  ~ACE_Service_Repository (void);
  // Terminate the repository.

  int close (void);
  // Terminate the repository.

  // = Search structure operations (all acquire locks as necessary). 

  int insert (const ACE_Service_Record *);
  // Insert a new service record.

  int find (const char[], 
	    const ACE_Service_Record ** = 0, 
	    int ignore_suspended = 1);
  // Locate an existing service record.

  int remove (const char[]);
  // Remove an existing service record.

  // = Liveness control
  int resume (const char[], const ACE_Service_Record ** = 0);
  // Resume a service record.

  int suspend (const char[], const ACE_Service_Record ** = 0);
  // Suspend a service record.

  int current_size (void);
  // Return the current size of the repository.

  int total_size (void);
  // Return the total size of the repository.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int find_i (const char service_name[],
	      const ACE_Service_Record ** = 0, 
	      int ignore_suspended = 1);
  // Locates <service_name>.  Must be called without locks being
  // held...

  const ACE_Service_Record **service_vector_;
  // Contains all the configured services.

  int current_size_;
  // Current number of services.

  int total_size_;
  // Maximum number of service.

  static ACE_Service_Repository *svc_rep_;
  // Pointer to a process-wide <ACE_Service_Repository>.

  static int delete_svc_rep_;
  // Must delete the <svc_rep_> if non-0.


#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  ACE_Thread_Mutex lock_; 
  // Synchronization variable for the MT_SAFE Repository 
#endif /* ACE_MT_SAFE */
};

class ACE_Export ACE_Service_Repository_Iterator
  // = TITLE
  //     Iterate through the <ACE_Service_Repository>.
{
public:
  // = Initialization method.
  ACE_Service_Repository_Iterator (ACE_Service_Repository &sr, 
				   int ignored_suspended = 1);

  // = Iteration methods.

  int next (const ACE_Service_Record *&next_item);
  // Pass back the <next_item> that hasn't been seen in the set.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Service_Repository &svc_rep_;
  // Reference to the Service Repository we are iterating over.

  int next_;
  // Next index location that we haven't yet seen.

  int ignore_suspended_;
  // Are we ignoring suspended services?
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Repository.i"
#endif /* __ACE_INLINE__ */

#endif /* _SERVICE_REPOSITORY_H */

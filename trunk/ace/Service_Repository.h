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
public:
  enum {DEFAULT_SIZE = 50};
  // = Initialization and termination methods.
  ACE_Service_Repository (void);
  // Initialize the repository.

  ACE_Service_Repository (int size);
  // Initialize the repository.

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

#if defined (ACE_MT_SAFE)
  ACE_Thread_Mutex lock_; 
  // Synchronization variable for the MT_SAFE Repository 
#endif /* ACE_MT_SAFE */

  friend class ACE_Service_Repository_Iterator;
};

class ACE_Export ACE_Service_Repository_Iterator
  // = TITLE
  //
  // = DESCRIPTION
  //
{
public:
  ACE_Service_Repository_Iterator (ACE_Service_Repository &sr, 
				   int ignored_suspended = 1);
  int next (const ACE_Service_Record *&so);
  int advance (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Service_Repository &svc_rep_;
  int next_;
  int ignore_suspended_;
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Repository.i"
#endif /* __ACE_INLINE__ */

#endif /* _SERVICE_REPOSITORY_H */

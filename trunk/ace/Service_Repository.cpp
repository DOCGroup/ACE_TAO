// Service_Repository.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Service_Repository.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Repository.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Repository)

void
ACE_Service_Repository::dump (void) const
{
  ACE_TRACE ("ACE_Service_Repository::dump");
}

ACE_Service_Repository::ACE_Service_Repository (void)
  : current_size_ (0), 
    total_size_ (0),
    service_vector_ (0)
{
  ACE_TRACE ("ACE_Service_Repository::ACE_Service_Repository");
}

// Initialize the Repository to a clean slate.

int 
ACE_Service_Repository::open (int size)
{
  ACE_TRACE ("ACE_Service_Repository::open");

  this->total_size_ = size;
  this->service_vector_ = 
    (const ACE_Service_Record **) new ACE_Service_Record *[size];
  if (this->service_vector_ == 0)
    {
      errno = ENOMEM;
      return -1;
    }

  return 0;
}

ACE_Service_Repository::ACE_Service_Repository (int size)
  : current_size_ (0)
{
  ACE_TRACE ("ACE_Service_Repository::ACE_Service_Repository");

  if (this->open (size) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Service_Repository"));
}

/* Close down all the services */

int
ACE_Service_Repository::close (void)
{
  ACE_TRACE ("ACE_Service_Repository::close");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->service_vector_ != 0)
    {
      for (int i = 0; i < this->current_size_; i++)
	{
	  ACE_DEBUG ((LM_DEBUG, "shutting down %s\n", 
		     this->service_vector_[i]->name ()));
	  delete (ACE_Service_Record *) this->service_vector_[i];
	}
      
      delete [] this->service_vector_;
      this->service_vector_ = 0;
      this->current_size_  = 0;
    }
  return 0;
}

ACE_Service_Repository::~ACE_Service_Repository (void)
{
  ACE_TRACE ("ACE_Service_Repository::~ACE_Service_Repository");
  this->close ();
}

// Locate an entry with NAME in the table.  If IGNORE_SUSPENDED is set
// then only consider services marked as resumed.  If the caller wants
// the located entry, pass back a pointer to the located entry via
// SRP.  If NAME is not found -1 is returned.  If NAME is found, but
// it is suspended and the caller wants to ignore suspended services a
// -2 is returned.  Must be called with locks held.

int 
ACE_Service_Repository::find_i (const char name[],
				const ACE_Service_Record **srp,
				int ignore_suspended)
{
  ACE_TRACE ("ACE_Service_Repository::find_i");
  int i;

  for (i = 0; i < this->current_size_; i++)
    if (ACE_OS::strcmp (name, this->service_vector_[i]->name ()) == 0)
      break;
  
  if (i < this->current_size_)
    {
      if (srp != 0)
	*srp = this->service_vector_[i];
      if (ignore_suspended && this->service_vector_[i]->active () == 0)
	return -2;
      return i;
    }
  else
    return -1;
}

int 
ACE_Service_Repository::find (const char name[],
			      const ACE_Service_Record **srp,
			      int ignore_suspended)
{
  ACE_TRACE ("ACE_Service_Repository::find");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  return this->find_i (name, srp, ignore_suspended);
}


// Insert the ACE_Service_Record SR into the repository.  Note that
// services may be inserted either resumed or suspended.

int 
ACE_Service_Repository::insert (const ACE_Service_Record *sr)
{
  ACE_TRACE ("ACE_Service_Repository::insert");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int i;

  for (i = 0; i < this->current_size_; i++)
    if (ACE_OS::strcmp (sr->name (), 
		  this->service_vector_[i]->name ()) == 0)
      break;

  if (i < this->current_size_) // Replacing an existing entry
    {
      // Check for self-assignment...
      if (sr == this->service_vector_[i]) 
	return 0;
      delete (ACE_Service_Record *) this->service_vector_[i];
      this->service_vector_[i] = sr;
      return 0;
    }
  else if (i < this->total_size_) // Adding a new entry.
    {
      this->service_vector_[i] = sr;
      this->current_size_++;
      return 0;
    }
  else
    return -1;
}
 
// Re-resume a service that was previously suspended. 

int
ACE_Service_Repository::resume (const char name[], 
				const ACE_Service_Record **srp)
{
  ACE_TRACE ("ACE_Service_Repository::resume");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  int i = this->find_i (name, srp, 0);

  if (i == -1)
    return -1;

  this->service_vector_[i]->resume ();
  return 0;
}

// Suspend a service so that it will not be considered active under
// most circumstances by other portions of the ACE_Service_Repository.

int
ACE_Service_Repository::suspend (const char name[], 
				 const ACE_Service_Record **srp)
{
  ACE_TRACE ("ACE_Service_Repository::suspend");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int i = this->find_i (name, srp, 0);

  if (i == -1)
    return -1;

  this->service_vector_[i]->suspend ();
  return 0;
}

// Completely remove a <name> entry from the Repository and
// dynamically unlink it if it was originally dynamically linked.
// Since the order of services in the Respository does not matter, we
// simply overwrite the entry being deleted with the final entry in
// the array and decrement the <service_count> by 1.

int 
ACE_Service_Repository::remove (const char name[])
{
  ACE_TRACE ("ACE_Service_Repository::remove");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int i = this->find_i (name, 0, 0);

  if (i == -1)
    return -1;
  else
    {
      const void *handle = this->service_vector_[i]->handle ();
      delete (ACE_Service_Record *) this->service_vector_[i];

      if (handle != 0)
	ACE_OS::dlclose ((void *) handle);

      if (--this->current_size_ > 1)
	this->service_vector_[i] 
	  = this->service_vector_[this->current_size_];
      return 0;
    }
}

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Repository_Iterator)

void
ACE_Service_Repository_Iterator::dump (void) const
{
  ACE_TRACE ("ACE_Service_Repository_Iterator::dump");
}

// Initializes the iterator and skips over any suspended entries at
// the beginning of the table, if necessary.  Note, you must not
// perform destructive operations on elements during this iteration...

ACE_Service_Repository_Iterator::ACE_Service_Repository_Iterator 
  (ACE_Service_Repository &sr, int ignr_suspended)
  : svc_rep_ (sr),
    next_ (-1),
    ignore_suspended_ (ignr_suspended)
{
  this->advance ();
}

// Obtains a pointer to the next valid service in the table.  If there
// are no more entries, returns 0, else 1.

int 
ACE_Service_Repository_Iterator::next (const ACE_Service_Record *&sr)
{
  ACE_TRACE ("ACE_Service_Repository_Iterator::next");
  if (this->next_ < this->svc_rep_.current_size_)
    {
      sr = this->svc_rep_.service_vector_[this->next_];
      return 1;
    }
  else
    return 0;
}

// Advance the iterator by the proper amount.  If we are ignoring
// suspended entries and the current entry is suspended, then we must
// skip over this entry.  Otherwise, we must advance the NEXT index to
// reference the next valid service entry.

int
ACE_Service_Repository_Iterator::advance (void)
{
  ACE_TRACE ("ACE_Service_Repository_Iterator::advance");
  for (++this->next_;
       this->next_ < this->svc_rep_.current_size_
       && this->ignore_suspended_
       && this->svc_rep_.service_vector_[this->next_]->active () == 0;
       this->next_++)
    continue;
  return this->next_;
}

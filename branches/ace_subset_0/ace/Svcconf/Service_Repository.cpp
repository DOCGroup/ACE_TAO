// Service_Repository.cpp
// $Id$

#include "ace/Svcconf/Service_Repository.h"
#include "ace/Utils/Object_Manager.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#if !defined (__ACE_INLINE__)
#include "ace/Svcconf/Service_Repository.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Service_Repository, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Repository)

// Process-wide Service Repository.
ACE_Service_Repository *ACE_Service_Repository::svc_rep_ = 0;

// Controls whether the Service_Repository is deleted when we shut
// down (we can only delete it safely if we created it)!
int ACE_Service_Repository::delete_svc_rep_ = 0;

void
ACE_Service_Repository::dump (void) const
{
  ACE_TRACE ("ACE_Service_Repository::dump");
}

ACE_Service_Repository::ACE_Service_Repository (void)
  : service_vector_ (0),
    current_size_ (0),
    total_size_ (0)
{
  ACE_TRACE ("ACE_Service_Repository::ACE_Service_Repository");
}

ACE_Service_Repository *
ACE_Service_Repository::instance (int size /* = ACE_Service_Repository::DEFAULT_SIZE */)
{
  ACE_TRACE ("ACE_Service_Repository::instance");

  if (ACE_Service_Repository::svc_rep_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));
      if (ACE_Service_Repository::svc_rep_ == 0)
        {
          if (ACE_Object_Manager::starting_up () ||
              !ACE_Object_Manager::shutting_down ())
            {
              ACE_NEW_RETURN (ACE_Service_Repository::svc_rep_,
                              ACE_Service_Repository (size),
                              0);
              ACE_Service_Repository::delete_svc_rep_ = 1;
            }
        }
    }

  return ACE_Service_Repository::svc_rep_;
}

ACE_Service_Repository *
ACE_Service_Repository::instance (ACE_Service_Repository *s)
{
  ACE_TRACE ("ACE_Service_Repository::instance");
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));

  ACE_Service_Repository *t = ACE_Service_Repository::svc_rep_;
  // We can't safely delete it since we don't know who created it!
  ACE_Service_Repository::delete_svc_rep_ = 0;

  ACE_Service_Repository::svc_rep_ = s;
  return t;
}

void
ACE_Service_Repository::close_singleton (void)
{
  ACE_TRACE ("ACE_Service_Repository::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (ACE_Service_Repository::delete_svc_rep_)
    {
      delete ACE_Service_Repository::svc_rep_;
      ACE_Service_Repository::svc_rep_ = 0;
      ACE_Service_Repository::delete_svc_rep_ = 0;
    }
}

// Initialize the Repository to a clean slate.

int
ACE_Service_Repository::open (int size)
{
  ACE_TRACE ("ACE_Service_Repository::open");

  ACE_Service_Type **temp;

  ACE_NEW_RETURN (temp,
                  ACE_Service_Type *[size],
                  -1);

  this->service_vector_ = ACE_const_cast (const ACE_Service_Type **,
                                          temp);
  this->total_size_ = size;
  return 0;
}

ACE_Service_Repository::ACE_Service_Repository (int size)
  : current_size_ (0)
{
  ACE_TRACE ("ACE_Service_Repository::ACE_Service_Repository");

  if (this->open (size) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Service_Repository")));
}

// Finalize (call <fini> and possibly delete) all the services.

int
ACE_Service_Repository::fini (void)
{
  ACE_TRACE ("ACE_Service_Repository::fini");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->service_vector_ != 0)
    {
      // <fini> the services in reverse order.  Note that if services
      // were removed from the middle of the repository the order
      // won't necessarily be maintained since the <remove> method
      // performs compaction.  However, the common case is not to
      // remove services, so typically they are deleted in reverse
      // order.

      for (int i = this->current_size_ - 1; i >= 0; i--)
        {
          if (ACE::debug ())
            ACE_DEBUG ((LM_DEBUG,
                        ACE_LIB_TEXT ("finalizing %s\n"),
                        this->service_vector_[i]->name ()));
          ACE_Service_Type *s =
            ACE_const_cast (ACE_Service_Type *,
                            this->service_vector_[i]);
          s->fini ();
        }
    }

  return 0;
}

// Close down all the services.

int
ACE_Service_Repository::close (void)
{
  ACE_TRACE ("ACE_Service_Repository::close");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->service_vector_ != 0)
    {
      // Delete services in reverse order.  Note that if services were
      // removed from the middle of the repository the order won't
      // necessarily be maintained since the <remove> method performs
      // compaction.  However, the common case is not to remove
      // services, so typically they are deleted in reverse order.

      for (int i = this->current_size_ - 1; i >= 0; i--)
        {
          ACE_Service_Type *s = ACE_const_cast (ACE_Service_Type *,
                                                this->service_vector_[i]);
          --this->current_size_;
          delete s;
        }

      delete [] this->service_vector_;
      this->service_vector_ = 0;
      this->current_size_ = 0;
    }

  return 0;
}

ACE_Service_Repository::~ACE_Service_Repository (void)
{
  ACE_TRACE ("ACE_Service_Repository::~ACE_Service_Repository");
  this->close ();
}

// Locate an entry with <name> in the table.  If <ignore_suspended> is
// set then only consider services marked as resumed.  If the caller
// wants the located entry, pass back a pointer to the located entry
// via <srp>.  If <name> is not found -1 is returned.  If <name> is
// found, but it is suspended and the caller wants to ignore suspended
// services a -2 is returned.  Must be called with locks held.

int
ACE_Service_Repository::find_i (const ACE_TCHAR name[],
                                const ACE_Service_Type **srp,
                                int ignore_suspended)
{
  ACE_TRACE ("ACE_Service_Repository::find_i");
  int i;

  for (i = 0; i < this->current_size_; i++)
    if (ACE_OS::strcmp (name,
                        this->service_vector_[i]->name ()) == 0)
      break;

  if (i < this->current_size_)
    {
      if (this->service_vector_[i]->fini_called ())
        {
          if (srp != 0)
            *srp = 0;
          return -1;
        }

      if (srp != 0)
        *srp = this->service_vector_[i];
      if (ignore_suspended
          && this->service_vector_[i]->active () == 0)
        return -2;
      return i;
    }
  else
    return -1;
}

int
ACE_Service_Repository::find (const ACE_TCHAR name[],
                              const ACE_Service_Type **srp,
                              int ignore_suspended)
{
  ACE_TRACE ("ACE_Service_Repository::find");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  return this->find_i (name, srp, ignore_suspended);
}


// Insert the ACE_Service_Type SR into the repository.  Note that
// services may be inserted either resumed or suspended.

int
ACE_Service_Repository::insert (const ACE_Service_Type *sr)
{
  ACE_TRACE ("ACE_Service_Repository::insert");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int i;

  // Check to see if this is a duplicate.
  for (i = 0; i < this->current_size_; i++)
    if (ACE_OS::strcmp (sr->name (),
                        this->service_vector_[i]->name ()) == 0)
      break;

  // Replacing an existing entry
  if (i < this->current_size_)
    {
      // Check for self-assignment...
      if (sr == this->service_vector_[i])
        return 0;
      ACE_Service_Type *s = ACE_const_cast (ACE_Service_Type *,
                                            this->service_vector_[i]);
      delete s;
      this->service_vector_[i] = sr;
      return 0;
    }
  // Adding a new entry.
  else if (i < this->total_size_)
    {
      this->service_vector_[i] = sr;
      this->current_size_++;
      return 0;
    }

  return -1;
}

// Re-resume a service that was previously suspended.

int
ACE_Service_Repository::resume (const ACE_TCHAR name[],
                                const ACE_Service_Type **srp)
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
ACE_Service_Repository::suspend (const ACE_TCHAR name[],
                                 const ACE_Service_Type **srp)
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
// the array and decrement the <current_size> by 1.

int
ACE_Service_Repository::remove (const ACE_TCHAR name[])
{
  ACE_TRACE ("ACE_Service_Repository::remove");
  ACE_Service_Type *s = 0;
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
    int i = this->find_i (name, 0, 0);

    if (i == -1)
      return -1;

    s = ACE_const_cast (ACE_Service_Type *,
                        this->service_vector_[i]);
    --this->current_size_;

    if (this->current_size_ >= 1)
      this->service_vector_[i]
        = this->service_vector_[this->current_size_];
  }
  delete s;
  return 0;
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
  (ACE_Service_Repository &sr,
   int ignr_suspended)
  : svc_rep_ (sr),
    next_ (-1),
    ignore_suspended_ (ignr_suspended)
{
  this->advance ();
}

// Obtains a pointer to the next valid service in the table.  If there
// are no more entries, returns 0, else 1.

int
ACE_Service_Repository_Iterator::next (const ACE_Service_Type *&sr)
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

int
ACE_Service_Repository_Iterator::done (void) const
{
  ACE_TRACE ("ACE_Service_Repository_Iterator::done");

  return this->next_ >= this->svc_rep_.current_size_;
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

  return this->next_ < this->svc_rep_.current_size_;
}

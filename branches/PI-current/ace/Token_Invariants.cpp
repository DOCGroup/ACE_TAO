// Token_Invariants.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Token_Invariants.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Token_Invariants.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Token_Invariants, "$Id$")

ACE_Token_Invariant_Manager *ACE_Token_Invariant_Manager::instance_ = 0;

ACE_Token_Invariant_Manager *
ACE_Token_Invariant_Manager::instance (void)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::instance");

  // Perform the Double-Check pattern...
  if (instance_ == 0)
    {
      ACE_MT (ACE_TOKEN_CONST::MUTEX *lock =
        ACE_Managed_Object<ACE_TOKEN_CONST::MUTEX>::get_preallocated_object
          (ACE_Object_Manager::ACE_TOKEN_INVARIANTS_CREATION_LOCK);
        ACE_GUARD_RETURN (ACE_TOKEN_CONST::MUTEX, ace_mon, *lock, 0));

      if (instance_ == 0)
        {
          ACE_NEW_RETURN (instance_, ACE_Token_Invariant_Manager, 0);

          // Register for destruction with ACE_Object_Manager.
          ACE_Object_Manager::at_exit (instance_);
        }
    }

  return instance_;
}

ACE_Token_Invariant_Manager::ACE_Token_Invariant_Manager (void)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::ACE_Token_Invariant_Manager");
}

int
ACE_Token_Invariant_Manager::mutex_acquired (const char *token_name)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::mutex_acquired");

  ACE_GUARD_RETURN (ACE_TOKEN_CONST::MUTEX, ace_mon, this->lock_, -1);

  ACE_Mutex_Invariants *inv = 0;
  if (this->get_mutex (token_name, inv) == -1)
    return -1;
  else
    return inv->acquired ();
}

int
ACE_Token_Invariant_Manager::acquired (const ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::acquired");

  // Reach into the proxy to find the token type.
  if (proxy->token_->type () == ACE_Tokens::MUTEX)
    return this->mutex_acquired (proxy->name ());
  else // ACE_Tokens::RWLOCK.
    {
      if (proxy->type () == ACE_RW_Token::READER)
        return this->reader_acquired (proxy->name ());
      else // ACE_RW_Token::WRITER.
        return this->writer_acquired (proxy->name ());
    }
}

void
ACE_Token_Invariant_Manager::releasing (const ACE_Token_Proxy *proxy)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::releasing");

  // Reach into the proxy to find the token type.
  if (proxy->token_->type () == ACE_Tokens::MUTEX)
    this->mutex_releasing (proxy->name ());
  else // ACE_Tokens::RWLOCK.
    this->rwlock_releasing (proxy->name ());
}

void
ACE_Token_Invariant_Manager::mutex_releasing (const char *token_name)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::mutex_releasing");
  ACE_GUARD (ACE_TOKEN_CONST::MUTEX, ace_mon, this->lock_);

  ACE_Mutex_Invariants *inv = 0;
  if (this->get_mutex (token_name, inv) == 0)
    inv->releasing ();
}

int
ACE_Token_Invariant_Manager::reader_acquired (const char *token_name)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::reader_acquired");
  ACE_GUARD_RETURN (ACE_TOKEN_CONST::MUTEX, ace_mon, this->lock_, -1);

  ACE_RWLock_Invariants *inv = 0;
  if (this->get_rwlock (token_name, inv) == -1)
    return -1;
  else
    return inv->reader_acquired ();
}

int
ACE_Token_Invariant_Manager::writer_acquired (const char *token_name)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::writer_acquired");

  ACE_GUARD_RETURN (ACE_TOKEN_CONST::MUTEX, ace_mon, this->lock_, -1);

  ACE_RWLock_Invariants *inv = 0;
  if (this->get_rwlock (token_name, inv) == -1)
    return -1;
  else
   return inv->writer_acquired ();
}

void
ACE_Token_Invariant_Manager::rwlock_releasing (const char *token_name)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::rwlock_releasing");

  ACE_GUARD (ACE_TOKEN_CONST::MUTEX, ace_mon, this->lock_);

  ACE_RWLock_Invariants *inv = 0;
  if (this->get_rwlock (token_name, inv) == 0)
    inv->releasing ();
}

void
ACE_Token_Invariant_Manager::dump (void) const
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("mutex_collection_:\n")));
  mutex_collection_.dump ();
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("rwlock_collection_:\n")));
  rwlock_collection_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}


int
ACE_Token_Invariant_Manager::get_mutex (const char *token_name,
                                        ACE_Mutex_Invariants *&inv)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::get_mutex");
  TOKEN_NAME name (token_name);
  if (mutex_collection_.find (name, inv) == -1)
    // We did not find one in the collection.
    {
      ACE_Mutex_Invariants *new_invariant;

      ACE_NEW_RETURN (new_invariant, ACE_Mutex_Invariants, -1);

      if (mutex_collection_.bind (name, new_invariant) == -1)
        {
          delete new_invariant;
          return -1;
        }

      if (mutex_collection_.find (name, inv) == -1)
        // We did not find one in the collection.
        return -1;
    }

  return 0;
}

int
ACE_Token_Invariant_Manager::get_rwlock (const char *token_name,
                                         ACE_RWLock_Invariants *&inv)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::get_rwlock");
  TOKEN_NAME name (token_name);
  if (rwlock_collection_.find (name, inv) == -1)
    // We did not find one in the collection.
    {
      ACE_RWLock_Invariants *new_invariant;

      ACE_NEW_RETURN (new_invariant, ACE_RWLock_Invariants, -1);

      if (rwlock_collection_.bind (name, new_invariant) == -1)
        return -1;

      if (rwlock_collection_.find (name, inv) == -1)
        // We did not find one in the collection.
        return -1;
    }

  return 0;
}


ACE_Token_Invariant_Manager::~ACE_Token_Invariant_Manager (void)
{
  ACE_TRACE ("ACE_Token_Invariant_Manager::~ACE_Token_Invariant_Manager");
  MUTEX_COLLECTION_ITERATOR iterator (mutex_collection_);

  for (MUTEX_COLLECTION_ENTRY *temp = 0;
       iterator.next (temp) != 0;
       iterator.advance ())
    delete temp->int_id_;

  RWLOCK_COLLECTION_ITERATOR iterator2 (rwlock_collection_);

  for (RWLOCK_COLLECTION_ENTRY *temp2 = 0;
       iterator2.next (temp2) != 0;
       iterator2.advance ())
    delete temp2->int_id_;
}

// **************************************************
// **************************************************
// **************************************************

ACE_Mutex_Invariants::ACE_Mutex_Invariants (void)
: owners_ (0)
{
}

int
ACE_Mutex_Invariants::acquired (void)
{
  if (++owners_ > 1)
    {
      owners_ = 42;
      return 0;
    }
  else
    return 1;
}

void
ACE_Mutex_Invariants::releasing (void)
{
  if (owners_ == 1)
    --owners_;
}

ACE_Mutex_Invariants::ACE_Mutex_Invariants (const ACE_Mutex_Invariants &rhs)
: owners_ (rhs.owners_)
{
}

void
ACE_Mutex_Invariants::operator= (const ACE_Mutex_Invariants &rhs)
{
  owners_ = rhs.owners_;
}

void
ACE_Mutex_Invariants::dump (void) const
{
  ACE_TRACE ("ACE_Mutex_Invariants::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("owners_ = %d\n"), owners_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// **************************************************
// **************************************************
// **************************************************

ACE_RWLock_Invariants::ACE_RWLock_Invariants (void)
: writers_ (0),
  readers_ (0)
{
}

int
ACE_RWLock_Invariants::writer_acquired (void)
{
  if (readers_ > 0)
    {
      writers_ = readers_ = 42;
      return 0;
    }
  else if (++writers_ > 1)
    {
      writers_ = readers_ = 42;
      return 0;
    }
  else
    return 1;
}

int
ACE_RWLock_Invariants::reader_acquired (void)
{
  if (writers_ > 0)
    {
      writers_ = readers_ = 42;
      return 0;
    }
  else
    {
      ++readers_;
      return 1;
    }
}

void
ACE_RWLock_Invariants::releasing (void)
{
  if (writers_ == 1)
    writers_ = 0;
  else if (readers_ > 0)
    --readers_;
}

ACE_RWLock_Invariants::ACE_RWLock_Invariants (const ACE_RWLock_Invariants &rhs)
: writers_ (rhs.writers_),
  readers_ (rhs.readers_)
{
}

void
ACE_RWLock_Invariants::operator= (const ACE_RWLock_Invariants &rhs)
{
  writers_ = rhs.writers_;
  readers_ = rhs.readers_;
}

void
ACE_RWLock_Invariants::dump (void) const
{
  ACE_TRACE ("ACE_RWLock_Invariants::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("writers_ = %d\n"),
              "readers_ = %d\n",
              writers_, readers_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Map_Manager<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Token_Name, ACE_Mutex_Invariants *>;
template class ACE_Map_Manager<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Token_Name, ACE_RWLock_Invariants *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Map_Manager<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<ACE_Token_Name, ACE_Mutex_Invariants *>
#pragma instantiate ACE_Map_Manager<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<ACE_Token_Name, ACE_RWLock_Invariants *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

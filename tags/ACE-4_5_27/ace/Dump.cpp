// Dump.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Synch_T.h"
#include "ace/Dump.h"
#include "ace/Object_Manager.h"

ACE_RCSID(ace, Dump, "$Id$")

// Implementations (very simple for now...)

ACE_Dumpable::~ACE_Dumpable (void)
{
  ACE_TRACE ("ACE_Dumpable::~ACE_Dumpable");
}

ACE_Dumpable::ACE_Dumpable (const void *this_ptr)
  : this_ (this_ptr)
{
  ACE_TRACE ("ACE_Dumpable::ACE_Dumpable");
}

ACE_Dumpable_Ptr::ACE_Dumpable_Ptr (const ACE_Dumpable *dumper)
  : dumper_ (dumper)
{
  ACE_TRACE ("ACE_Dumpable_Ptr::ACE_Dumpable_Ptr");
}

const ACE_Dumpable *
ACE_Dumpable_Ptr::operator->() const
{
  ACE_TRACE ("ACE_Dumpable_Ptr::operator->");
  return this->dumper_;
}

void
ACE_Dumpable_Ptr::operator= (const ACE_Dumpable *dumper) const
{
  ACE_TRACE ("ACE_Dumpable_Ptr::operator=");
  if (this->dumper_ != dumper)
    {
      delete (ACE_Dumpable *) this->dumper_;
      ((ACE_Dumpable_Ptr *) this)->dumper_ = dumper;
    }
}

ACE_ODB::ACE_ODB (void)
{
  ACE_TRACE ("ACE_ODB::ACE_ODB");
}

ACE_ODB *
ACE_ODB::instance (void)
{
  ACE_TRACE ("ACE_ODB::instance");

  if (ACE_ODB::instance_ == 0)
    {
      ACE_MT (ACE_Thread_Mutex *lock =
        ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
          (ACE_Object_Manager::ACE_DUMP_LOCK);
        ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock, 0));

      if (ACE_ODB::instance_ == 0)
        ACE_NEW_RETURN (ACE_ODB::instance_, ACE_ODB, 0);
    }

  return ACE_ODB::instance_;
}

void
ACE_ODB::dump_objects (void)
{
  ACE_TRACE ("ACE_ODB::dump_objects");
  for (int i = 0; i < this->current_size_; i++)
    {
      if (this->object_table_[i].this_ != 0)
        // Dump the state of the object.
        this->object_table_[i].dumper_->dump ();
    }
}

// This method registers a new <dumper>.  It detects
// duplicates and simply overwrites them.

void
ACE_ODB::register_object (const ACE_Dumpable *dumper)
{
  ACE_TRACE ("ACE_ODB::register_object");
  int i;
  int slot = 0;

  for (i = 0; i < this->current_size_; i++)
    {
      if (this->object_table_[i].this_ == 0)
        slot = i;
      else if (this->object_table_[i].this_ == dumper->this_)
        {
          slot = i;
          break;
        }
    }

  if (i == this->current_size_)
    {
      slot = this->current_size_++;
      ACE_ASSERT (this->current_size_ < ACE_ODB::MAX_TABLE_SIZE);
    }
  this->object_table_[slot].this_ = dumper->this_;
  this->object_table_[slot].dumper_ = dumper;
}

void
ACE_ODB::remove_object (const void *this_ptr)
{
  ACE_TRACE ("ACE_ODB::remove_object");
  int i;

  for (i = 0; i < this->current_size_; i++)
    {
      if (this->object_table_[i].this_ == this_ptr)
        break;
    }

  if (i < this->current_size_)
    {
      this->object_table_[i].this_ = 0;
      this->object_table_[i].dumper_ = 0;
    }
}

ACE_ODB *ACE_ODB::instance_ = 0;

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
ACE_MT (template class ACE_Guard<ACE_Thread_Mutex>);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#pragma instantiate ACE_Guard<ACE_Thread_Mutex>
#endif /* ACE_MT_SAFE */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


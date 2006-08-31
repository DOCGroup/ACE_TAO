// $Id$

#include "cuts/Event_Guard.h"

#if !defined (__CUTS_INLINE__)
# include "cuts/Event_Guard.inl"
#endif

#include "cuts/Counter.h"
#include "ace/Guard_T.h"

//
// CUTS_Event_Guard
//
CUTS_Event_Guard::CUTS_Event_Guard (void)
{

}

//
// CUTS_Event_Guard
//
CUTS_Event_Guard::~CUTS_Event_Guard (void)
{
  for ( CUTS_Counter_Map::iterator iter = this->counter_.begin ();
        iter != this->counter_.end ();
        iter ++)
  {
    delete iter->second;
  }
}

//
// insert_counter
//
CUTS_Bounded_Counter * CUTS_Event_Guard::register_event (long id, size_t count)
{
  // We are updating the structure of the <counter_> map so
  // we need to get writers access.
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex, guard, this->mutex_, false);

  // Attempt to insert the new counter ID.
  std::pair <CUTS_Counter_Map::iterator, bool> result =
    this->counter_.insert (CUTS_Counter_Map::value_type (id, 0));

  if (result.second)
  {
    /// Since we have add a new ID to <counter_> map we have
    /// to creat a new CUTS_Counter for the ID.
    result.first->second = new CUTS_Bounded_Counter (count);
  }

  return result.first->second;
}

//
// remove_counter
//
void CUTS_Event_Guard::unregister_event (long id)
{
  // We are updating the structure of the <counter_> map so
  // we need to get writers access.
  ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->mutex_);

  // Locate the ID in the <counter_> map.
  CUTS_Counter_Map::iterator iter = this->counter_.find (id);

  if (iter != this->counter_.end ())
  {
    // Delete the <CUTS_Counter> and remove the element
    // from the <counter_> map.
    delete iter->second;
    this->counter_.erase (iter);
  }
}

//
// enabled
//
bool CUTS_Event_Guard::enabled (void)
{
  // Since we are not updating the internal structure read
  // access will do just fine.
  ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex, guard, this->mutex_, false);

  // Since we are not <enabled_> we need to determine if we are
  // <enabled_>.
  size_t enabled_count = 0;

  // We need to count the number of enabled <CUTS_Counter> objects.
  for ( CUTS_Counter_Map::const_iterator iter = this->counter_.begin ();
        iter != this->counter_.end ();
        iter ++)
  {
    if (iter->second->is_full ())
      enabled_count ++;
  }

  return enabled_count == this->counter_.size ();
}

//
// reset
//
void CUTS_Event_Guard::reset (void)
{
  // Since we are not updating the internal structure read
  // access will do just fine.
  ACE_READ_GUARD (ACE_RW_Thread_Mutex, guard, this->mutex_);

  // Reset all the counter in the <counter_> map.
  for ( CUTS_Counter_Map::iterator iter = this->counter_.begin ();
        iter != this->counter_.end ();
        iter ++)
  {
    iter->second->reset ();
  }
}

//
// handle_event
//
void CUTS_Event_Guard::handle_event (long id)
{
  ACE_READ_GUARD (ACE_RW_Thread_Mutex, guard, this->mutex_);

  // Get the element from the map with the specified <id>.
  CUTS_Counter_Map::iterator iter = this->counter_.find (id);

  // Increment the event counter for the event.
  if (iter != this->counter_.end ())
    iter->second->increment ();
}

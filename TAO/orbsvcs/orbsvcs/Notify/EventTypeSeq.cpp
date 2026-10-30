#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/EventTypeSeq.h"
#include "orbsvcs/Notify/Topology_Saver.h"

#include "tao/debug.h"

#include <algorithm>
#include <new>
//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_EventTypeSeq::TAO_Notify_EventTypeSeq ()
{
}

TAO_Notify_EventTypeSeq::TAO_Notify_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq)
{
  this->insert_seq (event_type_seq);
}

TAO_Notify_EventTypeSeq &
TAO_Notify_EventTypeSeq::operator = (const TAO_Notify_EventTypeSeq & rhs)
{
  this->event_types_ = rhs.event_types_;
  return *this;
}

TAO_Notify_EventTypeSeq::TAO_Notify_EventTypeSeq (const TAO_Notify_EventTypeSeq & rhs)
  : TAO_Notify_Object ()
  , TAO_Notify::Topology_Savable ()
  , TAO_Notify::Topology_Object ()
  , event_types_ (rhs.event_types_)
{
}

TAO_Notify_EventTypeSeq::const_iterator
TAO_Notify_EventTypeSeq::begin () const
{
  return this->event_types_.begin ();
}

TAO_Notify_EventTypeSeq::const_iterator
TAO_Notify_EventTypeSeq::end () const
{
  return this->event_types_.end ();
}

size_t
TAO_Notify_EventTypeSeq::size () const
{
  return this->event_types_.size ();
}

bool
TAO_Notify_EventTypeSeq::empty () const
{
  return this->event_types_.empty ();
}

void
TAO_Notify_EventTypeSeq::clear ()
{
  this->event_types_.clear ();
}

bool
TAO_Notify_EventTypeSeq::contains (TAO_Notify_EventType const & event_type) const
{
  return std::find (this->event_types_.begin (), this->event_types_.end (), event_type)
    != this->event_types_.end ();
}

int
TAO_Notify_EventTypeSeq::add (TAO_Notify_EventType const & event_type)
{
  if (this->contains (event_type))
    return 1;

  try
    {
      this->event_types_.push_back (event_type);
    }
  catch (std::bad_alloc const &)
    {
      return -1;
    }

  return 0;
}

int
TAO_Notify_EventTypeSeq::erase (TAO_Notify_EventType const & event_type)
{
  Storage::iterator const position =
    std::find (this->event_types_.begin (), this->event_types_.end (), event_type);
  if (position == this->event_types_.end ())
    return -1;

  this->event_types_.erase (position);
  return 0;
}

void
TAO_Notify_EventTypeSeq::populate (CosNotification::EventTypeSeq& event_type_seq) const
{
  event_type_seq.length (static_cast<CORBA::ULong> (this->size ()));

  CORBA::ULong i = 0;
  for (TAO_Notify_EventType const & event_type : this->event_types_)
    event_type_seq[i++] = event_type.native ();
}

void
TAO_Notify_EventTypeSeq::populate_no_special (CosNotification::EventTypeSeq& event_type_seq) const
{
  // If the special exists in us, don't include it.
  const TAO_Notify_EventType& special = TAO_Notify_EventType::special ();

  if (this->contains (special))
    {
      event_type_seq.length (static_cast<CORBA::ULong> (this->size () - 1));
    }
  else
      event_type_seq.length (static_cast<CORBA::ULong> (this->size ()));

  CORBA::ULong i = 0;
  for (TAO_Notify_EventType const & event_type : this->event_types_)
    {
      if (event_type.is_special () == 0) // if its not the special event type.
        event_type_seq[i++] = event_type.native ();
    }
}

void
TAO_Notify_EventTypeSeq::insert_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      this->add (event_type);
    }
}

void
TAO_Notify_EventTypeSeq::remove_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      this->erase (event_type);
    }
}

void
TAO_Notify_EventTypeSeq::insert_seq (const TAO_Notify_EventTypeSeq& event_type_seq)
{
  for (TAO_Notify_EventType const & event_type : event_type_seq)
    this->add (event_type);
}

void
TAO_Notify_EventTypeSeq::remove_seq (const TAO_Notify_EventTypeSeq& event_type_seq)
{
  if (&event_type_seq == this)
    {
      this->clear ();
      return;
    }

  for (TAO_Notify_EventType const & event_type : event_type_seq)
    this->erase (event_type);
}

void
TAO_Notify_EventTypeSeq::add_and_remove (TAO_Notify_EventTypeSeq& seq_added, TAO_Notify_EventTypeSeq& seq_remove)
{
  const TAO_Notify_EventType& special = TAO_Notify_EventType::special ();

  if (this->contains (special)) // If this object has the special type.
    {
      // If the added sequence has the special type, nothing else is added or removed.
      if (seq_added.contains (special)) // * overrides.
        {
          seq_added.clear ();   // remove everything from the sequence bening added.
          seq_remove.clear (); // remove everything form whats being removed.
        }
      else // sequence being added does not have *
        {
          if (!seq_added.empty ()) // if sequence being added is non empty
            {
              this->clear ();   // take away the * from this object.
              this->insert_seq (seq_added); // insert the sequence being added as the new list of types.

              seq_remove.clear ();   // reset all that is being removed.
              seq_remove.add (special); // remove *
            }
          else // nothing is being added
            {
              if (seq_remove.contains (special)) // we're removing everything
                {
                  this->clear ();
                  seq_remove.clear ();   // reset all that is being removed.
                  seq_remove.add (special); // remove *
                }
              else if (!seq_remove.empty ())
                {
                  // if we're currently subscribed for everything and did not added anything, we can't remove something.
                  seq_remove.clear ();
                }
            }
        }
    }
  else // if this object does not have the special type.
    {
      if (seq_added.contains (special)) // but the seq. being added has the special type,
        {
          if (seq_remove.contains (special)) // and you're removing * as well
            {
              seq_added.clear ();      // ignore the request
              seq_remove.clear ();  // ignore the request
            }
          else // seq being removed does not have the special type
            {
              seq_remove.clear (); // everything that we're subscribed for is being removed.
              seq_remove.insert_seq (*this);

              this->clear (); // insert the special in this object.
              this->add (special);

              seq_added.clear (); // also clear our set and add only *
              seq_added.add (special);
            }
        }
      else // seq being added does not have special.
        {
          if (seq_remove.contains (special)) // but we're removing everything.
            {
              seq_remove.clear (); // move all that we have currently to removed.
              seq_remove.insert_seq (*this);
            }

          // so now there are no specials anywhere..
            {
              //= First remove the duplicates in the added and removes lists.
              // compute the intersection.

              TAO_Notify_EventTypeSeq common;
              common.intersection (seq_added, seq_remove);

              // remove the common elements from both the lists so Added {BCDK} and Removed {CDEA} will yield Added {BK} and Removed {EA}
              seq_added.remove_seq (common);
              seq_remove.remove_seq (common);

              // If we're already subscribed for an element we should not subscribe again (duplicate events).
              // so if we currently subscribe for ABC and we Added {BK} we should now get ABCK as current subscription and Added {K}
              common.clear ();
              common.intersection (*this, seq_added);
              // remove the common elements from the added list. i,e. doent ask to add what we're already added for.
              seq_added.remove_seq (common);
              // update the current subscription.
              this->insert_seq (seq_added);


              // Similarly for removed.. if we're removing EA and now our current list looks like ABC we should emd up with
              // current subscription BC and Removed {A}
              common.clear ();
              common.intersection (*this, seq_remove);

              seq_remove.clear ();
              seq_remove.insert_seq (common); // only remove what we currently have.

              this->remove_seq (seq_remove);
            }
        }
    }
}

void
TAO_Notify_EventTypeSeq::intersection (const TAO_Notify_EventTypeSeq& rhs, const TAO_Notify_EventTypeSeq& lhs)
{
  // linear search.
  for (TAO_Notify_EventType const & rhs_event_type : rhs)
    for (TAO_Notify_EventType const & lhs_event_type : lhs)
      if (rhs_event_type == lhs_event_type) // if both are same add to this object.
        this->add (rhs_event_type);
}

void
TAO_Notify_EventTypeSeq::dump () const
{
  for (TAO_Notify_EventType const & event_type : this->event_types_)
    {
      event_type.dump ();
      ORBSVCS_DEBUG ((LM_DEBUG, ", "));
    }
}

  // TAO_Notify::Topology_Object
void
TAO_Notify_EventTypeSeq::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  bool changed = this->self_changed_;
  this->self_changed_ = false;
  this->children_changed_ = false;
  TAO_Notify::NVPList attrs;

  if (!this->empty ())
  {
    saver.begin_object(0, "subscriptions", attrs, changed);
    for (TAO_Notify_EventType & event_type : this->event_types_)
    {
      event_type.save_persistent (saver);
    }
// todo:
//    for all deleted children
//    {
//      saver.delete_child(child_type, child_id);
//    }
    saver.end_object(0, "subscriptions");
  }
}

TAO_Notify::Topology_Object*
TAO_Notify_EventTypeSeq::load_child (ACE_CString const & type, CORBA::Long id,
                                     TAO_Notify::NVPList const & attrs)
{
  ACE_UNUSED_ARG (id);
  TAO_Notify::Topology_Object *result = this;
  TAO_Notify_EventType et;

  if ((type == "subscription") && et.init(attrs))
  {
    if (DEBUG_LEVEL) ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Event_Type reload subscription\n")
      ));
    this->add (et);
  }
  return result;
}

void
TAO_Notify_EventTypeSeq::release ()
{
  delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL

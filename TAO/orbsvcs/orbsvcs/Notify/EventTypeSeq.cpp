// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/EventTypeSeq.h"
#include "orbsvcs/Notify/Topology_Saver.h"

#include "tao/debug.h"
//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_EventTypeSeq::TAO_Notify_EventTypeSeq (void)
{
}

TAO_Notify_EventTypeSeq::TAO_Notify_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq)
{
  this->insert_seq (event_type_seq);
}

TAO_Notify_EventTypeSeq &
TAO_Notify_EventTypeSeq::operator = (const TAO_Notify_EventTypeSeq & rhs)
{
  ACE_Unbounded_Set <TAO_Notify_EventType>::operator = (rhs);
  return *this;
}

TAO_Notify_EventTypeSeq::TAO_Notify_EventTypeSeq (const TAO_Notify_EventTypeSeq & rhs)
  : TAO_Notify_Object ()
  , TAO_Notify::Topology_Savable ()
  , ACE_Unbounded_Set <TAO_Notify_EventType> (rhs)
  , TAO_Notify::Topology_Object ()
{
}

void
TAO_Notify_EventTypeSeq::populate (CosNotification::EventTypeSeq& event_type_seq) const
{
  event_type_seq.length (static_cast<CORBA::ULong> (this->size ()));

  inherited::CONST_ITERATOR iter (*this);

  TAO_Notify_EventType* event_type = 0;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    event_type_seq[i] = event_type->native ();
}

void
TAO_Notify_EventTypeSeq::populate_no_special (CosNotification::EventTypeSeq& event_type_seq) const
{
  // If the special exists in us, don't include it.
  const TAO_Notify_EventType& special = TAO_Notify_EventType::special ();

  if (this->find (special) == 0)
    {
      event_type_seq.length (static_cast<CORBA::ULong> (this->size () - 1));
    }
  else
      event_type_seq.length (static_cast<CORBA::ULong> (this->size ()));

  inherited::CONST_ITERATOR iter (*this);

  TAO_Notify_EventType* event_type = 0;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    {
      if (event_type->is_special () == 0) // if its not the special event type.
        event_type_seq[i] = event_type->native ();
    }
}

void
TAO_Notify_EventTypeSeq::insert_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::insert (event_type);
    }
}

void
TAO_Notify_EventTypeSeq::remove_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::remove (event_type);
    }
}

void
TAO_Notify_EventTypeSeq::insert_seq (const TAO_Notify_EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (event_type_seq);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    inherited::insert (*event_type);
}

void
TAO_Notify_EventTypeSeq::remove_seq (const TAO_Notify_EventTypeSeq& event_type_seq)
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (event_type_seq);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    inherited::remove (*event_type);
}

void
TAO_Notify_EventTypeSeq::add_and_remove (TAO_Notify_EventTypeSeq& seq_added, TAO_Notify_EventTypeSeq& seq_remove)
{
  const TAO_Notify_EventType& special = TAO_Notify_EventType::special ();

  if (this->find (special) == 0) // If this object has the special type.
    {
      if (seq_added.find (special) == 0) // if the seq. being added has the special type, you cannot be adding or removing anythings. * overrides.
        {
          seq_added.reset ();   // remove everything from the sequence bening added.
          seq_remove.reset (); // remove everything form whats being removed.
        }
      else // sequence being added does not have *
        {
          if (!seq_added.is_empty ()) // if sequence being added is non empty
            {
              this->reset ();   // take away the * from this object.
              this->insert_seq (seq_added); // insert the sequence being added as the new list of types.

              seq_remove.reset ();   // reset all that is being removed.
              seq_remove.insert (special); // remove *
            }
          else // nothing is being added
            {
              if (seq_remove.find (special) == 0) // we're removing everything
                {
                  this->reset ();
                  seq_remove.reset ();   // reset all that is being removed.
                  seq_remove.insert (special); // remove *
                }
              else if (!seq_remove.is_empty ())
                {
                  // if we're currently subscribed for everything and did not added anything, we can't remove something.
                  seq_remove.reset ();
                }
            }
        }
    }
  else // if this object does not have the special type.
    {
      if (seq_added.find (special) == 0) // but the seq. being added has the special type,
        {
          if (seq_remove.find (special) == 0) // and you're removing * as well
            {
              seq_added.reset ();      // ignore the request
              seq_remove.reset ();  // ignore the request
            }
          else // seq being removed does not have the special type
            {
              seq_remove.reset (); // everything that we're subscribed for is being removed.
              seq_remove.insert_seq (*this);

              this->reset (); // insert the special in this object.
              this->insert (special);

              seq_added.reset (); // also clear our set and add only *
              seq_added.insert (special);
            }
        }
      else // seq being added does not have special.
        {
          if (seq_remove.find (special) == 0) // but we're removing everything.
            {
              seq_remove.reset (); // move all that we have currently to removed.
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
              common.reset ();
              common.intersection (*this, seq_added);
              // remove the common elements from the added list. i,e. doent ask to add what we're already added for.
              seq_added.remove_seq (common);
              // update the current subscription.
              this->insert_seq (seq_added);


              // Similarly for removed.. if we're removing EA and now our current list looks like ABC we should emd up with
              // current subscription BC and Removed {A}
              common.reset ();
              common.intersection (*this, seq_remove);

              seq_remove.reset ();
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
  TAO_Notify_EventTypeSeq::CONST_ITERATOR rhs_iter (rhs);
  TAO_Notify_EventType* rhs_event_type;

  TAO_Notify_EventTypeSeq::CONST_ITERATOR lhs_iter (lhs);
  TAO_Notify_EventType* lhs_event_type;

  for (rhs_iter.first (); rhs_iter.next (rhs_event_type); rhs_iter.advance ())
    {
      for (lhs_iter.first (); lhs_iter.next (lhs_event_type); lhs_iter.advance ())
        {
          if (*rhs_event_type == *lhs_event_type) // if both are same add to this object.
            this->insert (*rhs_event_type);
        }
    }
}

void
TAO_Notify_EventTypeSeq::dump (void) const
{
  TAO_Notify_EventTypeSeq::CONST_ITERATOR iter (*this);

  TAO_Notify_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    {
      event_type->dump ();
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

  TAO_Notify_EventTypeSeq::ITERATOR iter (*this);
  TAO_Notify_EventType* event_type;

  if (this->size() != 0)
  {
    saver.begin_object(0, "subscriptions", attrs, changed);
    for (iter.first (); iter.next (event_type) != 0; iter.advance ())
    {
      event_type->save_persistent(saver);
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
TAO_Notify_EventTypeSeq::load_child (const ACE_CString &type, CORBA::Long id,
  const TAO_Notify::NVPList& attrs)
{
  ACE_UNUSED_ARG (id);
  TAO_Notify::Topology_Object *result = this;
  TAO_Notify_EventType et;

  if ((type == "subscription") && et.init(attrs))
  {
    if (DEBUG_LEVEL) ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Event_Type reload subscription\n")
      ));
    inherited::insert(et);
  }
  return result;
}

void
TAO_Notify_EventTypeSeq::release (void)
{
  delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL

// $Id$

#include "Notification_Receiver_i.h"
#include "Notifier_i.h"

ACE_RCSID(libsrc, Notifier_i, "$Id$")

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)

class Notification_Receiver_Entry
{
  // = TITLE
  //   Keeps track of context information associated with 
  //   a <Event_Comm::Notification_Receiver> entry.
public:
  Notification_Receiver_Entry (Event_Comm::Notification_Receiver *notification_receiver,
			       const char *filtering_criteria);
  ~Notification_Receiver_Entry (void);

  // = Set/get filtering criteria.
  void criteria (const char *criteria);
  const char *criteria (void);

  // = Set/get Event_Comm::Notification_Receiver object reference.
  Event_Comm::Notification_Receiver *receiver (void);
  void receiver (Event_Comm::Notification_Receiver *);

  // = Set/get the compiled regular expression buffer.
  const char *regexp (void);
  void regexp (char *);

private:
  const char *filtering_criteria_;
  // String containing the filtering criteria.

  char *compiled_regexp_;
  // Compiled representation of the regular expression (see
  // regexpr(3g)).

  Event_Comm::Notification_Receiver *receiver_;
  // Object reference for the Event_Comm::Notification_Receiver.
};

// = Set/get filtering criteria.

void 
Notification_Receiver_Entry::criteria (const char *criteria)
{
  ACE_OS::free (ACE_MALLOC_T (this->filtering_criteria_));
  this->filtering_criteria_ = ACE_OS::strdup (criteria);
}

const char *
Notification_Receiver_Entry::criteria (void)
{
  return this->filtering_criteria_;
}

// = Set/get Event_Comm::Notification_Receiver object reference.

Event_Comm::Notification_Receiver *
Notification_Receiver_Entry::receiver (void)
{
  return this->receiver_;
}

void 
Notification_Receiver_Entry::receiver (Event_Comm::Notification_Receiver *receiver)
{
  this->receiver_ = receiver;
}

const char *
Notification_Receiver_Entry::regexp (void)
{
  return this->compiled_regexp_;
}

void 
Notification_Receiver_Entry::regexp (char *regexp)
{
  ACE_OS::free (ACE_MALLOC_T (this->compiled_regexp_));
  this->compiled_regexp_ = regexp;
}

Notification_Receiver_Entry::Notification_Receiver_Entry (Event_Comm::Notification_Receiver *receiver, 
							  const char *filtering_criteria)
  : receiver_ (receiver),
    filtering_criteria_ (0),
    compiled_regexp_ (0)
{
  char *compile_buffer = 0;

  this->criteria (filtering_criteria);
  ACE_ASSERT (this->criteria ());

  // Check for wildcard case first.
  if (ACE_OS::strcmp (filtering_criteria, "") == 0)
    compile_buffer = ACE_OS::strdup ("");
  else 
    // Compile the regular expression (the 0's cause ACE_OS::compile
    // to allocate space).
    compile_buffer = ACE_OS::compile (filtering_criteria, 0, 0);

  // Should throw an exception here!
  ACE_ASSERT (compile_buffer != 0);

  this->regexp (compile_buffer); 
  ACE_ASSERT (this->regexp ());

  // Increment the reference count since we are keeping a copy of
  // this...
  this->receiver_->_duplicate (this->receiver_);
}

Notification_Receiver_Entry::~Notification_Receiver_Entry (void)
{
  ACE_OS::free (this->filtering_criteria_);
  ACE_OS::free (this->compiled_regexp_);
  // Decrement the object reference count.
  CORBA::release (this->receiver_);
}

Notifier_i::Notifier_i (size_t size)
  : map_ (size)
{
}

// Add a new receiver to the table, being careful to check for
// duplicate entries.  A receiver is considered a duplicate under
// the following circumstances:
//   1. It has the same marker name and the same filtering criteria
//   2. It has the same marker name and its filtering criteria is "" (the wild card).

void
Notifier_i::subscribe (Event_Comm::Notification_Receiver *receiver_ref,
		       const char *filtering_criteria,
		       CORBA::Environment &IT_env)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "in Notifier_i::subscribe for %s with filtering criteria \"%s\"\n",
	      receiver_ref->_marker (),
              filtering_criteria));
  ACE_SString key (receiver_ref->_marker ());
  MAP_ITERATOR mi (this->map_);

  // Try to locate an entry using its marker name (which should be
  // unique across the system).  If we don't find the entry, or if the
  // filtering criteria is different that is good news since we
  // currently don't allow duplicates...  In particular, if @@ Should
  // duplicates be allowed?

  for (MAP_ENTRY *me = 0; mi.next (me) != 0; mi.advance ())
    {
      Notification_Receiver_Entry *nr_entry = me->int_id_;

      // Check for a duplicate entry.
      if (key == me->ext_id_
	  && (ACE_OS::strcmp (filtering_criteria, "") == 0
	      || ACE_OS::strcmp (filtering_criteria, nr_entry->criteria ()) == 0))
	{
	  // Inform the caller that the
	  // Event_Comm::Notification_Receiver * is already being
	  // used.

	  errno = EADDRINUSE;
	  ACE_ERROR ((LM_ERROR, 
		      "duplicate entry for receiver %s with criteria \"%s\"",
		     receiver_ref->_marker (),
                      filtering_criteria));
	  // Raise exception here???
	  return;
	}
    }

  // If we get this far then we didn't find a duplicate, so add the
  // new entry!
  Notification_Receiver_Entry *nr_entry;
  ACE_NEW (nr_entry,
           Notification_Receiver_Entry (receiver_ref,
                                        filtering_criteria));
  // Try to add new <Notification_Receiver_Entry> to the map.
  else if (this->map_.bind (key, nr_entry) == -1)
    {
      // Prevent memory leaks.
      delete nr_entry;
      // Raise exception here...
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "bind failed"));
    }
}

// Remove a receiver from the table.  

void
Notifier_i::unsubscribe (Event_Comm::Notification_Receiver *receiver_ref,
			 const char *filtering_criteria,
			 CORBA::Environment &IT_env)
{
  ACE_DEBUG ((LM_DEBUG, 
              "in Notifier_i::unsubscribe for %s\n",
	     receiver_ref->_marker ()));
  Notification_Receiver_Entry *nr_entry = 0;
  ACE_SString key;
  MAP_ITERATOR mi (this->map_);
  int found = 0;

  // Don't make a copy since we are deleting...
  key.rep ((char *) receiver_ref->_marker ());

  // Locate <Notification_Receiver_Entry> and free up resources.  @@
  // Note, we don't properly handle deallocation of KEYS!

  for (MAP_ENTRY *me = 0; mi.next (me) != 0; mi.advance ())
    {
      if (key == me->ext_id_
	  && (ACE_OS::strcmp (filtering_criteria, "") == 0
	      || ACE_OS::strcmp (filtering_criteria, nr_entry->criteria ()) == 0))
	{
	  ACE_DEBUG ((LM_DEBUG,
                      "removed entry %s with criteria \"%s\"\n",
                      receiver_ref->_marker (),
                      filtering_criteria));		     
	  found = 1;
	  // @@ This is a hack, we need a better approach!
	  if (this->map_.unbind (key, nr_entry) == -1)
	    ACE_ERROR ((LM_ERROR,
                        "unbind failed for %s\n", 
		       receiver_ref->_marker ()));
	  else
	    delete nr_entry;
	}
    }

  if (found == 0)
    ACE_ERROR ((LM_ERROR,
                "entry %s with criteria \"%s\" not found\n",
	       receiver_ref->_marker (),
                filtering_criteria));
}

// Disconnect all the receivers, giving them the <reason>.

void
Notifier_i::send_disconnect (const char *reason,
			     CORBA::Environment &IT_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::send_disconnect = %s\n",
              reason));
  MAP_ITERATOR mi (this->map_);
  int count = 0;

  // Notify all the receivers, taking into account the filtering criteria.

  for (MAP_ENTRY *me = 0; mi.next (me) != 0; mi.advance ())
    {
      Event_Comm::Notification_Receiver *receiver_ref = me->int_id_->receiver ();
      ACE_ASSERT (receiver_ref->_marker () != 0);
      ACE_DEBUG ((LM_DEBUG,
                  "disconnecting client %s\n",
                  receiver_ref->_marker ()));
      TRY 
        {
          receiver_ref->disconnect (reason, IT_X);
        }
      CATCHANY 
        {
          cerr << "Unexpected exception " << IT_X << endl;
        }
      ENDTRY;
      delete me->int_id_;
      delete me->ext_id_.rep ();
      count++;
    }

  this->map_.close ();
  if (count == 1)
    ACE_DEBUG ((LM_DEBUG,
                "there was 1 receiver\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "there were %d receivers\n",
                count));
}

// Notify all receivers whose filtering criteria match the event.

void
Notifier_i::send_notification (const Event_Comm::Notification &notification,
			       CORBA::Environment &IT_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::send_notification = %s\n", 
	     notification.tag_));
  MAP_ITERATOR mi (this->map_);
  int count = 0;

  // Notify all the receivers.
  // @@ Later on we need to consider the filtering_criteria!

  for (MAP_ENTRY *me = 0; mi.next (me) != 0; mi.advance ())
    {
      Event_Comm::Notification_Receiver *receiver_ref = me->int_id_->receiver ();
      ACE_ASSERT (receiver_ref->_marker () != 0);
      const char *regexp   = me->int_id_->regexp ();
      const char *criteria = me->int_id_->criteria ();
      ACE_ASSERT (regexp);
      ACE_ASSERT (criteria);

      // Do a regular expression comparison to determine matching.
      if (ACE_OS::strcmp ("", criteria) == 0 // Everything matches the wildcard.
//	  || ACE_OS::strcmp (notification.tag_, regexp) == 0)
	  || ACE_OS::step (notification.tag_, regexp) != 0)
	{
	  ACE_DEBUG ((LM_DEBUG,
                      "string %s matched regexp \"%s\" for client %s\n",
		     notification.tag_, me->int_id_->criteria (), 
		     receiver_ref->_marker ()));
	  TRY 
            {
              receiver_ref->receive_notification (notification, IT_X);
            }
	  CATCHANY 
            {
              cerr << "Unexpected exception " << IT_X << endl;
              continue;
            }
	  ENDTRY;
	  count++;
	}
    }

  if (count == 1)
    ACE_DEBUG ((LM_DEBUG,
                "there was 1 receiver\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "there were %d receivers\n",
                count));
}

#endif /* ACE_HAS_ORBIX */

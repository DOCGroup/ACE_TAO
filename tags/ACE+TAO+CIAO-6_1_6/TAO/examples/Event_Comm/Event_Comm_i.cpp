// $Id$

#include "Event_Comm_i.h"
#include "ace/OS_NS_regex.h"

/**
 * Keeps track of context information associated with
 * a <Event_Comm::Consumer> entry.
 */
class Consumer_Entry
{
public:
  // = Initialization and termination methods.
  Consumer_Entry (Event_Comm::Consumer *consumer,
                  const char *filtering_criteria);
  // Constructor.

  ~Consumer_Entry (void);
  // Descriptor.

  // = Set/get filtering criteria.
  void criteria (const char *criteria);
  const char *criteria (void);

  // = Set/get Event_Comm::Consumer object reference.
  Event_Comm::Consumer *consumer (void);
  void consumer (Event_Comm::Consumer *);

  // = Set/get the compiled regular expression buffer.
  const char *regexp (void);
  void regexp (char *);

private:
  const char *filtering_criteria_;
  // String containing the filtering criteria.

  char *compiled_regexp_;
  // Compiled representation of the regular expression (see
  // regexpr(3g)).

  Event_Comm::Consumer_ptr consumer_;
  // Object reference for the <Event_Comm::Consumer>.
};

// = Set/get filtering criteria.

void
Consumer_Entry::criteria (const char *criteria)
{
  ACE_OS::free ((void *) this->filtering_criteria_);
  ACE_ALLOCATOR (this->filtering_criteria_,
                 ACE_OS::strdup (criteria));
}

const char *
Consumer_Entry::criteria (void)
{
  return this->filtering_criteria_;
}

// = Set/get Event_Comm::Consumer object reference.

Event_Comm::Consumer *
Consumer_Entry::consumer (void)
{
  return this->consumer_;
}

void
Consumer_Entry::consumer (Event_Comm::Consumer *consumer)
{
  this->consumer_ = consumer;
}

const char *
Consumer_Entry::regexp (void)
{
  return this->compiled_regexp_;
}

void
Consumer_Entry::regexp (char *regexp)
{
  ACE_OS::free ((void *) this->compiled_regexp_);
  this->compiled_regexp_ = regexp;
}

Consumer_Entry::Consumer_Entry (Event_Comm::Consumer *consumer,
                                const char *filtering_criteria)
  : filtering_criteria_ (0),
    compiled_regexp_ (0),
    consumer_ (consumer)
{
  char *compile_buffer = 0;

  this->criteria (filtering_criteria);
  ACE_ASSERT (this->criteria ());

  // Check for wildcard case first.
  if (ACE_OS::strcmp (filtering_criteria, "") == 0)
    ACE_ALLOCATOR (compile_buffer,
                   ACE_OS::strdup (""));
  else
  {
#if defined (ACE_HAS_REGEX)
    // Compile the regular expression (the 0's cause ACE_OS::compile
    // to allocate space).
    compile_buffer = ACE_OS::compile (filtering_criteria, 0, 0);
#else
    // Win32 does not support regular expression functions such as compile.
    ACE_ALLOCATOR (compile_buffer,
                   ACE_OS::strdup (""));
#endif // #if defined (ACE_HAS_REGEX)
  }

  // Should throw an exception here!
  ACE_ASSERT (compile_buffer != 0);

  this->regexp (compile_buffer);
  ACE_ASSERT (this->regexp () != 0);

  // Increment the reference count since we are keeping a copy of
  // this...
  this->consumer_ = Event_Comm::Consumer::_duplicate (this->consumer_);
}

Consumer_Entry::~Consumer_Entry (void)
{
  ACE_OS::free ((void *) this->filtering_criteria_);
  ACE_OS::free ((void *) this->compiled_regexp_);
  // Decrement the object reference count.
  CORBA::release (this->consumer_);
}

Notifier_i::Notifier_i (size_t size)
  : map_ (size)
{
// if platforms (such as win32) do not support the REGEXP functions
// such as <compile> and <step> then warn the user that the regular
// expression feature is not available.
#ifndef ACE_HAS_REGEX
  ACE_DEBUG ((LM_DEBUG, "\n WARNING: This platform does not support \
the functions for regular expressions.\n\
The filtering criteria will not work.\n"));
#endif //#ifndef ACE_HAS_REGEX
}

// Add a new consumer to the table, being careful to check for
// duplicate entries.  A consumer is considered a duplicate under the
// following circumstances:
//
//   1. It has the same object reference and the same filtering
//      criteria.
//   2. It has the same object reference and its filtering criteria is
//      "" (the wild card).

void
Notifier_i::subscribe (Event_Comm::Consumer_ptr consumer_ref,
                       const char *filtering_criteria)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::subscribe for %x with filtering criteria \"%s\"\n",
              consumer_ref,
              filtering_criteria));

  MAP_ITERATOR mi (this->map_);

  // Try to locate an entry checking if the object references are
  // equivalent.  If we don't find the entry, or if the filtering
  // criteria is different that is good news since we currently don't
  // allow duplicates...  @@ Should duplicates be allowed?

  for (MAP_ENTRY *me = 0; mi.next (me) != 0; mi.advance ())
    {
      Consumer_Entry *nr_entry = me->int_id_;

      // The <_is_equivalent> function checks if objects are the same.
      // NOTE: this call might not behave well on other ORBs since
      // <_is_equivalent> isn't guaranteed to differentiate object
      // references.

      // Check for a duplicate entry.
      if (consumer_ref->_is_equivalent (me->ext_id_)
          && (ACE_OS::strcmp (filtering_criteria,
                              "") == 0
              || ACE_OS::strcmp (filtering_criteria,
                                 nr_entry->criteria ()) == 0))
        {
          // Inform the caller that the <Event_Comm::Consumer> * is
          // already being used.

          throw Event_Comm::Notifier::CannotSubscribe (
            "Duplicate consumer and filtering criteria found.\n");
        }
    }

  // If we get this far then we didn't find a duplicate, so add the
  // new entry!
  Consumer_Entry *nr_entry;
  ACE_NEW (nr_entry,
           Consumer_Entry (consumer_ref,
                           filtering_criteria));

  // Try to add new <Consumer_Entry> to the map.
  if (this->map_.bind (nr_entry->consumer(), nr_entry) == -1)
    {
      // Prevent memory leaks.
      delete nr_entry;
      throw Event_Comm::Notifier::CannotSubscribe (
        "Failed to add Consumer to internal map\n");
    }
}

// Remove a consumer from the table.

void
Notifier_i::unsubscribe (Event_Comm::Consumer_ptr consumer_ref,
                         const char *filtering_criteria)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::unsubscribe for %x\n",
              consumer_ref));

  Consumer_Entry *nr_entry = 0;
  MAP_ITERATOR mi (this->map_);
  int found = 0;

  // Locate <Consumer_Entry> and free up resources.  @@ Note, we don't
  // properly handle deallocation of KEYS!

  for (MAP_ENTRY *me = 0;
       mi.next (me) != 0;
       mi.advance ())
    {
      nr_entry = me->int_id_;

      // The <_is_equivalent> function checks if objects are the same.
      // NOTE: this call might not behave well on other ORBs since
      // <_is_equivalent> isn't guaranteed to differentiate object
      // references.

      // Look for a match ..
      if (consumer_ref->_is_equivalent (me->ext_id_)
          && (ACE_OS::strcmp (filtering_criteria, "") == 0
              || ACE_OS::strcmp (filtering_criteria,
                                 nr_entry->criteria ()) == 0))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "removed entry %x with criteria \"%s\"\n",
                      consumer_ref,
                      filtering_criteria));
          found = 1;
          // @@ This is a hack, we need a better approach!
          if (this->map_.unbind (me->ext_id_,
                                 nr_entry) == -1)
            throw Event_Comm::Notifier::CannotUnsubscribe (
              "Internal map unbind failed.");
          else
            delete nr_entry;
        }
    }

  if (found == 0)
    throw Event_Comm::Notifier::CannotUnsubscribe (
      "The Consumer and filtering criteria were not found.");
}

// Disconnect all the consumers, giving them the <reason>.

void
Notifier_i::disconnect (const char *reason)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::send_disconnect = %s\n",
              reason));

  MAP_ITERATOR mi (this->map_);
  int count = 0;

  // Notify all the consumers, taking into account the filtering
  // criteria.

  for (MAP_ENTRY *me = 0;
       mi.next (me) != 0;
       mi.advance ())
    {
      Event_Comm::Consumer_ptr consumer_ref =
        me->ext_id_;

      ACE_ASSERT (consumer_ref != 0);
      ACE_DEBUG ((LM_DEBUG,
                  "disconnecting client %x\n",
                  consumer_ref));
      try
        {
          consumer_ref->disconnect (reason);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Unexpected exception\n");
        }

      delete me->int_id_;
      count++;
    }

  this->map_.close ();

  if (count == 1)
    ACE_DEBUG ((LM_DEBUG,
                "there was 1 consumer\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "there were %d consumers\n",
                count));
}

// Notify all consumers whose filtering criteria match the event.

void
Notifier_i::push (const Event_Comm::Event &event)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::send_notification = %s\n",
              (const char *) event.tag_));
  MAP_ITERATOR mi (this->map_);
  int count = 0;

  // Notify all the consumers.

  for (MAP_ENTRY *me = 0; mi.next (me) != 0; mi.advance ())
    {
      Event_Comm::Consumer_ptr consumer_ref = me->int_id_->consumer ();
      ACE_ASSERT (consumer_ref != 0);

#if defined (ACE_HAS_REGEX)
      char *regexp = const_cast<char *> (me->int_id_->regexp ());
      ACE_ASSERT (regexp);

      const char *criteria = me->int_id_->criteria ();
      ACE_ASSERT (criteria);

      // Do a regular expression comparison to determine matching.
      if (ACE_OS::strcmp ("", criteria) == 0 // Everything matches the wildcard.
          || ACE_OS::step (event.tag_, regexp) != 0)
#endif // #if defined (ACE_HAS_REGEX)
        // if ACE_HAS_REGEX has not been defined,
        // let everything through.
        {
          ACE_DEBUG ((LM_DEBUG,
                      "string %s matched regexp \"%s\" for client %x\n",
                      (const char *) event.tag_,
                      me->int_id_->criteria (),
                      consumer_ref));
          try
            {
              consumer_ref->push (event);
            }
          catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception ("Unexpected exception\n");
              continue;
            }
          count++;
        }
    }

  if (count == 1)
    ACE_DEBUG ((LM_DEBUG,
                "there was 1 consumer\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "there were %d consumers\n",
                count));
}

// -------------

ShutdownCallback::~ShutdownCallback (void)
{
}

// -------------

//FUZZ: disable check_for_lack_ACE_OS
Consumer_i::Consumer_i (void)
  : shutdown (0)
{
}
//FUZZ: enable check_for_lack_ACE_OS

Consumer_i::~Consumer_i (void)
{
}

// Inform the <Event_Comm::Consumer> that <event> has
// occurred.

void
Consumer_i::push (const Event_Comm::Event &event)
{
  const char *tmpstr = event.tag_;

  ACE_DEBUG ((LM_DEBUG,
              "**** got notification = %s\n",
              tmpstr));
}

// Disconnect the <Event_Comm::Consumer> from the
// <Event_Comm::Notifier>.

void
Consumer_i::disconnect (const char *reason)
{
  ACE_DEBUG ((LM_DEBUG,
              "**** got disconnected due to %s\n",
              reason));

  ACE_ASSERT (shutdown != 0);

  shutdown->close ();
}

void
Consumer_i::set (ShutdownCallback *_shutdown)
{
  shutdown = _shutdown;
}


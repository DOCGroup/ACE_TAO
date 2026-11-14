#include "Event_Comm_i.h"
#include "ace/OS_NS_regex.h"

#include <memory>
#include <string>

struct Compiled_Regex_Deleter
{
  void operator() (char *regex) const
  {
    ACE_OS::free (regex);
  }
};

/**
 * Keeps track of context information associated with
 * a <Event_Comm::Consumer> entry.
 */
class Consumer_Entry
{
public:
  Consumer_Entry (Event_Comm::Consumer *consumer,
                 char const *filtering_criteria);

  ~Consumer_Entry ();

  Consumer_Entry (Consumer_Entry const &) = delete;
  Consumer_Entry &operator= (Consumer_Entry const &) = delete;
  Consumer_Entry (Consumer_Entry &&) = delete;
  Consumer_Entry &operator= (Consumer_Entry &&) = delete;

  std::string const &criteria () const;

  Event_Comm::Consumer *consumer () const;

  char *regexp ();

private:
  /// Filtering criteria associated with the consumer.
  std::string filtering_criteria_;

  /// Compiled representation of the regular expression.
  std::unique_ptr<char, Compiled_Regex_Deleter> compiled_regexp_ {};

  /// Object reference for the consumer.
  Event_Comm::Consumer_ptr consumer_ {};
};

std::string const &
Consumer_Entry::criteria () const
{
  return this->filtering_criteria_;
}

Event_Comm::Consumer *
Consumer_Entry::consumer () const
{
  return this->consumer_;
}

char *
Consumer_Entry::regexp ()
{
  return this->compiled_regexp_.get ();
}

Consumer_Entry::Consumer_Entry (Event_Comm::Consumer *consumer,
                                char const *filtering_criteria)
  : filtering_criteria_ (filtering_criteria)
  , consumer_ (consumer)
{
  char *compile_buffer {};

  ACE_ASSERT (filtering_criteria != nullptr);

  // Check for wildcard case first.
  if (*filtering_criteria == '\0')
    ACE_ALLOCATOR (compile_buffer,
                   ACE_OS::strdup (""));
  else
  {
#if defined (ACE_HAS_REGEX)
    // Compile the regular expression (the 0's cause ACE_OS::compile
    // to allocate space).
    compile_buffer = ACE_OS::compile (filtering_criteria, nullptr, nullptr);
#else
    // Win32 does not support regular expression functions such as compile.
    ACE_ALLOCATOR (compile_buffer,
                   ACE_OS::strdup (""));
#endif // #if defined (ACE_HAS_REGEX)
  }

  // Should throw an exception here!
  ACE_ASSERT (compile_buffer != nullptr);

  this->compiled_regexp_.reset (compile_buffer);
  ACE_ASSERT (this->regexp () != nullptr);

  // Increment the reference count since we are keeping a copy of
  // this...
  this->consumer_ = Event_Comm::Consumer::_duplicate (this->consumer_);
}

Consumer_Entry::~Consumer_Entry ()
{
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

/// Adds a new consumer after checking for duplicate entries. A consumer is
/// considered a duplicate when it has the same object reference and matching
/// filtering criteria, or when the criteria is empty (the wildcard).

void
Notifier_i::subscribe (Event_Comm::Consumer_ptr consumer_ref,
                       char const *filtering_criteria)
{
  std::string const criteria (filtering_criteria);

  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::subscribe for %x with filtering criteria \"%s\"\n",
              consumer_ref,
              filtering_criteria));

  MAP_ITERATOR mi (this->map_);

  // Try to locate an entry checking if the object references are
  // equivalent.  If we don't find the entry, or if the filtering
  // criteria is different that is good news since we currently don't
  // allow duplicates...  @@ Should duplicates be allowed?

  for (MAP_ENTRY *me {}; mi.next (me) != 0; mi.advance ())
    {
      Consumer_Entry const * const nr_entry = me->int_id_;

      // The <_is_equivalent> function checks if objects are the same.
      // NOTE: this call might not behave well on other ORBs since
      // <_is_equivalent> isn't guaranteed to differentiate object
      // references.

      // Check for a duplicate entry.
      if (consumer_ref->_is_equivalent (me->ext_id_)
          && (criteria.empty () || criteria == nr_entry->criteria ()))
        {
          // Inform the caller that the <Event_Comm::Consumer> * is
          // already being used.

          throw Event_Comm::Notifier::CannotSubscribe (
            "Duplicate consumer and filtering criteria found.\n");
        }
    }

  // If we get this far then we didn't find a duplicate, so add the
  // new entry!
  std::unique_ptr<Consumer_Entry> nr_entry {
    new Consumer_Entry (consumer_ref, filtering_criteria)};

  // Try to add new <Consumer_Entry> to the map.
  if (this->map_.bind (nr_entry->consumer (), nr_entry.get ()) == -1)
    {
      throw Event_Comm::Notifier::CannotSubscribe (
        "Failed to add Consumer to internal map\n");
    }

  nr_entry.release ();
}

/// Removes a consumer from the table.

void
Notifier_i::unsubscribe (Event_Comm::Consumer_ptr consumer_ref,
                         char const *filtering_criteria)
{
  std::string const criteria (filtering_criteria);

  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::unsubscribe for %x\n",
              consumer_ref));

  MAP_ITERATOR mi (this->map_);
  bool found {};

  // Locate <Consumer_Entry> and free up resources.  @@ Note, we don't
  // properly handle deallocation of KEYS!

  for (MAP_ENTRY *me {};
       mi.next (me) != 0;
       mi.advance ())
    {
      Consumer_Entry *nr_entry = me->int_id_;

      // The <_is_equivalent> function checks if objects are the same.
      // NOTE: this call might not behave well on other ORBs since
      // <_is_equivalent> isn't guaranteed to differentiate object
      // references.

      // Look for a match ..
      if (consumer_ref->_is_equivalent (me->ext_id_)
          && (criteria.empty () || criteria == nr_entry->criteria ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "removed entry %x with criteria \"%s\"\n",
                      consumer_ref,
                      filtering_criteria));
          // @@ This is a hack, we need a better approach!
          if (this->map_.unbind (me->ext_id_,
                                 nr_entry) == -1)
            throw Event_Comm::Notifier::CannotUnsubscribe (
              "Internal map unbind failed.");
          else
            {
              found = true;
              std::unique_ptr<Consumer_Entry> owner {nr_entry};
            }
        }
    }

  if (!found)
    throw Event_Comm::Notifier::CannotUnsubscribe (
      "The Consumer and filtering criteria were not found.");
}

/// Disconnects all consumers, giving them the @a reason.

void
Notifier_i::disconnect (char const *reason)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::send_disconnect = %s\n",
              reason));

  MAP_ITERATOR mi (this->map_);
  int count {};

  // Notify all the consumers, taking into account the filtering
  // criteria.

  for (MAP_ENTRY *me {};
       mi.next (me) != 0;
       mi.advance ())
    {
      Event_Comm::Consumer_ptr const consumer_ref =
        me->ext_id_;
      std::unique_ptr<Consumer_Entry> const entry {me->int_id_};

      ACE_ASSERT (consumer_ref != nullptr);
      ACE_DEBUG ((LM_DEBUG,
                  "disconnecting client %x\n",
                  consumer_ref));
      try
        {
          consumer_ref->disconnect (reason);
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Unexpected exception\n");
        }

      ++count;
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

/// Notifies all consumers whose filtering criteria match the @a event.

void
Notifier_i::push (Event_Comm::Event const &event)
{
  ACE_DEBUG ((LM_DEBUG,
              "in Notifier_i::send_notification = %s\n",
              static_cast<char const *> (event.tag_)));
  MAP_ITERATOR mi (this->map_);
  int count {};

  // Notify all the consumers.

  for (MAP_ENTRY *me {}; mi.next (me) != 0; mi.advance ())
    {
      Event_Comm::Consumer_ptr const consumer_ref = me->int_id_->consumer ();
      ACE_ASSERT (consumer_ref != nullptr);
      std::string const &criteria = me->int_id_->criteria ();

#if defined (ACE_HAS_REGEX)
      char * const regexp = me->int_id_->regexp ();
      ACE_ASSERT (regexp);

      // Do a regular expression comparison to determine matching.
      if (criteria.empty () // Everything matches the wildcard.
          || ACE_OS::step (event.tag_, regexp) != 0)
#endif // #if defined (ACE_HAS_REGEX)
        // if ACE_HAS_REGEX has not been defined,
        // let everything through.
        {
          ACE_DEBUG ((LM_DEBUG,
                      "string %s matched regexp \"%s\" for client %x\n",
                      static_cast<char const *> (event.tag_),
                      criteria.c_str (),
                      consumer_ref));
          try
            {
              consumer_ref->push (event);
            }
          catch (CORBA::Exception const& ex)
            {
              ex._tao_print_exception ("Unexpected exception\n");
              continue;
            }
          ++count;
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

/// Informs the @a Consumer that the @a event has occurred.

void
Consumer_i::push (Event_Comm::Event const &event)
{
  char const *tmpstr = event.tag_;

  ACE_DEBUG ((LM_DEBUG,
              "**** got notification = %s\n",
              tmpstr));
}

/// Disconnects the @a Consumer from the @a Notifier.

void
Consumer_i::disconnect (char const *reason)
{
  ACE_DEBUG ((LM_DEBUG,
              "**** got disconnected due to %s\n",
              reason));

  ACE_ASSERT (shutdown_ != nullptr);

  shutdown_->close ();
}

void
Consumer_i::set (ShutdownCallback *shutdown_callback)
{
  shutdown_ = shutdown_callback;
}

